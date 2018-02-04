#ifndef PIXELNODE_H
#define PIXELNODE_H

enum {INITIAL, ACTIVE, EXPANDED};

class pixelNode
{
public:
    pixelNode();
    pixelNode(int, int, int);
    ~pixelNode();
    void setPrevNode(pixelNode&);

private:
    double* linkCost; // cost of neighbors
    int state; // state in the dij algorithm
    double totalCost; // total cost to the seed
    pixelNode* prevNode; // previous node
    int column, row; // pos in the image
    int index; // the index in the linked list
};

#endif // PIXELNODE_H
