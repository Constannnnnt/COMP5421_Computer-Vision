close all
clear
clc

%https://www.mathworks.com/matlabcentral/fileexchange/37004-suite-of-functions-to-perform-uniform-sampling-of-a-sphere
addpath('../lib/gco/matlab', ...
		'../lib/S2_Sampling_Suite/S2 Sampling Toolbox', ...
		'../lib/nearest_neighbour');

%GCO_UnitTest; % Get GCO lib initialized
dataPath = '../data/data02';
dataType = '/*.bmp';


% step 1: uniform resampling
[ResampledImage, L_o] = resampling(dataPath, dataType);


% step 2: find denominator image




% step 3: initial normal estimation




% step 4: refine normals by MRF graph cut




% step 5: contruct 3D models










