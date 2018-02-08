#include "pixelnode.h"


pixelNode::pixelNode(){

}

pixelNode::pixelNode(int x, int y, float total_cost) {

    this->x = x;
    this->y = y;
    this->totalCost = total_cost;
    this->parent = NULL;

}

pixelNode::~pixelNode(){
    delete parent;
}

void pixelNode::setParent(pixelNode* p) {
    this->parent = p;
    return;
}


// Access private data member
pixelNode* pixelNode::getParent(){
    return this->parent;
}

int pixelNode::getX(){
    return this->x;
}

int pixelNode::getY(){
    return this->y;
}

float pixelNode::getTotalCost(){
    return this->totalCost;
}





