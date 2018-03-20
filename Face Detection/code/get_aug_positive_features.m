
function features_pos_aug = get_aug_positive_features(train_path_pos, feature_params)
% 'train_path_pos' is a string. This directory contains 36x36 images of faces
%   
% 'feature_params' is a struct, with fields
%   feature_params.template_size (probably 36), the number of pixels
%      spanned by each train / test template and
%   feature_params.hog_cell_size (default 6), the number of pixels in each
%      HoG cell. template size should be evenly divisible by hog_cell_size.
%      Smaller HoG cell sizes tend to work better, but they make things
%      slower because the feature dimensionality increases and more
%      importantly the step size of the classifier decreases at test time.


% 'features_pos_aug' is N by D matrix where N is the number of faces and D
% is the template dimensionality, which would be
%   (feature_params.template_size / feature_params.hog_cell_size)^2 * 31
% if you're using the default vl_hog parameters

% Useful functions:
% vl_hog, HOG = VL_HOG(IM, CELLSIZE)
%  http://www.vlfeat.org/matlab/vl_hog.html  (API)
%  http://www.vlfeat.org/overview/hog.html   (Tutorial)
% rgb2gray


% Caltech Faces stored as .jpg
% dir() returns a 6713*1 struct array
% eg. image_files(1).name = caltech_web_crop_00001.jpg
image_files = dir( fullfile( train_path_pos, '*.jpg') ); 
num_images = length(image_files);

% rand() returns (0,1)
D = (feature_params.template_size / feature_params.hog_cell_size)^2 * 31;
features_pos_aug = zeros(num_images*3, D);

% use of vl_hog, 31 comes from the use of default UoCTTI variant
% vl_hog: image: [36 x 36 x 1]
% vl_hog: descriptor: [6 x 6 x 31]

% For improved performance, try mirroring or warping the positive training examples.
for i = 1:num_images
	% read image
	img_path = fullfile( train_path_pos, image_files(i).name );
	img = imread(img_path);

	% if rgb, convert to gray
    if size(img, 3) == 3
	    img = rgb2gray(img);
    end

	% use of hog
	img_flip = flip(img ,2); % horizontal flip
	img_constrast = img * 0.8;
    img_tmp = imresize(img, 1+rand()*0.5);
    start_index = 0;
    if size(img_tmp, 1) > size(img, 1)
        start_index = int8(floor((size(img_tmp, 1) - size(img,1)) / 2));
    end
    img_crop = img_tmp;
    if start_index ~= 0
        img_crop = img_tmp(start_index:start_index+size(img, 1), start_index:start_index+size(img,2));
    end
	
	hog_flip = vl_hog(single(img_flip), feature_params.hog_cell_size);
	hog_contrast = vl_hog(single(img_constrast), feature_params.hog_cell_size);
    hog_crop = vl_hog(single(img_crop), feature_params.hog_cell_size);

	features_pos_aug( 3*(i-1)+1, 1:end ) = reshape(hog_flip, 1, D);
	features_pos_aug( 3*(i-1)+2, 1:end ) = reshape(hog_contrast, 1, D);
    features_pos_aug( 3*(i-1)+3, 1:end ) = reshape(hog_crop, 1, D);

end
