clear global;
clear;

%% setup
addpath(genpath('../lib/'));
datapath = '../data/data02/';
lightvecpath = '../data/data02/lightvec.txt';
lightvec = load(lightvecpath);
image_files = dir(fullfile(datapath, '*.bmp'));
num_images = size(image_files, 1);

%% uniform resampling
vertices = icosahedron(4);
IDX = knnsearch(vertices, lightvec);
[vertex_IDX, ia, ic] = unique(IDX);
num_direction = size(vertex_IDX, 1);
sample_img = imread(fullfile(datapath, image_files(1).name));
Io = double(zeros([size(sample_img) num_direction]));
sum_LoLi = zeros(num_direction, 1);

for i = 1 : num_images,
    img = double(imread(fullfile(datapath, image_files(i).name)));
    LoLi = vertices(IDX(i),:) * lightvec(i, :)';
    sum_LoLi(ic(i)) = sum_LoLi(ic(i)) + LoLi;
    Io(:, :, :, ic(i)) = Io(:,:,:, ic(i)) + img * LoLi;
end

for i = 1 : num_direction,
    Io(:, :, :, i) = Io(:, :, :, i) / sum_LoLi(i);
end

%% denominator image
% we need to convert the rgb images to grayscale because if we have three
% chanels, it is hard to rank the pixels
gray_images = double(zeros([size(sample_img, 1) size(sample_img,2) num_direction]));
for i = 1 : num_direction,
    % Matlab algorithms: rgb2gray converts RGB values to grayscale values by forming a
    % weighted sum of the R, G, and B components: 0.2989 * R + 0.5870 * G + 0.1140 * B
    % If we just do rgb2gray, all gray vlaues will become 1 because of the
    % thresholding
    gray_images(:,:, i) = 0.2989 * Io(:,:,1, i) + 0.5870 * Io(:,:,2, i) + 0.1140 * Io(:,:,3, i);
end
% figure
% imshow(Io(:, :, :, 200));
% imshow(gray_images(:, : , 1));

pixel_rank = zeros(size(gray_images));
light_rank = zeros(num_direction, 1);
for i = 1 : size(sample_img, 1),
    for j = 1 : size(sample_img, 2),
        pixel = squeeze(gray_images(i ,j, :));
        [~, idx] = sort(pixel);
        light_rank(idx) = 1 : num_direction;
        pixel_rank(i, j, :) = light_rank;
    end
end

L = 0.7 * num_direction;
H = 0.9 * num_direction;

k_L = zeros(num_direction, 1);
r_L = zeros(num_direction, 1);

for i = 1 : num_direction,
    rank = pixel_rank(:, :, i) > L;
    k_L(i) = sum(sum(rank(:)));
    r_L(i) = mean2(rank);
end

denominator = k_L .* (r_L < H);
[~, denominator_idx] = max(denominator);
% disp(denominator_idx);

%% Local Normal Estimation by Ratio Images
% to eliminate p, we divide k - 1 resampled images by the denominator image
% to obtain k - 1 ratio images
ratio_images = gray_images;
% note that in the energy function we should have normals, therefore we
% should store the normal values now
denominator_images = ratio_images(:, :, denominator_idx);
normals = zeros([size(sample_img, 1) size(sample_img, 2) 3]);
% remove reduntant light vectors
lightvec = vertices(vertex_IDX, :);
% get denominator light
denominator_light = lightvec(denominator_idx, :);
newlightvec = lightvec(1 : denominator_idx - 1, :);
newlightvec = [newlightvec; lightvec(denominator_idx + 1 : end, :)];

for i = 1 : size(sample_img, 1),
    for j =  1 : size(sample_img, 2),
        denominator_pixel = denominator_images(i, j);
        ratio_pixel = [squeeze(ratio_images(i, j, 1 : denominator_idx - 1)); ...
            squeeze(ratio_images(i,j, denominator_idx + 1 : end))];
        X = ratio_pixel * denominator_light - denominator_pixel * newlightvec;
        [~, ~, N] = svd(X);
        if (N(3, 3) > 0),
            normals(i,j,:) = N(:, 3);
        else
            normals(i,j,:) = -N(:, 3);
        end
    end
end
figure, imshow(normals);

%% Minimization via Graph Cuts
refine_vertices = icosahedron(5);
refine_normals = reshape(normals, [], 3);
refine_IDX = knnsearch(refine_vertices, refine_normals);
[refine_vertex_IDX, refine_ia, refine_ic] = unique(refine_IDX);
refine_vertices = refine_vertices(refine_vertex_IDX, :);

% data term: per-pixel difference between the measured and the estimated
% ratio images, here, the measured images are icosahedron(5);
E_data = pdist2(refine_vertices, refine_normals);
lambda = 0.5; % how to define lambda?
sigma = 0.85;  % how to define sigma?
E_smooth = lambda * log(1 + pdist2(refine_vertices, refine_vertices) / (2 * sigma * sigma));

% since the datatype is double in E_data and E_smooth, and the value is
% small, around 1.xxxx or something. and we need int32 for the GCO lib. 
% If we directly convert it and use the lib, it will do nothing since it is almost 0 and 1. 
% We need to scale it so that the edge cost is effective
E_data = int32(E_data .* 10000);
E_smooth = int32(E_smooth .* 10000);

labels = refine_vertices;
L = size(labels, 1);
norm_size = size(normals);
% except the denominator image
img_height = size(sample_img, 1);
img_width = size(sample_img, 2);
edge_num = (img_height - 1) * (img_width) + (img_height) * (img_width - 1);
pixel_num = norm_size(1) * norm_size(2);

% construct neighboring matrix
Si = zeros(edge_num, 1);
Sj = zeros(edge_num, 1);
Sv = ones(edge_num, 1);
counter = 0;

for i = 1 : (img_height - 1),
    for j = 1 : img_width,
        counter = counter + 1;
        Si(counter) = (i - 1) * img_width + j;
        Sj(counter) = i * img_width + j;
    end
end

for i = 1 : img_height,
    for j = 1 : (img_width - 1),
        counter = counter + 1;
        Si(counter) = (i - 1) * img_width + j;
        Sj(counter) = (i - 1) * img_width + j + 1;
    end
end

neighboring_matrix = sparse(Si, Sj, Sv, pixel_num, pixel_num);

% Using GCO 
handle = GCO_Create(pixel_num, L);
GCO_SetDataCost(handle, E_data);
GCO_SetSmoothCost(handle, E_smooth);
GCO_SetNeighbors(handle, neighboring_matrix);
GCO_Expansion(handle);
optimal_label = GCO_GetLabeling(handle);
[E, D, S] = GCO_ComputeEnergy(handle);
GCO_Delete(handle);

optimal_normals = refine_vertices(optimal_label, :);
optimal_normals = reshape(optimal_normals, [img_height, img_width, 3]);
figure, imshow((-1/sqrt(3) * optimal_normals(:,:,1) + 1/sqrt(3) * optimal_normals(:,:,2) + 1/sqrt(3) * optimal_normals(:,:,3)) / 1.1);

%% 
