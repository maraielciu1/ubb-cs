//
// Created by Mara Ielciu on 15.03.2024.
//

#include "Service.h"
#include <stdlib.h>
#pragma once
#include "Repo/Repository.h"
#include <string.h>

// create Service
Service* createService(Repository* repo,UndoRedoRepo* undoRedo, int typeRepo)
{
    Service* s;
    s = (Service*)malloc(sizeof(Service));
    s->repo = repo;
    s->undoRedo = undoRedo;
    s->typeRepo=typeRepo;
    return s;
}

UndoRedoRepo *getUndoRedo(Service *s)
{
    return s->undoRedo;
}

// destroy Service
void destroyService(Service* s)
{
    destroyRepo(s->repo);
    destroyUndoRedoRepo(s->undoRedo);
    free(s);
}

Repository *getRepository(Service *s)
{
    return s->repo;
}

// add Material to Service
int addMaterialService(Service *service, Material *m)
{
    int index= checkIfMaterialIsInRepository(service->repo, m);
    if (index== -1)
    {
        // Because I have performed an operation, I shall delete redo repository (can't redo after performing an operation)
        if (service->typeRepo == 1)
        {
            resetRedoOperations(service->undoRedo);
            Operation *operation = createOperation(2, m, NULL);
            addUndoOperation(service->undoRedo, operation);
        }
        else
        {
            resetRedoDynamicArray(service->undoRedo);
            DynamicArray *dynamicArray = copyDynamicArrayOfMaterials(getDynamicArray(getRepository(service)));
            addUndoDynamicArray(service->undoRedo, dynamicArray);
        }
        addRepo(getRepository(service), m);
        return 1;
    }

    return 0;
}

// delete Material from Service
int deleteMaterialService(Service* s, Material *m)
{
    int index= checkIfMaterialIsInRepository(s->repo, m);
    if (index!= -1)
    {
        // Because I have performed an operation, I shall delete redo repository (can't redo after performing an operation)
        if (s->typeRepo == 1)
        {
            resetRedoOperations(s->undoRedo);
            Operation *operation = createOperation(1, m, NULL);
            addUndoOperation(s->undoRedo, operation);
        }
        else
        {
            resetRedoDynamicArray(s->undoRedo);
            DynamicArray *dynamicArray = copyDynamicArrayOfMaterials(getDynamicArray(getRepository(s)));
            addUndoDynamicArray(s->undoRedo, dynamicArray);
        }
        removeRepo(s->repo, index);
        return 1;
    }
    return 0;
}

// update Material from Service
int updateMaterialService(Service* s, Material *m1, Material* m2)
{
    int index= checkIfMaterialIsInRepository(s->repo, m1);
    if (index!= -1)
    {
        // Because I have performed an operation, I shall delete redo repository (can't redo after performing an operation)
        if (s->typeRepo == 1)
        {
            resetRedoOperations(s->undoRedo);
            Operation *operation = createOperation(3, m2, m1);
            addUndoOperation(s->undoRedo, operation);
        }
        else
        {
            resetRedoDynamicArray(s->undoRedo);
            DynamicArray *dynamicArray = copyDynamicArrayOfMaterials(getDynamicArray(getRepository(s)));
            addUndoDynamicArray(s->undoRedo, dynamicArray);
        }
        updateRepo(s->repo, index, m2);
        return 1;
    }
    return 0;
}

DynamicArray* pastExpirationDate(Service* s, char* str)
{
    DynamicArray* new_arr= createDynamicArray(10,&destroyMaterial);
    Repository *repo = getRepository(s);
    for(int i=0;i<getLengthOfDynamicArray(repo->arr);i++)
    {
        Material* m = copyMaterial(getMaterialByPosition(repo,i));
        if(past_date(getExpDate(m),getCurrentDate())==0)
            if(strstr(getName(m),str)||strlen(str)==0)
            {

                addElementToDynamicArray(new_arr,m);
            }
    }
    return new_arr;

}

// get Material from Service that have the string at the beg of the name and past the expiration date
DynamicArray* pastExpirationDateBg(Service* s, char* str)
{
    DynamicArray* new_arr= createDynamicArray(10,&destroyMaterial);
    Repository *repo = getRepository(s);
    for(int i=0;i<getLengthOfDynamicArray(repo->arr);i++)
    {
        Material* m = copyMaterial(getMaterialByPosition(repo,i));
        if(past_date(getExpDate(m),getCurrentDate())==0)
            if(getName(m)[0]==str[0])
            {

                addElementToDynamicArray(new_arr,m);
            }
    }
    return new_arr;

}

// sort ascending the Materials from Service by quantity
void sortMaterialsByQuantityA(DynamicArray* materials) {
    int i, j;
    for (i = 0; i < getLengthOfDynamicArray(materials) - 1; i++) {
        for (j = 0; j < getLengthOfDynamicArray(materials) - i - 1; j++) {
            Material* m1 = getElementOnPosition(materials, j);
            Material* m2 = getElementOnPosition(materials, j + 1);
            if (getQuantity(m1) > getQuantity(m2)) {
                // Swapping elements in the array
                Material* temp = materials->elements[j];
                materials->elements[j] = materials->elements[j + 1];
                materials->elements[j + 1] = temp;
            }
        }
    }
}

// sort descending the Materials from Service by quantity
void sortMaterialsByQuantityD(DynamicArray* materials)
{
    int i, j;
    for (i = 0; i < getLengthOfDynamicArray(materials) - 1; i++) {
        for (j = 0; j < getLengthOfDynamicArray(materials) - i - 1; j++) {
            Material* m1 = getElementOnPosition(materials, j);
            Material* m2 = getElementOnPosition(materials, j + 1);
            if (getQuantity(m1) < getQuantity(m2)) {
                // Swapping elements in the array
                Material* temp = materials->elements[j];
                materials->elements[j] = materials->elements[j + 1];
                materials->elements[j + 1] = temp;
            }
        }
    }
}

// get Material from Service that have the string as supplier and the quantity less than q
DynamicArray* getQforSupplier(Service* s, char* str, int q)
{
    DynamicArray* new_arr= createDynamicArray(10,&destroyMaterial);
    DynamicArray* arr = get_all_service(s);
    for(int i=0;i<getLengthOfDynamicArray(arr);i++)
    {
        Material* m = copyMaterial(getElementOnPosition(arr,i));
        if(strcmp(getSupplier(m),str)==0)
            if(getQuantity(m)<q)
            {
                addElementToDynamicArray(new_arr,m);
            }
    }
    return new_arr;
}

// return ascending the Materials from Service that have the string as supplier and the quantity less than q
DynamicArray* returnAscending(Service* s, char* str, int q)
{
    DynamicArray* arr = getQforSupplier(s,str,q);
    sortMaterialsByQuantityA(arr);
    return arr;
}

// return descending the Materials from Service that have the string as supplier and the quantity less than q
DynamicArray* returnDescending(Service* s, char* str, int q)
{
    DynamicArray* arr = getQforSupplier(s,str,q);
    sortMaterialsByQuantityD(arr);
    return arr;
}

int undoRedoService(Service *service, DynamicArray *undoFromThisArray, DynamicArray *redoToThisArray)
{
    if (service->typeRepo == 1)
    {
        if (getLengthOfDynamicArray(undoFromThisArray) == 0)
            return 0;
        //get last element from the undo stack
        Operation *operation = (Operation *)getElementOnPosition(undoFromThisArray, getLengthOfDynamicArray(undoFromThisArray) - 1);
        Operation *operationToBeAddedToRedo = NULL;

        if (get_type(operation) == 1)
        {
            addRepo(getRepository(service), copyMaterial(get_old_m(operation)));
            operationToBeAddedToRedo = createOperation(2, get_old_m(operation), NULL);
        }
        else if (get_type(operation) == 2)
        {
            removeRepo(getRepository(service), checkIfMaterialIsInRepository(getRepository(service), get_old_m(operation)));
            operationToBeAddedToRedo = createOperation(1, get_old_m(operation), NULL);
        }
        else if (get_type(operation) == 3)
        {
            updateRepo(getRepository(service), checkIfMaterialIsInRepository(getRepository(service), get_old_m(operation)), copyMaterial(get_new_m(operation)));
            operationToBeAddedToRedo = createOperation(3, get_new_m(operation), get_old_m(operation));
        }
        addElementToDynamicArray(redoToThisArray, operationToBeAddedToRedo);
        deleteElementFromDynamicArray(undoFromThisArray, getLengthOfDynamicArray(undoFromThisArray) - 1);
    }
    else if (service->typeRepo == 2)
    {
        if (getLengthOfDynamicArray(undoFromThisArray) == 0)
            return 0;
        DynamicArray *dynamicArray = (DynamicArray *)getElementOnPosition(undoFromThisArray, getLengthOfDynamicArray(undoFromThisArray) - 1);
        DynamicArray *dynamicArrayToBeAddedToRedo = copyDynamicArrayOfMaterials(getDynamicArray(getRepository(service)));
        addElementToDynamicArray(redoToThisArray, dynamicArrayToBeAddedToRedo);
        DynamicArray *aux;
        aux = getDynamicArray(getRepository(service));
        service->repo->arr = copyDynamicArrayOfMaterials(dynamicArray);
        destroyDynamicArray(aux);
        deleteElementFromDynamicArray(undoFromThisArray, getLengthOfDynamicArray(undoFromThisArray) - 1);
    }
    return 1;
}

DynamicArray* sortMaterials(Service* s, char* supplier, int quantity, SortFunction sortFunc)
{
    return sortFunc(s, supplier, quantity);
}

// get all Materials from Service
DynamicArray* get_all_service(Service* s)
{
    return get_all(s->repo);
}