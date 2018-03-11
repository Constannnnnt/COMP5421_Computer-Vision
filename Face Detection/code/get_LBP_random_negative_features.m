
function LBP_feature_neg = get_LBP_random_negative_features(non_face_scn_path, feature_params, num_samples)

image_files = dir( fullfile( non_face_scn_path, '*.jpg' ));
num_images = length(image_files);
num_samples_per_img = ceil(num_samples/num_images);

D = 256;
LBP_feature_neg = zeros(num_samples, D);

% For best performance, you should sample random negative examples at multiple scales.
for i = 1:num_images

	img_path = fullfile( non_face_scn_path, image_files(i).name );
	img = imread(img_path);

	if size(img, 3) == 3
	    img = rgb2gray(img);
	end

	[height, width] = size(img);

	for j = 1:num_samples_per_img
		
		top_left_height = ceil( rand()*(height-feature_params.template_size) );
		top_left_width = ceil( rand()*(width-feature_params.template_size) );

		img_crop = img(top_left_height:top_left_height+feature_params.template_size-1, top_left_width:top_left_width+feature_params.template_size-1);

		index = (i-1)*num_samples_per_img + j;

		features = LBP(img_crop, false);
		LBP_feature_neg(index, 1:end) = reshape(features, 1, D);

	end
end


