#include "pixelnode.h"


pixelNode::pixelNode(){

}

pixelNode::pixelNode(int row, int col, float total_cost) {

    this->row = row;
    this->column = col;
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

int pixelNode::getCol(){
    return this->column;
}

int pixelNode::getRow(){
    return this->row;
}

float pixelNode::getTotalCost(){
    return this->totalCost;
}





