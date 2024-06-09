#include "FixedCapBiMap.h"
#include "FixedCapBiMapIterator.h"

FixedCapBiMap::FixedCapBiMap(int capacity) {
	if(capacity<=0)
        throw exception();
    this->capacity = capacity;
    this->mapSize = 0;
    this->elements = new TElem[this->capacity];

}
// BC=WC=TC: Theta(1)

FixedCapBiMap::~FixedCapBiMap() {
	delete[] this->elements;
}
// BC=WC=TC: Theta(1)

bool FixedCapBiMap::add(TKey c, TValue v){
	if(isFull())
        throw exception();
    int count=0;
    int index=0;
    while(count<2 && index<this->mapSize)
    {
        if(this->elements[index].first==c)
            count++;
        index++;
    }
    if(count == 2)
        return false;
    this->elements[this->mapSize].first=c;
    this->elements[this->mapSize].second=v;
    this->mapSize++;
	return true;
}
// BC: Theta(1)-when the first 2 keys from the map = c
// WC: Theta(mapSize)-when the key c appears once or 0 times in the map
// TC: O(mapSize)

ValuePair FixedCapBiMap::search(TKey c) const{
    ValuePair result;
    result.first=NULL_TVALUE;
    result.second=NULL_TVALUE;
    int nrfound=0;
    int index=0;
    while(nrfound<2 && index<this->mapSize)
    {
        if(this->elements[index].first==c) {
            if (nrfound == 0) {
                result.first = this->elements[index].second;
                nrfound++;
            } else {
                result.second = this->elements[index].second;
                nrfound++;
            }
        }
        index++;
    }
	return result;
}
// BC: Theta(1)-when the first 2 keys from the map = c
// WC: Theta(mapSize)-when the key c appears once or 0 times in the map
// TC: O(mapSize)

bool FixedCapBiMap::remove(TKey c, TValue v){
	int index=0;
    while(index<this->mapSize)
    {
        if(this->elements[index].first==c && this->elements[index].second==v)
        {
            this->elements[index]=this->elements[this->mapSize-1];
            this->mapSize--;
            return true;
        }
        index++;
    }
	return false;
}
// BC: Theta(1)-when the first key is c with the value v
// WC: Theta(mapSize)-when the pair is not in the map or on last position
// TC: O(mapSize)


int FixedCapBiMap::size() const {
	return this->mapSize;
}
// BC=WC=TC: Theta(1)

bool FixedCapBiMap::isEmpty() const{
	return this->mapSize == 0;
}
// BC=WC=TC: Theta(1)

bool FixedCapBiMap::isFull() const {
	return this->mapSize==this->capacity;
}
// BC=WC=TC: Theta(1)

bool FixedCapBiMap::replace(TKey key, TValue old_value, TValue new_value)
{
    int index=0;
    while(index<this->mapSize)
    {
        if(this->elements[index].first==key&&this->elements[index].second==old_value)
        {
            this->elements[index].second=new_value;
            return true;
        }
        index++;
    }
    return false;
}
// BC: Theta(1) -> when the pair is the first pair of the array
// WC: Theta(mapSize) -> when the pair is on the last position or is not in the map
// TC: O(mapSize)

FixedCapBiMapIterator FixedCapBiMap::iterator() const {
	return FixedCapBiMapIterator(*this);
}
// BC=WC=TC: Theta(1)



