#pragma once

#include "SortedMultiMap.h"
#include <vector>

class SMMIterator {
    friend class SortedMultiMap;
private:
    const SortedMultiMap& map;
    SMMIterator(const SortedMultiMap& map);

    int* traversalList;
    int traversalSize;
    int traversalCapacity;
    int currentIndex;

    void inOrderTraversal(int node);
    void addToTraversalList(int node);

public:
    void first();
    void next();
    bool valid() const;
    TElem getCurrent() const;
    ~SMMIterator();
};


