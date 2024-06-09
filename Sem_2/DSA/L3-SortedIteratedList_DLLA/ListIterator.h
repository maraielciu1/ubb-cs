#pragma once
#include "SortedIteratedList.h"

//DO NOT CHANGE THIS PART
class ListIterator{
	friend class SortedIteratedList;
private:
	const SortedIteratedList& list;
	explicit ListIterator(const SortedIteratedList& list);
    int current; // current position of the element in the list
public:
	void first();
	void next();
	bool valid() const;
    TComp getCurrent() const;
};


