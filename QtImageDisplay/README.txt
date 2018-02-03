###############################################################################
#######						INTELLIGENT SCISSOR							#######
#######																	#######
###############################################################################


AUTHORS :
Manohar Kuse (mpkuse@ust.hk)
Sunil Prasad Jaiswal (spjaiswal@ust.hk)


WEB-PAGE :
http://kusemanohar.wordpress.com/2014/02/24/magnetic-lasso-from-scratch-intelligent-scissors/
http://www.youtube.com/watch?v=io0LRkLu2UQ


OBJECTIVE : 
The objective of this project is to understand the algorithm beneath the magnetic lasso tool. Magnetic lasso tool is one of the 
common tools found in tools like Gimp, Adobe Photoshop etc.  In this project we build a tool which allows a user to cut an object 
out of an image and paste it into another. The tool is intelligent in the sense that it snaps to the edges it detects in the underlying image.


GUI FEATURES
	File –> Open : Browse files and open image to mark
	File –> Write Contour : Write the delineated path as a PNG image
	File –> Write Mask : Write the marked mask as a PNG image
	File –> Quit : Quit the program
	
	Scissor –> Activate Scissor (check-able) : Activate/De-activate the intelligent scissor tool
	Scissor –> Reset Contour : Reset the delineated contour.
	Scissor –> Overlay Contour (check-able) : Activate/De-activate the display of delineated contours

	Image –> Zoom In : Zoom into the image
	Image –> Zoom Out : Zoom out of the image
	Image –> Blur : Blur image with 3×3 kernel*
	Image –> Blur : Blur image with 5×5 kernel*
	Image –> Gaussian Blur : Gaussian blur image*
	
	About –> Help : Display usage
	About –> Authors : Gives the names of the software’s creators
	About –> Qt : Gives the version of Qt
	
	
EXTRA CREDIT FEATURES
* => Blurring. Reduces noise, makes the image smooth, hence reduces the jitter in the contour


TECHNICAL INFO
The language of this software is C++. The entire software is built using the Qt-creator on Ubuntu 12.04. GUI uses the Qt libraries. 
Image processing is done using OpenCV 2.3. C++ STL libraries is used for the priority Queue. 
