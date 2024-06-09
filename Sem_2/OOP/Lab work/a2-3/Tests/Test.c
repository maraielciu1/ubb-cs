////
//// Created by Mara Ielciu on 17.03.2024.
////
//
//#include "Test.h"
//#include "Domain/DynamicArray.h"
//#include "Domain/Domain.h"
//#include <assert.h>
//#include <string.h>
//
//
//void testDynamicArray()
//{
//    DynamicArray* arr = createDynamicArray(2, &destroyMaterial);
//    assert(arr->capacity == 2);
//    assert(arr->length == 0);
//    assert(arr->elems != NULL);
//    assert(arr->destroyOp != NULL);
//    //destroyDynamicArray(arr);
//}
//
//void testMaterial()
//{
//    Date d = createDate(1, 2, 3);
//    Material* m = createMaterial("iron", "metal", 10,d);
//    assert(strcmp(getName(m), "iron") == 0);
//    assert(strcmp(getSupplier(m), "metal") == 0);
//    assert(getQuantity(m) == 10);
//
//    Material* m2 = copyMaterial(m);
//    assert(strcmp(getName(m2), "iron") == 0);
//    assert(strcmp(getSupplier(m2), "metal") == 0);
//    assert(getQuantity(m2) == 10);
//
//}
//
//void testRepository()
//{
//    Date d = createDate(1, 2, 3);
//    Material* m = createMaterial("iron", "metal", 10,d);
//    Repository* repo = createRepo();
//    assert(getSize(repo->arr) == 0);
//    assert(getSize(repo->undo) == 0);
//    assert(getSize(repo->redo) == 0);
//    addRepo(repo, m);
//    assert(getSize(repo->arr) == 1);
//    assert(getSize(repo->undo) == 1);
//    assert(getSize(repo->redo) == 0);
//
//    removeRepo(repo, m);
//    assert(getSize(repo->arr) == 0);
//    assert(getSize(repo->undo) == 2);
//    assert(getSize(repo->redo) == 0);
//
//    updateRepo(repo, m, m);
//    assert(getSize(repo->arr) == 0);
//
//    undo(repo);
//    assert(getSize(repo->arr) == 1);
//
//    redo(repo);
//    assert(getSize(repo->arr) == 0);
//
//}
//
//void testService()
//{
//    Date d = createDate(1, 2, 3);
//    Material* m = createMaterial("iron", "metal", 10,d);
//    Repository* repo = createRepo();
//    Service* s = createService(repo);
//    assert(get_size_service(s) == 0);
//    add(s, m);
//    assert(get_size_service(s) == 1);
//    undoS(s);
//    assert(get_size_service(s) == 0);
//    redoS(s);
//    assert(get_size_service(s) == 1);
//}
//
//void testAll()
//{
//    testDynamicArray();
//    testMaterial();
//    testRepository();
//    testService();
//}
//
//
//
//
