#include "SortedBag.h"
#include "SortedBagIterator.h"
#include <cmath>
#include <exception>
#include <iostream>

int SortedBag::hCode(TComp k) {
    return abs(k);
}
//BC=WC=Total=Theta(1)

int SortedBag::h1(TComp k) const {
    return hCode(k) % this->capacity;
}
//BC=WC=Total=Theta(1)

int SortedBag::h2(TComp k) const {
    return 1 + (hCode(k) % (this->capacity - 1));
}
//BC=WC=Total=Theta(1)

int SortedBag::h(TComp k, int i) const {
    return (h1(k) + i * h2(k)) % this->capacity;
}
//BC=WC=Total=Theta(1)

bool SortedBag::prim(int x) {
    if (x < 2 || (x > 2 && x % 2 == 0))
        return false;
    for (int d = 3; d * d <= x; d = d + 2)
        if (x % d == 0)
            return false;
    return true;
}
//BC=Theta(1), WC=O(sqrt(x)), Total=O(sqrt(x))

int SortedBag::firstPrimeGreaterThan(int n) {
    n++;
    while (!prim(n))
        n++;
    return n;
}
//BC=Theta(1), WC=O(n*sqrt(n)), Total=O(n*sqrt(n))

SortedBag::SortedBag(Relation r) {
    this->relation = r;
    this->capacity = maxCapacity;
    this->length = 0;
    this->nrElements = 0;
    this->loadFactor = 0.7;
    this->empty = -1111;
    this->deleted = -1112;
    this->hashTable = new TElem[maxCapacity];
    this->frequency = new int[maxCapacity];
    for (int i = 0; i < maxCapacity; i++) {
        this->hashTable[i] = this->empty;
        this->frequency[i] = 0;
    }
}
//BC=WC=Total=Theta(n)

void SortedBag::add(TComp e) {
    if (float(this->length) > this->loadFactor*float(this->capacity)) {
        // resize and rehash
        int prime = firstPrimeGreaterThan(this->capacity*2);
        int oldCap = this->capacity;
        this->capacity = prime;
        TComp *oldTable = this->hashTable;
        int *oldFrequency = this->frequency;
        this->hashTable = new TElem[capacity];
        this->frequency = new int[capacity];
        this->length = 0;
        this->nrElements = 0;
        for (int j = 0; j < this->capacity; j++) {
            this->hashTable[j] = this->empty;
            this->frequency[j] = 0;
        }
        for (int j = 0; j < oldCap; j++) {
            if (oldTable[j] != this->empty && oldTable[j] != this->deleted) {
                for (int k = 0; k < oldFrequency[j]; k++) {
                    this->add(oldTable[j]);
                }
            }
        }
        delete[] oldTable;
        delete[] oldFrequency;
    }

    int i = 0;
    int pos = h(e, i);
    while (i < this->capacity && this->hashTable[pos] != this->empty && this->hashTable[pos] != e) {
        i++;
        pos = h(e, i);
    }
    if (this->hashTable[pos] == e) {
        this->frequency[pos]++;
    } else {
        this->hashTable[pos] = e;
        this->frequency[pos] = 1;
        this->length++;
    }
    this->nrElements++;
}
//BC=Theta(1) -> we don't need to resize and insert the element on the firts try
// WC=Theta(capacity) -> we need to resize and rehash
// Total=O(capacity)

bool SortedBag::remove(TComp e) {
    int i = 0;
    int pos = h(e, i);
    while (i < this->capacity && this->hashTable[pos] != e && this->hashTable[pos] != this->empty) {
        i++;
        pos = h(e, i);
    }
    if (i == this->capacity || this->hashTable[pos] == this->empty)
        return false;

    this->frequency[pos]--;
    if (this->frequency[pos] == 0) {
        this->hashTable[pos] = this->deleted;
        this->length--;
    }
    this->nrElements--;

    return true;
}
//BC=Theta(1) -> remove the element on the first try
// WC=Theta(capacity) -> remove the element when i=capacity-1 or don't find it
// Total=O(capacity)

bool SortedBag::search(TComp elem) const {
    int i = 0;
    int pos = h(elem, i);
    while (i < this->capacity && this->hashTable[pos] != elem && this->hashTable[pos] != this->empty) {
        i++;
        pos = h(elem, i);
    }
    return !(i == this->capacity || this->hashTable[pos] == this->empty);
}
//BC=Theta(1) -> find the element on the first try
// WC=Theta(capacity) -> find the element when i=capacity-1 or don't find it
// Total=O(capacity)

int SortedBag::nrOccurrences(TComp elem) const {
    int i = 0;
    int pos = h(elem, i);
    while (i < this->capacity && this->hashTable[pos] != this->empty) {
        if (this->hashTable[pos] == elem) {
            return this->frequency[pos];
        }
        i++;
        pos = h(elem, i);
    }
    return 0;
}
//BC=Theta(1) -> find the element on the first try
// WC=Theta(capacity) -> find the element when i=capacity-1 or don't find it
// Total=O(capacity)

int SortedBag::size() const {
    return this->nrElements;
}
//BC=WC=Total=Theta(1)

bool SortedBag::isEmpty() const {
    return this->nrElements == 0;
}
//BC=WC=Total=Theta(1)

SortedBagIterator SortedBag::iterator() const {
    return SortedBagIterator(*this);
}
//BC=WC=Total=Theta(capacity)

SortedBag::~SortedBag() {
    delete[] this->hashTable;
    delete[] this->frequency;
}

int SortedBag::distinctCount() const {
    return this->length;
}
//BC=WC=Total=Theta(1)