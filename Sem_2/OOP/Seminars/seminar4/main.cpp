#include "DynamicVector.h"
#include "LinkedList.h"
#include "Tests.h"
#include <assert.h>

int main()
{
	DynamicVector<int> v{};
	Tests::testList(v);

	LinkedList<int> l{};
	Tests::testList(l);

//    List<int>* l1= nullptr;
//    if("sth")
//        l = new DynamicVector<int>{};
//    else
//        l = new LinkedList<int>{};


	return 0;
}