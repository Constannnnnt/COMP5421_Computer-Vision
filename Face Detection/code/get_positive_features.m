% Starter code prepared by James Hays for CS 143, Brown University
% This function should return all positive training examples (faces) from
% 36x36 images in 'train_path_pos'. Each face should be converted into a
% HoG template according to 'feature_params'. For improved performance, try
% mirroring or warping the positive training examples.

function features_pos = get_positive_features(train_path_pos, feature_params)
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


% 'features_pos' is N by D matrix where N is the number of faces and D
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
features_pos = zeros(num_images, D);

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
	hog = vl_hog(single(img), feature_params.hog_cell_size);
	features_pos(i, 1:end) = reshape(hog, 1, D);

end

