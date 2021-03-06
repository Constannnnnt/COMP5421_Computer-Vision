# General Desciption

This repo contains four projects for COMP5421 - Computer Vision@HKUST. And each of these four projects involves different knowledge background about computer vision.

* You may need UST CSE account to access some webpages.

## Image scissor

### General Idea

The general idea of this project is that after loading the image, we finished the cost computation by implementing the cost function provided in the [course webpage](https://course.cse.ust.hk/comp5421/Password_Only/projects/iscissor/index.html)*. Every time when the user clicks a seed, we will update the path tree according to the Dijkstra's algorithm.

### Artifacts

#### Original Images and Masks

We provide three images, one background image and two foreground images.

Foreground Image One and its mask:

<img src = "./iScissor/artifact/girl.jpg" width = "50%" height = "50%"><img src = "./iScissor/artifact/girl_mask.png" width = "50%" height = "50%">

Foreground Image Two and its mask:

<img src = "./iScissor/artifact/sf.jpg" width = "50%" height = "50%"><img src = "./iScissor/artifact/sf_mask.png" width = "50%" height = "50%">

Background image:

<img src = "./iScissor/artifact/hkust.jpg" width = "50%" height = "50%">

#### Results

The result is cool!!!

<img src = "./iScissor/artifact/artifact.jpg" width = "50%" height = "50%">


### Bonus

We implemented two ```whistles``` for this project.

- SnapSeed

To snap the first seed near the edge, we find the nearest neighbor of the seed on the edge after the user clicks the first seed. We used ```CV::Canny``` to get the edge and then use ```L2 distance``` to meansure the space similarity. Update the position of the first seed each time when finding out a optimal point.

- Blurring

We include two gaussiFor blurring effect, and we use opencv Guassian filter. We adjust the Guassian filter kernel size and standard deviation to achieve different level of blurring effect. After opening the image, we could add this blurring effect using the interface of our IScissor.
	```Tool -> Gaussian 3*3```
    ```Tool -> Gaussian 5*5```
After click on these two buttons, the image will be blurred. We provide a sample with our school HKUST. You can see that different level of blurring effect has been achieved. Comparing two filters, we could see that larger kernel size induces stronger blurring effect. See pics.an blurring filters in the projects such that the user could apply anyone of them to compare the difference before and after blurring. ***Also we notice that if the blurring effect gets stronger, the cost gets lower.***

- Orignal Pic and its cost graph:

<img src = "./iScissor/blur/school.jpg" width = "50%" height = "50%"><img src = "./iScissor/blur/school_costgraph.png" width = "50%" height = "50%">

- Blurring with 3*3 gaussian filters

<img src = "./iScissor/blur/school_blur3.png" width = "50%" height = "50%"><img src = "./iScissor/blur/school_blur3_costgraph.png" width = "50%" height = "50%">

- Blurring with 5*5 gaussian filters


<img src = "./iScissor/blur/school_blur5.png" width = "50%" height = "50%"><img src = "./iScissor/blur/school_blur5_costgraph.png" width = "50%" height = "50%">

## Face detection
The course project of COMP5421 Computer Vision

### General Idea
The task given by this assignment is to implement a multi-scale sliding window face detector based on concepts presented in Dalal-Triggs 2005 and Viola-Jones 2001. The algorithm will be evaluated using a common benchmark for face detection (Caltech).

### Methodology

1. Use HOG descriptor to genearte positive face images with cell size 3
2. Horizontally flip face images, contrast original face images with darker faces images( img*0.8 ), use HOG descriptor to generate postive images from these images and add these images into the postive face samples
3. Use HOG descriptor to generate negative cropped images (50000 images) from the database
4. Train the linear SVM Classifier
5. For each test image, for each position at each scale in the image, create a window and run the classifier to determine whether or not there is a face at that location
6. Step 5 will result in many overlapping bounding boxes for the the same face, which must then be combined or suppressed into one final bounding box (non maximum suppression).

### Experiments and Results
We conducted several experiments to indentify the best approach to detect the face.

1. Cell Size 6, flip and contrast the faces, without hard negative mining

<img src = "./Face Detection/img/01_1.png" width = "33%" height = "33%"><img src = "./Face Detection/img/01_2.png" width = "33%" height = "33%"><img src = "./Face Detection/img/01_3.png" width = "33%" height = "33%">
++**The average accuracy is 0.840, and it is very quick.**++

2. Cell Size 3, flip and contrast the faces, without hard negative mining

<img src = "./Face Detection/img/02_1.png" width = "33%" height = "33%"><img src = "./Face Detection/img/02_2.png" width = "33%" height = "33%"><img src = "./Face Detection/img/02_3.png" width = "33%" height = "33%">
++**The average accuracy is 0.916, and it takes about 20 minutes.**++

3. Cell Size 3, without hard negative mining

<img src = "./Face Detection/img/03_1.png" width = "33%" height = "33%"><img src = "./Face Detection/img/03_2.png" width = "33%" height = "33%"><img src = "./Face Detection/img/03_3.png" width = "33%" height = "33%">
++**The average accuracy is 0.823, and it takes about 30-40 minutes.**++

4. Cell Size 3, flip and contrast the faces, with hard negative mining

<img src = "./Face Detection/img/04_1.png" width = "33%" height = "33%"><img src = "./Face Detection/img/04_2.png" width = "33%" height = "33%"><img src = "./Face Detection/img/04_3.png" width = "33%" height = "33%">
++**The average accuracy is 0.901, and it takes about 90 minutes.**++

### Discussion

1. Notice that when the cell size gets smaller, the HOG descriptor can persent more details about the gradient and edge direction at each pixel, which leads to higher accuracy.

2. With more negative samples, the accuracy can be improved a bit, but at the same time, the detection speed becomes slow.

3. When we horizontally flip face images and make images darker (i.e. add more positive training data), we can largley improve the accuracy since we found that some faces in very dark background could not be detected and some side faces can not be recognized.

### Bonus

#### Hard Negative Mining.
Train on the original datasets, collect images which are falsely detected as faces and add them into the negative samples. Retrain again.

#### Find and utilize alternative positive training data.
1. Horizontally Flip face images
2. Generate contrast images (image * 0.8)

#### Implement an interesting feature
Implement Local Binary Pattern (LBP). Instead of using HOG descriptor, we use LBP to extract features from images.


## Single view metrology

### Methodology
1. calculate the vanishing points by **Bob collins' algorithm**.
2. Calculate the Projective matrix, where the scales are computed with reference points and vanishing points
3. Use Homograph Matrix to get the texture map (for simplicity, we directly "hardcode" points this time)
4. Mark Interesing points and get 3D coordinates
<img src = "./SingleViewModeling/img/heightCal.png" width = "100%" height = "100%">

5. Generate VMRL


### Results

Source Image

<img src = "./SingleViewModeling/img/SVM1.jpg" width = "50%" height = "100%">

Texture Maps

<img src = "./SingleViewModeling/SingleViewModel/result/sony/xy_patch.jpg" width = "33%" height = "100%"><img src = "./SingleViewModeling/SingleViewModel/result/sony/yz_patch.jpg" width = "33%" height = "100%"><img src = "./SingleViewModeling/SingleViewModel/result/sony/xz_patch.jpg" width = "33%" height = "100%">

Results

<img src = "./SingleViewModeling/img/1.png" width = "50%" height = "100%">

Source Image

<img src = "./SingleViewModeling/img/SVM2.jpg" width = "50%" height = "100%">

Texture Maps

<img src = "./SingleViewModeling/SingleViewModel/result/jixin'sbox/xy_patch.jpg" width = "33%" height = "100%"><img src = "./SingleViewModeling/SingleViewModel/result/jixin'sbox/yz_patch.jpg" width = "33%" height = "100%"><img src = "./SingleViewModeling/SingleViewModel/result/jixin'sbox/xz_patch.jpg" width = "33%" height = "100%">

Results

<img src = "./SingleViewModeling/img/3.png" width = "50%" height = "100%">

Source Image

<img src = "./SingleViewModeling/img/rubik.jpg" width = "50%" height = "100%">

Texture Maps

<img src = "./SingleViewModeling/SingleViewModel/result/rubik/corner1.bmp" width = "33%" height = "100%"><img src = "./SingleViewModeling/SingleViewModel/result/rubik/corner2.bmp" width = "33%" height = "100%"><img src = "./SingleViewModeling/SingleViewModel/result/rubik/corner3.bmp" width = "33%" height = "100%">
<img src = "./SingleViewModeling/SingleViewModel/result/rubik/face_corner.bmp" width = "33%" height = "100%"><img src = "./SingleViewModeling/SingleViewModel/result/rubik/left_top.png" width = "33%" height = "100%"><img src = "./SingleViewModeling/SingleViewModel/result/rubik/face_mid.bmp" width = "33%" height = "100%">


Results

<img src = "./SingleViewModeling/img/2.png" width = "50%" height = "100%">

Source Image

<img src = "./SingleViewModeling/SingleViewModel/result/painting/painting.jpg" width = "50%" height = "100%">

Texture Maps

<img src = "./SingleViewModeling/SingleViewModel/result/painting/deskleftside.jpg" width = "33%" height = "100%"><img src = "./SingleViewModeling/SingleViewModel/result/painting/rightleg.jpg" width = "33%" height = "100%"><img src = "./SingleViewModeling/SingleViewModel/result/painting/leftlegfront.jpg" width = "33%" height = "100%">
<img src = "./SingleViewModeling/SingleViewModel/result/painting/deskrightslant.jpg" width = "33%" height = "100%"><img src = "./SingleViewModeling/SingleViewModel/result/painting/deskrightside.jpg" width = "33%" height = "100%"><img src = "./SingleViewModeling/SingleViewModel/result/painting/Hxy_image.jpg" width = "33%" height = "100%">


Results

<img src = "./SingleViewModeling/img/4.png" width = "50%" height = "100%">


## Multiple view modeling
In this part, we include five examples to demonstrate the multiple view modeling.

example 02

<img src = "./Dense-Photometric-Stereo/img/02/1.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/02/3.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/02/2.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/02/4.png" width = "50%" height = "100%">

example 03

<img src = "./Dense-Photometric-Stereo/img/03/1.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/03/2.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/03/3.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/03/4.png" width = "50%" height = "100%">

example 04

<img src = "./Dense-Photometric-Stereo/img/04/1.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/04/2.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/04/3.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/04/4.png" width = "50%" height = "100%">

example 05

<img src = "./Dense-Photometric-Stereo/img/05/1.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/05/2.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/05/3.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/05/4.png" width = "50%" height = "100%">

example 06

<img src = "./Dense-Photometric-Stereo/img/06/1.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/06/2.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/06/3.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/06/4.png" width = "50%" height = "100%">

example 07

<img src = "./Dense-Photometric-Stereo/img/07/1.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/07/2.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/07/3.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/07/4.png" width = "50%" height = "100%">

example 08

<img src = "./Dense-Photometric-Stereo/img/08/1.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/08/2.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/08/3.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/08/4.png" width = "50%" height = "100%">

example 09

<img src = "./Dense-Photometric-Stereo/img/09/1.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/09/2.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/09/3.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/09/4.png" width = "50%" height = "100%">

example 10

<img src = "./Dense-Photometric-Stereo/img/10/1.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/10/2.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/10/3.png" width = "50%" height = "100%"><img src = "./Dense-Photometric-Stereo/img/10/4.png" width = "50%" height = "100%">
