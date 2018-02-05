#ifndef PIXELNODE_H
#define PIXELNODE_H

#include <stddef.h>

enum STATE {INITIAL, ACTIVE, EXPANDED};

class pixelNode
{
public:
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
    int getState();
    int getIndex();
    int getCol();
    int getRow();

private:
    double* linkCost;       // cost of neighbors
    STATE state;              // state in the dij algorithm
    double totalCost;       // total cost to the seed
    pixelNode* parent;      // previous node
    int column, row;        // pos in the image
    int index;              // the index in the linked list
};

#endif // PIXELNODE_H
