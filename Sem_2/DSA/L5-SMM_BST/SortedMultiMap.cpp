#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <iostream>
#include <vector>
#include <exception>
using namespace std;

SortedMultiMap::SortedMultiMap(Relation r) {
	relation=r;
    firstEmpty=0;
    capacity=20;
    mapSize=0;
    nodes=new BTSNode[capacity];
    for(int i=0;i<this->capacity-1;i++) {
        nodes[i].left=i+1;
        nodes[i].right=-1;
    }
    nodes[capacity-1].left=-1;
    nodes[capacity-1].right=-1;
    root=-1;
}
//BC=WC=Total=Theta(capacity)

void SortedMultiMap::add(TKey c, TValue v) {
	this->root=addRec(this->root,make_pair(c,v));
    this->mapSize++;
    if(this->mapSize>=capacity) {
        resize();
    }
}
//complexity of addRec

vector<TValue> SortedMultiMap::search(TKey c) const {
	int currentNode=this->root;
    while(currentNode!=-1&&nodes[currentNode].element.first!=c) {
        if(relation(c,nodes[currentNode].element.first)) {
            currentNode=nodes[currentNode].left;
        } else {
            currentNode=nodes[currentNode].right;
        }
    }
    vector<TValue> values;
    while(currentNode!=-1&&nodes[currentNode].element.first==c) {
        values.push_back(nodes[currentNode].element.second);
        currentNode=nodes[currentNode].left;
    }
    return values;
}
//BC=Theta(m) - m is the number of values that have the key c, when the searched key is in the root
//WC=Theta(mapSize) - it is the last key or the biggest and not in the map
//Total=O(mapSize)

bool SortedMultiMap::remove(TKey c, TValue v) {
	bool removed=false;
    this->root=removeRec(this->root,make_pair(c,v),removed);
    if(removed) {
        this->mapSize--;
    }
    return removed;
}
//complexity of removeRec


int SortedMultiMap::size() const {
	return mapSize;
}
//BC=WC=Total=Theta(1)

bool SortedMultiMap::isEmpty() const {
	return this->mapSize==0;
}
//BC=WC=Total=Theta(1)

SMMIterator SortedMultiMap::iterator() const {
	return SMMIterator(*this);
}
//BC=WC=Total=Theta(1)

SortedMultiMap::~SortedMultiMap() {
    delete[] nodes;
}
//BC=WC=Total=Theta(1)

void SortedMultiMap::resize() {
    int newCapacity=capacity*2;
    BTSNode* newNodes=new BTSNode[newCapacity];
    for(int i=0;i<capacity;i++) {
        newNodes[i]=nodes[i];
    }
    for(int i=capacity;i<newCapacity-1;i++) {
        newNodes[i].left=i+1;
        newNodes[i].right=-1;
    }
    newNodes[newCapacity-1].left=-1;
    newNodes[newCapacity-1].right=-1;
    delete[] nodes;
    nodes=newNodes;
    firstEmpty=capacity;
    capacity=newCapacity;
}
//BC=WC=Total=Theta(2*capacity)

int SortedMultiMap::allocate() {
    int newNode = this->firstEmpty;
    this->firstEmpty = this->nodes[firstEmpty].left;
    return newNode;
}
//BC=WC=Total=Theta(1)

void SortedMultiMap::deallocate(int node) {
    this->nodes[node].left = this->firstEmpty;
    this->nodes[node].right = -1;
    this->firstEmpty = node;
}
//BC=WC=Total=Theta(1)

int SortedMultiMap::createNode(TElem elem) {
    int newNode = this->allocate();
    this->nodes[newNode].element = elem;
    this->nodes[newNode].left = -1;
    this->nodes[newNode].right = -1;
    return newNode;
}
//BC=WC=Total=Theta(1)

int SortedMultiMap::addRec(int currentNode, TElem elem) {
    if(currentNode==-1) {
        int newNode=createNode(elem);
        return newNode;
    }
    if(relation(elem.first,nodes[currentNode].element.first)) {
        int leftChild=addRec(nodes[currentNode].left,elem);
        nodes[currentNode].left=leftChild;
    } else {
        int rightChild=addRec(nodes[currentNode].right,elem);
        nodes[currentNode].right=rightChild;
    }
    return currentNode;
}
//BC=Theta(1) - when the current node is the node where the element should be added
//WC=Theta(mapSize) - when the element is added in the last node
//Total=O(mapSize)


int SortedMultiMap::removeRec(int currentNode, TElem elem, bool &removed) {
    if (currentNode == -1) {
        return -1;
    }
    if (elem.first == nodes[currentNode].element.first) {
        if (elem.second == nodes[currentNode].element.second) {
            removed = true;
            if (nodes[currentNode].left == -1) {
                int rightChild = nodes[currentNode].right;
                deallocate(currentNode);
                return rightChild;
            } else if (nodes[currentNode].right == -1) {
                int leftChild = nodes[currentNode].left;
                deallocate(currentNode);
                return leftChild;
            } else {
                int minRightSubtree = getMinimum(nodes[currentNode].right);
                nodes[currentNode].element = nodes[minRightSubtree].element;
                nodes[currentNode].right = removeRec(nodes[currentNode].right, nodes[minRightSubtree].element, removed);
            }
        } else {
            nodes[currentNode].left = removeRec(nodes[currentNode].left, elem, removed);
        }
    } else if (relation(elem.first, nodes[currentNode].element.first)) {
        nodes[currentNode].left = removeRec(nodes[currentNode].left, elem, removed);
    } else {
        nodes[currentNode].right = removeRec(nodes[currentNode].right, elem, removed);
    }
    return currentNode;
}
//BC=Theta(1) - when the element is found and is first
//WC=Theta(mapSize) - when the element is not in the map
//Total=O(mapSize)


int SortedMultiMap::getMinimum(int currentNode) {
    while(nodes[currentNode].left!=-1) {
        currentNode=nodes[currentNode].left;
    }
    return currentNode;
}
//BC=Theta(1) - when the left child of the current node is -1
//WC=Theta(log_mapSize) if it is balanced, Theta(mapHeight) if it is not
//Total=O(log_mapSize) if it is balanced, O(mapHeight) if it is not

vector<TKey> SortedMultiMap::keySet() const {
    vector<TKey> keys;
    SMMIterator it = this->iterator();
    while(it.valid()) {
        TElem elem = it.getCurrent();
        if(keys.empty()||keys.back()!=elem.first) {
            keys.push_back(elem.first);
        }
        it.next();
    }
    return keys;
}
//BC=WC=Total=Theta(mapSize)

