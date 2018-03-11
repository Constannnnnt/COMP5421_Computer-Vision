
function LBP_feature_pos = get_LBP_postive_features(train_path_pos, feature_params)

image_files = dir( fullfile( train_path_pos, '*.jpg') ); 
num_images = length(image_files);

D = 256;
LBP_feature_pos = zeros(num_images, D);

for i = 1:num_images
	% read image
	img_path = fullfile( train_path_pos, image_files(i).name );
	img = imread(img_path);

	% if rgb, convert to gray
	if size(img, 3) == 3
	    img = rgb2gray(img);
	end

	features = LBP(img, false);
	LBP_feature_pos(i, 1:end) = reshape(features, 1, D);

end

