#include <assert.h>
#include <exception>

#include "ListIterator.h"
#include "SortedIndexedList.h"

using namespace std;

bool relation1(TComp e1, TComp e2) {
	if (e1 <= e2) {
		return true;
	}
	else {
		return false;
	}
}

void testAll(){
	SortedIndexedList list = SortedIndexedList(relation1);
	assert(list.size() == 0);
	assert(list.isEmpty());
    list.add(1);

    //test for removeBetween(int start, int end)
    try {
        list.removeBetween(0, 1);
        assert(false);
    } catch (exception&) {
        assert(true);
    }
    try {
        list.removeBetween(-1, 1);
        assert(false);
    } catch (exception&) {
        assert(true);
    }
    try {
        list.removeBetween(1, 0);
        assert(false);
    } catch (exception&) {
        assert(true);
    }
    assert(list.size() == 1);
    assert(!list.isEmpty());
    assert(list.search(1) == 0);
    assert(list.search(2) == -1);
    assert(list.search(3) == -1);
    assert(!list.isEmpty());
    list.add(2);
    list.add(3);
    list.removeBetween(1, 2);
    assert(list.size() == 1);
    assert(list.search(1) == 0);
    assert(list.search(2) == -1);
    assert(list.search(3) == -1);


    ListIterator iterator = list.iterator();
    assert(iterator.valid());
    iterator.first();
    assert(iterator.getCurrent() == 1);
    iterator.next();
    assert(!iterator.valid());
    iterator.first();
    assert(iterator.valid());
    assert(list.search(1) == 0);
    assert(list.remove(0) == 1);
    assert(list.size() == 0);
    assert(list.isEmpty());

}

