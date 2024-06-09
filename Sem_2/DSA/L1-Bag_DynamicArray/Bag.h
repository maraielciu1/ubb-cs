#pragma once
#include <utility>
//DO NOT INCLUDE BAGITERATOR
typedef int TKey;
typedef int TValue;
typedef std::pair<TKey, TValue> Pair;

//DO NOT CHANGE THIS PART
#define NULL_TELEM -111111;
typedef int TElem;
class BagIterator; 
class Bag {

private:
	Pair* elements;
    int capacity;
    int bagIndex;
    int bagSize;
    void setOccurrences(int occ,TElem elem)
    {
        int index=0;
        while(index<this->bagIndex)
        {
            if(this->elements[index].first==elem)
            {
                this->bagSize=this->bagSize-(this->elements[index].second-occ);
                this->elements[index].second=occ;
                break;
            }
            index++;
        }
    }

	//DO NOT CHANGE THIS PART
	friend class BagIterator;
public:
	//constructor
	Bag();

	//adds an element to the bag
	void add(TElem e);

	//removes one occurrence of an element from a bag
	//returns true if an element was removed, false otherwise (if e was not part of the bag)
	bool remove(TElem e);

	//checks if an element appears is the bag
	bool search(TElem e) const;

	//returns the number of occurrences for an element in the bag
	int nrOccurrences(TElem e) const;

	//returns the number of elements from the bag
	int size() const;

	//returns an iterator for this bag
	BagIterator iterator() const;

	//checks if the bag is empty
	bool isEmpty() const;

	//destructor
	~Bag();

    //remove nr occurrences of elem. if the element appears less than nr times, all occurences are removed
    //returns the nr of times the element was removed
    //throws an exception if nr is negative
    int removeOccurrences(int nr, TElem elem);
};