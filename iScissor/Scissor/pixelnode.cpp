#include "pixelnode.h"

pixelNode::pixelNode()
{

}

pixelNode::pixelNode(int w, int h, int idx) {
    this->column = w;
    this->row = h;
    this->state = INITIAL;
    this->totalCost = 0;
    this->prevNode = nullptr;
    this->index = idx;
    this->linkCost = new double[8];
}

pixelNode::~pixelNode()
{
    delete prevNode;
    delete[] linkCost;
}

void pixelNode::setPrevNode(pixelNode& p) {
    this->prevNode = &p;
    return;
}
