#include "pixelnode.h"

pixelNode::pixelNode(){

}

pixelNode::pixelNode(int col, int row):FiibHeapNode() {

    this->col = col;
    this->row = row;
    this->totalCost = DBL_MAX;
    this->prevNode = NULL;

}

pixelNode::~pixelNode(){

}

void pixelNode::setLinkCost(int link, double value) {
    this->linkcost[link] = value;
}

void pixelNode::LinkCost(pixelNode *pn) {
    if(pn->getCol() == this->col + 1 && pn->getRow() == this->row)
        return linkCost[0];
    if(pn->getCol() == this->col + 1 && pn->getRow() == this->row - 1)
        return linkCost[1];
    if(pn->getCol() == this->col && pn->getRow() == this->row - 1)
        return linkCost[2];
    if(pn->getCol() == this->col - 1 && pn->getRow() == this->row-1)
        return linkCost[3];
    if(pn->getCol() == this->col - 1 && pn->getRow() == this->row)
        return linkCost[4];
    if(pn->getCol() == this->col - 1 && pn->getRow() == this->row + 1)
        return linkCost[5];
    if(pn->getCol() == this->col && pn->getRow() == this->row + 1)
        return linkCost[6];
    if(pn->getCol() == this->col + 1 &&pn->getRow() == this->row + 1)
        return linkCost[7];
    return -1.0;
}


void pixelNode::setParent(pixelNode* p) {
    this->prevNode = p;
    return;
}

void pixelNode::resetPrevNode() {
    this->prevNode = NULL;
    return;
}

void pixelNode::resetTotalCost(double x) {
    this->totalCost = x;
    return;
}

void pixelNode:: operator =(FibHeapNode& RHS) {
    pixelNode& pRHS=(pixelNode&)RHS;
    FHN_Assign(RHS);
    this->totalCost = pRHS.totalCost;
}

int  pixelNode:: operator ==(FibHeapNode& RHS) {
    pixelNode& pRHS=(pixelNode&)RHS;
    if (FHN_Cmp(RHS)) return 0;
    // Key compare goes here in derived classes
    if(RHS.NegInfinityFlag && NegInfinityFlag) return 1;
    return totalCost == pRHS.totalCost;
}
int  pixelNode::operator <(FibHeapNode& RHS) {
    int X;
    pixelNode& pRHS=(pixelNode&)RHS;
    if ((X=FHN_Cmp(RHS)) != 0)
        return X < 0 ? 1 : 0;
    // Key compare goes here in derived classes
    if(RHS.NegInfinityFlag && NegInfinityFlag)
        return 0;
    return totalCost < pRHS.totalCost;
}

//return column and row of neighbor in link direction
void pixelNode:: Neighbor(int link,int &c,int &r) {
    const int linktable[8][2]={
        {1,0}, // link 0
        {1,-1}, // link 1
        {0,-1}, // link 2
        {-1,-1}, // link 3
        {-1,0}, // link 4
        {-1,1}, // link 5
        {0,1}, // link 6
        {1,1} // link7
    };
    c = this->col + linktable[link][0];
    r = this->row + linktable[link][1];
}


// Access private data member
pixelNode* pixelNode::getParent(){
    return this->prevNode;
}

int pixelNode::getCol(){
    return this->col;
}

int pixelNode::getRow(){
    return this->row;
}

float pixelNode::getTotalCost(){
    return this->totalCost;
}

void pixelNode::getLinkCost(int link) {
    return this->linkcost[link];
}

