#ifndef PIXELNODE_H
#define PIXELNODE_H

#include <stddef.h>

class pixelNode
{
public:

    pixelNode();
    pixelNode(int x, int y, float total_cost);
    ~pixelNode();
    void setParent(pixelNode*);

    // Access function
    pixelNode* getParent();
    float getTotalCost();
    int getX();
    int getY();

private:
    float totalCost;        // total cost to the seed
    pixelNode* parent;      // parent node
    int x, y;               // pos in the image

};

/*
class pixelNode
{
public:
    static cv::Mat img;

    pixelNode();
    pixelNode(int, int, int);
    ~pixelNode();
    void setParent(pixelNode*);
    void computeNeighborsCost();
    void setState(STATE);
    void computeTotalCost();

    // Access function
    pixelNode* getParent();
    double getTotalCost();
    STATE getState();
    int getIndex();
    int getCol();
    int getRow();

private:
    double* linkCost;       // cost of neighbors
    STATE state;            // state in the dij algorithm
    double totalCost;       // total cost to the seed
    pixelNode* parent;      // previous node
    int column, row;        // pos in the image
    int index;              // the index in the linked list

};
*/

#endif // PIXELNODE_H
