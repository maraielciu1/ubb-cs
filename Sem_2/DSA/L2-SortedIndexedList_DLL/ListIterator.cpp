#include "ListIterator.h"
#include "SortedIndexedList.h"
#include <iostream>

using namespace std;

ListIterator::ListIterator(const SortedIndexedList& list) : list(list) {
    this->position=0;
    this->current= reinterpret_cast<Node *>(this->list.head);
}
// BC=WC=Total=Theta(1)

void ListIterator::first(){
	this->position=0;
    this->current= reinterpret_cast<Node *>(this->list.head);
}
// BC=WC=Total=Theta(1)

void ListIterator::next(){
	if(this->valid()){
        this->position++;
        this->current=this->current->next;
    }
    else{
        throw exception();
    }
}
// BC=WC=Total=Theta(1)

bool ListIterator::valid() const{
	return this->position<this->list.size()&&this->current!= nullptr;
}
// BC=WC=Total=Theta(1)


TComp ListIterator::getCurrent() const{
	//return this->list.getElement(this->position);
    if(this->valid())
        return this->current->info;
    else
        throw exception();
}
// BC=WC=Total=Theta(1)


