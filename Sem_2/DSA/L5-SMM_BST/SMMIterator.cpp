#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <exception>

SMMIterator::SMMIterator(const SortedMultiMap& d) : map(d) {
    traversalCapacity = 10;  // Initial capacity
    traversalList = new int[traversalCapacity];
    traversalSize = 0;
    currentIndex = 0;

    if (map.root != -1) {
        inOrderTraversal(map.root);
    }
}
//BC=WC=Total=Theta(mapSize)

SMMIterator::~SMMIterator() {
    delete[] traversalList;
}
//BC=WC=Total=Theta(1)

void SMMIterator::inOrderTraversal(int node) {
    if (node == -1) return;
    inOrderTraversal(map.nodes[node].left);
    addToTraversalList(node);
    inOrderTraversal(map.nodes[node].right);
}
//BC=WC=Total=Theta(mapSize)

void SMMIterator::addToTraversalList(int node) {
    if (traversalSize == traversalCapacity) {
        traversalCapacity *= 2;
        int* newTraversalList = new int[traversalCapacity];
        for (int i = 0; i < traversalSize; i++) {
            newTraversalList[i] = traversalList[i];
        }
        delete[] traversalList;
        traversalList = newTraversalList;
    }
    traversalList[traversalSize++] = node;
}
//BC=WC=Total=Theta(traversalCap)

void SMMIterator::first() {
    currentIndex = 0;
}
//BC=WC=Total=Theta(1)

void SMMIterator::next() {
    if (!valid()) {
        throw std::exception();
    }
    currentIndex++;
}
//BC=WC=Total=Theta(1)

bool SMMIterator::valid() const {
    return currentIndex < traversalSize;
}
//BC=WC=Total=Theta(1)

TElem SMMIterator::getCurrent() const {
    if (!valid()) {
        throw std::exception();
    }
    return map.nodes[traversalList[currentIndex]].element;
}
//BC=WC=Total=Theta(1)

