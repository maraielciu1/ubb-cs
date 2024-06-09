#include "Bag.h"
#include "BagIterator.h"
#include <exception>
#include <iostream>
using namespace std;


Bag::Bag() {
	this->capacity=10;
    this->elements = new Pair[this->capacity];
    this->bagIndex=0;
    this->bagSize=0;
}
// BC=WC=TC: Theta(1)


void Bag::add(TElem elem) {
	int index=0;
    int found=0;
    while(found==0 && index<this->bagIndex)
    {
        if(this->elements[index].first==elem)
        {
            this->elements[index].second++;
            found=1;
            this->bagSize++;
        }
        index++;
    }
    if(found==0)
    {
        if(this->bagSize<this->capacity)
        {
            this->elements[this->bagIndex].first=elem;
            this->elements[this->bagIndex].second=1;
            this->bagIndex++;
            this->bagSize++;
        }
        else
        {
            Pair* newElements = new Pair[this->capacity*2];
            for(int i=0;i<this->bagIndex;i++)
            {
                newElements[i]=this->elements[i];
            }
            newElements[this->bagIndex].first=elem;
            newElements[this->bagIndex].second=1;
            this->bagIndex++;
            this->bagSize++;
            this->capacity*=2;
            delete[] this->elements;
            this->elements=newElements;
        }
    }
}
// BC: Theta(1) -> when the element is the first one, so we update the frequency
// WC: Theta(bagIndex) -> when the element is not in the bag, so we have to add it at the end or we have to resize the array
// TC: O(bagIndex)

bool Bag::remove(TElem elem) {
    int index=0;
    while(index<this->bagIndex)
    {
        if(this->elements[index].first==elem)
        {
            this->elements[index].second--;
            this->bagSize--;
            if(this->elements[index].second==0)
            {
                this->elements[index]=this->elements[this->bagIndex-1];
                this->bagIndex--;
            }
            return true;
        }
        index++;
    }
	return false; 
}
// BC: Theta(1) -> when the element is the first one
// WC: Theta(bagIndex) -> when the element is the last one or it is not in the bag
// TC: O(bagIndex)


bool Bag::search(TElem elem) const {
    int index=0;
    while(index<this->bagIndex) {
        if (this->elements[index].first == elem)
            return true;
        index++;
    }
	return false; 
}
// BC: Theta(1) -> when the element is the first one
// WC: Theta(bagIndex) -> when the element is the last one or it is not in the bag
// TC: O(bagIndex)

int Bag::nrOccurrences(TElem elem) const {
    int index=0;
    while(index<this->bagIndex)
    {
        if(this->elements[index].first==elem)
            return this->elements[index].second;
        index++;
    }
	return 0;
}
// BC: Theta(1) -> when the element is the first one
// WC: Theta(bagIndex) -> when the element is the last one or it is not in the bag
// TC: O(bagIndex)


int Bag::size() const {
	return this->bagSize;
}
// BC=WC=TC: Theta(1)


bool Bag::isEmpty() const {
	return this->bagSize==0;
}
// BC=WC=TC: Theta(1)

BagIterator Bag::iterator() const {
	return BagIterator(*this);
}
// BC=WC=TC: Theta(1)


Bag::~Bag() {
	delete[] this->elements;
}
// BC=WC=TC: Theta(1)

int Bag::removeOccurrences(int nr, TElem elem) {
    if(nr<0)
        throw std::exception();
    else{
        int occurrences=this->nrOccurrences(elem);
        if(occurrences==0)
            return 0;
        else if(occurrences-nr<=0)
        {
            if(occurrences==1)
            {
                this->remove(elem);
                return 1;
            }
            else
            {
                int aux=occurrences;
                this->setOccurrences(1,elem);
                this->remove(elem);
                return aux;
            }
        }
        else
        {
            this->setOccurrences(occurrences-nr,elem);
            return nr;
        }

    }
}
// BC: Theta(1) -> when the element is the first one and the nr of occ is greater than nr
// WC: Theta(bagIndex) -> when nr of occ is smaller or equal than nr
// TC: O(bagIndex)


