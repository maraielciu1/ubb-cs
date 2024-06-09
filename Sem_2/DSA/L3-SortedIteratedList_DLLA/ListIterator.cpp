#include "ListIterator.h"
#include "SortedIteratedList.h"
#include <exception>

using namespace std;

ListIterator::ListIterator(const SortedIteratedList& list) : list(list){
    current= list.head;
}
// BC=WC=Total=Theta(1)

void ListIterator::first(){
	current = list.head;
}
// BC=WC=Total=Theta(1)

void ListIterator::next(){
    if(current==-1)
        throw exception();
    current = list.nodes[current].next;
}
// BC=WC=Total=Theta(1)

bool ListIterator::valid() const{
    return current != -1;
}
// BC=WC=Total=Theta(1)

TComp ListIterator::getCurrent() const{
    if(current==-1)
        throw exception();
    return list.nodes[current].element;
}
// BC=WC=Total=Theta(1)


