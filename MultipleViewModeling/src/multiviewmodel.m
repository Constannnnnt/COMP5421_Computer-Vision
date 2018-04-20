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

