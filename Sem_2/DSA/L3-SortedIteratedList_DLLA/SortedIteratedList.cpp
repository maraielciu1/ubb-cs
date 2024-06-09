#include "ListIterator.h"
#include "SortedIteratedList.h"
#include <iostream>
using namespace std;
#include <exception>

SortedIteratedList::SortedIteratedList(Relation r){
    this->nodes = new DLLANode[10];
    this->cap=10;
    this->length=0;
    this->head=-1;
    this->tail=-1;
    this->relation=r;
    for (int i = 0; i < cap; i++) {
        this->nodes[i].next = i + 1;
        this->nodes[i].prev = -1;
    }
    this->nodes[this->cap-1].next = -1;
    this->nodes[this->cap-1].prev = -1;
    this->firstEmpty = 0;
}
// BC=WC=Total=Theta(capacity)

int SortedIteratedList::size() const {
    return length;
}
// BC=WC=Total=Theta(1)

bool SortedIteratedList::isEmpty() const {
    return length == 0;
}
// BC=WC=Total=Theta(1)

ListIterator SortedIteratedList::first() const {
    return ListIterator(*this);
}
// BC=WC=Total=Theta(1)

TComp SortedIteratedList::getElement(ListIterator poz) const {
    if (!poz.valid()||poz.current>=cap||poz.current<0) {
        throw exception();
    }
    return nodes[poz.current].element;
}
// BC=WC=Total=Theta(1)

TComp SortedIteratedList::remove(ListIterator& poz) {
    if (!poz.valid() || poz.current >= cap || poz.current < 0) {
        throw exception();
    }

    TComp e = nodes[poz.current].element;
    int nextIndex = nodes[poz.current].next;  // Get the next index before modification.
    if (poz.current == head) {
        head = nodes[head].next;
        if (head != -1) {
            nodes[head].prev = -1;
        }
    } else if (poz.current == tail) {
        tail = nodes[tail].prev;
        if (tail != -1) {
            nodes[tail].next = -1;
        }
    } else {
        nodes[nodes[poz.current].prev].next = nodes[poz.current].next;
        if (nodes[poz.current].next != -1) {  // Check if there is a next node
            nodes[nodes[poz.current].next].prev = nodes[poz.current].prev;
        }
    }

    nodes[poz.current].next = firstEmpty;
    nodes[poz.current].prev = -1;
    firstEmpty = poz.current;
    length--;
    if (nextIndex != -1) {
        poz.current = nextIndex;  // Move iterator to the next valid element.
    } else {
        poz.current = -1;  // There are no more elements, make the iterator invalid.
    }
    return e;  // Return the removed element.
}
// BC=WC=Total=Theta(1)

void SortedIteratedList::add(TComp e) {
    int index = allocate();
    if (index == -1) { // resize needed
        DLLANode* newNodes = new DLLANode[this->cap * 2];
        for (int i = 0; i < this->cap; i++) {
            newNodes[i].prev = this->nodes[i].prev;
            newNodes[i].element = this->nodes[i].element;
            newNodes[i].next = this->nodes[i].next;
        }
        for (int i = this->cap; i < this->cap * 2 - 1; i++) {
            newNodes[i].next = i + 1;
            newNodes[i].prev = - 1;
        }
        newNodes[this->cap * 2 - 1].next = -1;
        newNodes[this->cap * 2 - 1].prev = -1;
        delete[] this->nodes;
        this->nodes = newNodes;
        this->firstEmpty = this->cap;
        this->cap = this->cap * 2;
        index = this->allocate();
    }
    this->nodes[index].element = e;

    int current = this->head;
    int prev = -1;
    while (current != -1 && this->relation(this->nodes[current].element, e)) {
        prev = current;
        current = this->nodes[current].next;
    }

    if (prev == -1) {
        this->nodes[index].next = this->head;
        if (this->head != -1)
            this->nodes[this->head].prev = index;
        this->head = index;
        if (this->tail == -1)
            this->tail = index;
    } else {
        this->nodes[index].next = current;
        this->nodes[index].prev = prev;
        this->nodes[prev].next = index;
        if (current != -1)
            this->nodes[current].prev = index;
        else
            this->tail = index;
    }
    this->length++;

}
// BC=Theta(1) when the element becomes the first in the list and we don't need to resize
// WC=Theta(n) when the element is the last in the list or we need to resize
// Total=O(n)

ListIterator SortedIteratedList::search(TComp e) const {
    ListIterator it(*this);
    while (it.valid()) {
        if (it.getCurrent() == e)
            return it;
        it.next();
    }
    return it;

}
// BC=Theta(1) when the element is the first in the list
// WC=Theta(n) when the element is the last in the list or it is not in the list
// Total=O(n)


SortedIteratedList::~SortedIteratedList() {
    delete[] nodes;
}
// BC=WC=Total=Theta(1)

int SortedIteratedList::allocate() {
    int newElem = this->firstEmpty;
    if (newElem != -1) {
        this->firstEmpty = this->nodes[this->firstEmpty].next;
        if (this->firstEmpty != -1)
            this->nodes[this->firstEmpty].prev = -1;
        this->nodes[newElem].next = -1;
        this->nodes[newElem].prev = -1;
    }
    return newElem;
}
// BC=WC=Total=Theta(1)

