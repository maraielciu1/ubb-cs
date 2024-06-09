#include <iostream>
#include <cassert>
#include "ShortTest.h"
#include "ExtendedTest.h"
#include "SortedMultiMap.h"

bool ascending(TKey a, TKey b) {
    return a <= b;
}

void testKeySet() {
    SortedMultiMap smm(ascending);
    smm.add(1, 100);
    smm.add(2, 200);
    smm.add(3, 300);
    smm.add(1, 101);
    smm.add(2, 201);
    smm.add(4, 400);

    vector<TKey> keys = smm.keySet();
    vector<TKey> expectedKeys = {1, 2, 3, 4};
    assert(keys.size() == expectedKeys.size());
    for (int i = 0; i < keys.size(); i++) {
        assert(keys[i] == expectedKeys[i]);
    }
    smm.add(4,500);
    keys = smm.keySet();
    assert(keys.size() == expectedKeys.size());
    for (int i = 0; i < keys.size(); i++) {
        assert(keys[i] == expectedKeys[i]);
    }
    smm.remove(1,100);
    keys = smm.keySet();
    assert(keys.size() == expectedKeys.size());
    for (int i = 0; i < keys.size(); i++) {
        assert(keys[i] == expectedKeys[i]);
    }
    smm.remove(1,101);
    keys = smm.keySet();
    expectedKeys={2,3,4};
    assert(keys.size() == expectedKeys.size());
    for (int i = 0; i < keys.size(); i++) {
        assert(keys[i] == expectedKeys[i]);
    }
    std::cout << "testKeySet passed!" << std::endl;
}

int main(){
    testAll();
	testAllExtended();
    testKeySet();

    std::cout<<"Finished SMM Tests!"<<std::endl;

}
