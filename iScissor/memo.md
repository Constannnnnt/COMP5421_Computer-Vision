## Notes for Image Scissor

### Goal:
```
1. allows a user to cut an object out of one image and paste it into another
2.helps the user trace the object by providing a "live wire" that automatically snaps to and wraps around the object of interest
```

### Features:
* see the descriptions
* question: how to copy and paste the selected objects?

### Insights:
Click seed point: any pixel in the image => computes the a (shortest) path from the seed point to the mouse cursor that hugs the contours of the image as closly as possible.

This path is computed by converting the image into a graph where pixels correspond to nodes. Each node is connected to its 8 immediate neighbors. Each link has a cost relating to the derivative of the images across the link.

This path is computed by finding the minimum cost path in the graph, from the seed to the mouse position.

The path will follow the egdes in the image, as a sequence of links in the graph.


### Data Structure
Pixel Node:
```
struct Node{ 
    double linkCost[8]; // the costs of each link
    int state; // INITIAL, ACTIVE, or EXPANDED 
    double totalCost; // minimum total cost from this node to the seed node
    Node *prevNode; // predecessor along the minimum cost path from the seed to that node
    int column, row; //  position of the node in original image
    //other unrelated fields;
    }
    ```
For visualization and debugging purposes, you should convert a pixel node array into an image that displays the computed cost values in the user interface. The link costs shown are the average of the costs over the RGB channels, as described above (NOT the per-channel costs).

### Potential Extra Credits
1. Modify the interface and program to allow blurring the image by different amounts before computing link costs (level 1)
2. Try different costs functions, for example the method described in  Intelligent Scissors for Image Composition, and modify the user interface to allow the user to select different functions (level 1)
3. Implement path cooling, as described in Intelligent Scissors for Image Composition (level 1)
4. Implement dynamic training, as described in Intelligent Scissors for Image Composition(level 2)
5. Implement a seed snapping feature, where the seed is automatically moved to the closest edge (level 1)
6. Implement a live wire with sub-pixel precision (level 3)


### BUGS
1. can not the get the accurate position when clicking at the image when the image is not aligned correctly
2. will crash after closing the applications
