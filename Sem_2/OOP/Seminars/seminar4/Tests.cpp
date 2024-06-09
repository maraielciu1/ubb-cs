#include "Tests.h"
#include "DynamicVector.h"
#include "LinkedList.h"
#include <assert.h>

void Tests::testList(List<int>& list)
{
	list.add(10);
	list.add(20);
	assert(list.getSize() == 2);
	assert(list[1] == 20);
	list[1] = 40;
	assert(list[1] == 40);
	list.add(30);
	assert(list.getSize() == 3);
}
