% Starter code prepared by James Hays for CS 143, Brown University
% This function returns detections on all of the images in a given path.
% You will want to use non-maximum suppression on your detections or your
% performance will be poor (the evaluation counts a duplicate detection as
% wrong). The non-maximum suppression is done on a per-image basis. The
% starter code includes a call to a provided non-max suppression function.

function [bboxes, confidences, image_ids, mh_features_neg] = .... 
    run_detector(test_scn_path, w, b, feature_params, mine_hard)
% 'test_scn_path' is a string. This directory contains images which may or
%    may not have faces in them. This function should work for the MIT+CMU
%    test set but also for any other images (e.g. class photos)
% 'w' and 'b' are the linear classifier parameters
% 'feature_params' is a struct, with fields
%   feature_params.template_size (probably 36), the number of pixels
%      spanned by each train / test template and
%   feature_params.hog_cell_size (default 6), the number of pixels in each
%      HoG cell. template size should be evenly divisible by hog_cell_size.
%      Smaller HoG cell sizes tend to work better, but they make things
%      slower because the feature dimensionality increases and more
%      importantly the step size of the classifier decreases at test time.

% 'bboxes' is Nx4. N is the number of detections. bboxes(i,:) is
%   [x_min, y_min, x_max, y_max] for detection i. 
%   Remember 'y' is dimension 1 in Matlab!
% 'confidences' is Nx1. confidences(i) is the real valued confidence of
%   detection i.
% 'image_ids' is an Nx1 cell array. image_ids{i} is the image file name
%   for detection i. (not the full path, just 'albert.jpg')

% The placeholder version of this code will return random bounding boxes in
% each test image. It will even do non-maximum suppression on the random
% bounding boxes to give you an example of how to call the function.

% Your actual code should convert each test image to HoG feature space with
% a _single_ call to vl_hog for each scale. Then step over the HoG cells,
% taking groups of cells that are the same size as your learned template,
% and classifying them. If the classification is above some confidence,
% keep the detection and then pass all the detections for an image to
% non-maximum suppression. For your initial debugging, you can operate only
% at a single scale and you can skip calling non-maximum suppression.

test_scenes = dir( fullfile( test_scn_path, '*.jpg' ));

%initialize parameters
cell_size = feature_params.hog_cell_size;
cell_num = feature_params.template_size / feature_params.hog_cell_size;
D = 256;
%scales = [1];

%initialize these as empty and incrementally expand them.
bboxes = zeros(0,4);
confidences = zeros(0,1);
image_ids = cell(0,1);
mh_features_neg = zeros(0,D);

for k = 1:length(test_scenes)
      
    fprintf('Detecting faces in %s\n', test_scenes(k).name)
    img = imread( fullfile( test_scn_path, test_scenes(k).name ));
    img = single(img)/255;
    if(size(img,3) > 1)
        img = rgb2gray(img);
    end

    % Then step over the HoG cells,
    % taking groups of cells that are the same size as your learned template,
    % and classifying them. If the classification is above some confidence,
    % keep the detection and then pass all the detections for an image to
    % non-maximum suppression. For your initial debugging, you can operate only
    % at a single scale and you can skip calling non-maximum suppression.

    cur_bboxes = zeros(0,4);
    cur_confidences = zeros(0,1);
    cur_image_ids = cell(0,1);
    cur_mh_features_neg = zeros(0,D);

    for scale = 0.1:0.05:1.1
        img_scaled = imresize(img, scale);
        % [height, width] = size(img_scaled);

        hog_features = LBP(single(img_scaled), false);

        num_y_detection = size(hog_features,1) - cell_num + 1;
        num_x_detection = size(hog_features,2) - cell_num + 1;

        s_window_features = zeros(num_y_detection*num_x_detection, D);

        for i = 1:num_y_detection
            for j = 1:num_x_detection
                window_index = (i-1)*num_x_detection + j;
                s_window_features(window_index,:) = reshape( hog_features(i:i+cell_num-1, j:j+cell_num-1, :), 1, D);
            end
        end

        scores = s_window_features * w + b;
        if(mine_hard == true)
            idx = find(scores > 2.1);
        else    
            idx = find(scores > 0.95);
        end

        % find cur_scale_bboxes
        idx_j = mod(idx, num_x_detection);
        idx_i = (idx - idx_j)/num_x_detection + 1;

        cur_y_min = (idx_i-1) * cell_size + 1;
        cur_x_min = (idx_j-1) * cell_size + 1;
        cur_scale_bboxes = [cur_x_min, cur_y_min, cur_x_min+cell_size*cell_num-1, cur_y_min+cell_size*cell_num-1]./scale;

        cur_scale_confidences = scores(idx);
        cur_scale_image_ids = repmat( {test_scenes(k).name}, [num_y_detection*num_x_detection, 1] );
        cur_scale_mh_features_neg = s_window_features(idx, :);


        cur_bboxes = [cur_bboxes; cur_scale_bboxes];
        cur_confidences = [cur_confidences; cur_scale_confidences];
        cur_image_ids = [cur_image_ids; cur_scale_image_ids];
        cur_mh_features_neg = [cur_mh_features_neg; cur_scale_mh_features_neg];
        
    end

    % non_max_supr_bbox can actually get somewhat slow with thousands of
    % initial detections. You could pre-filter the detections by confidence,
    % e.g. a detection with confidence -1.1 will probably never be
    % meaningful. You probably _don't_ want to threshold at 0.0, though. You
    % can get higher recall with a lower threshold. You don't need to modify
    % anything in non_max_supr_bbox, but you can.
    if(mine_hard == false)
        [is_maximum] = non_max_supr_bbox(cur_bboxes, cur_confidences, size(img));

        cur_confidences = cur_confidences(is_maximum,:);
        cur_bboxes      = cur_bboxes(     is_maximum,:);
        cur_image_ids   = cur_image_ids(  is_maximum,:);
    end
 
    bboxes      = [bboxes;      cur_bboxes];
    confidences = [confidences; cur_confidences];
    image_ids   = [image_ids;   cur_image_ids];
    mh_features_neg = [mh_features_neg; cur_mh_features_neg];

end




