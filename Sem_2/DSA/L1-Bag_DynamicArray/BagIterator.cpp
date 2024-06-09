#include <exception>
#include "BagIterator.h"
#include "Bag.h"

using namespace std;


BagIterator::BagIterator(const Bag& c): bag(c)
{
	this->current_element=0;
    this->current_frequency=1;
}
// BC=WC=TC: Theta(1)

void BagIterator::first() {
    this->current_element=0;
    this->current_frequency=1;
}
// BC=WC=TC: Theta(1)

void BagIterator::next() {
    if(this->valid()) {
        if (this->current_frequency < this->bag.elements[this->current_element].second) {
            this->current_frequency++;
        } else {
            this->current_element++;
            this->current_frequency = 1;
        }
    }
    else
    {
        throw exception();
    }
}
// BC=WC=TC: Theta(1)

bool BagIterator::valid() const {
	return (this->current_element<this->bag.bagIndex && this->current_frequency<=this->bag.elements[this->current_element].second);
}
// BC=WC=TC: Theta(1)


TElem BagIterator::getCurrent() const
{
    if(this->valid())
    {
        return this->bag.elements[this->current_element].first;
    }
    else
    {
        throw exception();
    }
}
// BC=WC=TC: Theta(1)