#include "SortedBag.h"
#include "SortedBagIterator.h"
#include <iostream>
#include "ShortTest.h"
#include "ExtendedTest.h"
#include <assert.h>

using namespace std;

int main() {
	testAll();
	testAllExtended();

	cout << "Test over" << endl;
}