//
// Created by Mara Ielciu on 24.03.2024.
//
#include <cassert>
#include "TestDynamicArray.h"
#include "DynamicArray.h"

void TestDynamicArray::testDynamicArray1() {
    Tutorial tutorial1{"Tutorial1", "Tutorial1","www.tutorial1.com", 100, 10,10 };
    Tutorial tutorial2{"Tutorial2", "Tutorial2","www.tutorial2.com", 200, 20,20 };
    Tutorial tutorial3{"Tutorial3", "Tutorial3","www.tutorial3.com", 300, 30,30 };
    DynamicArray<Tutorial> arr(2);
    assert(arr.getSizeArray() == 0);
    assert(arr.getCapacityArray() == 2);
    arr.addElementArray(tutorial1);
    assert(arr.getSizeArray() == 1);
    assert(arr.getCapacityArray() == 2);
    arr.addElementArray(tutorial2);
    assert(arr.getSizeArray() == 2);
    assert(arr.getCapacityArray() == 2);
    arr.addElementArray(tutorial3);
    assert(arr.getSizeArray() == 3);
    assert(arr.getCapacityArray() == 4);
    arr.deleteElementArray(1);
    assert(arr.getSizeArray() == 2);
    assert(arr.getCapacityArray() == 4);
    arr.updateElementArray(0, tutorial3);
    assert(arr.getElementArray(0) == tutorial3);
    try{
        arr.deleteElementArray(10);
        assert(false);
    }
    catch(std::invalid_argument &e){
        assert(true);
    }
    try{
        arr.updateElementArray(10, tutorial3);
        assert(false);
    }
    catch(std::invalid_argument &e){
        assert(true);
    }
    try{
        arr.getElementArray(10);
        assert(false);
    }
    catch(std::invalid_argument &e){
        assert(true);
    }
    DynamicArray<Tutorial> arr2 = arr;
    assert(arr2.getSizeArray() == 2);
    assert(arr2.getCapacityArray() == 4);
    assert(arr2.getElementArray(0) == tutorial3);
    assert(arr2.getElementArray(1) == tutorial3);
    DynamicArray<Tutorial> arr3{arr};
    assert(arr3.getSizeArray() == 2);
    assert(arr3.getCapacityArray() == 4);
    assert(arr3.getElementArray(0) == tutorial3);
    assert(arr3.getElementArray(1) == tutorial3);
    Tutorial* tutorials = arr.getAllElementsArray();
    assert(tutorials[0] == tutorial3);
    assert(tutorials[1] == tutorial3);
    DynamicArray<Tutorial> sourceArray(2); // Smaller capacity to test resizing logic
    Tutorial t1("Title1", "Presenter1", "https://link1.com", 10, 1, 100);
    Tutorial t2("Title2", "Presenter2", "https://link2.com", 20, 2, 200);
    sourceArray.addElementArray(t1);
    sourceArray.addElementArray(t2);
    DynamicArray<Tutorial> targetArray(1);
    targetArray = sourceArray;
    assert(targetArray.getSizeArray() == 2);
    assert(targetArray.getElementArray(0).getTitle() == "Title1");
    assert(targetArray.getElementArray(1).getPresenter() == "Presenter2");
    sourceArray.addElementArray(Tutorial("Title3", "Presenter3", "https://link3.com", 30, 3, 300));
    assert(targetArray.getSizeArray() == 2);
    targetArray = targetArray;
    assert(targetArray.getSizeArray() == 2);
    assert(targetArray.returnPosition(t1) == 0);
    assert(targetArray.returnPosition(t2) == 1);
    assert(targetArray.returnPosition(tutorial1) == -1);

}
