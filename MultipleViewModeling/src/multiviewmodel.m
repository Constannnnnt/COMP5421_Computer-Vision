clear global;

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
        disp(N);
        if (N(3, 3) > 0),
            normals(i,j,:) = N(:, 3);
        else
            normals(i,j,:) = -N(:, 3);
        end
    end
end

%% Minimization via Graph Cuts
refine_vertices = icosahedron(5);
refine_normals = reshape(normals, [], 3);
refine_IDX = knnsearch(refine_vertices, refine_normals);
[refine_vertex_IDX, refine_ia, refine_ic] = unique(IDX);
refine_vertices = refine_vertices(refine_vertex_IDX, :);

E_data 
