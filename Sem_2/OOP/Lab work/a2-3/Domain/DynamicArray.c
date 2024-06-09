//
// Created by Mara Ielciu on 15.03.2024.
//

#include "DynamicArray.h"

#include <stdlib.h>

// create DynamicArray
DynamicArray *createDynamicArray(int capacity, DestroyElementFunctionPointer destroyElementFunctionPointer)
{
    DynamicArray *dynamicArray = (DynamicArray *)malloc(sizeof(DynamicArray));
    dynamicArray->elements = (TElem *)malloc(capacity * sizeof(TElem));
    dynamicArray->length = 0;
    dynamicArray->capacity = capacity;
    dynamicArray->destroyElementFunctionPointer = destroyElementFunctionPointer;

    return dynamicArray;
}

// Resizes the capacity of the dynamic array.
void resizeDynamicArray(DynamicArray *dynamicArray)
{
    dynamicArray->capacity *= 2;
    dynamicArray->elements = (TElem *)realloc(dynamicArray->elements, dynamicArray->capacity * sizeof(TElem));
}

// Retrieves the length (number of elements) of the dynamic array.
int getLengthOfDynamicArray(DynamicArray *dynamicArray)
{
    return dynamicArray->length;
}
// Retrieves the capacity of the dynamic array.
int getCapacityOfDynamicArray(DynamicArray *dynamicArray)
{
    return dynamicArray->capacity;
}

// Retrieves the elements array of the dynamic array.
TElem *getElements(DynamicArray *dynamicArray)
{
    return dynamicArray->elements;
}
// Retrieves the destroy element function pointer of the dynamic array.
DestroyElementFunctionPointer getDestroyElementFunctionPointer(DynamicArray *dynamicArray)
{
    return dynamicArray->destroyElementFunctionPointer;
}

// Sets the length (number of elements) of the dynamic array.
void setLengthOfDynamicArray(DynamicArray *dynamicArray, int length)
{
    dynamicArray->length = length;
}
// Retrieves the element at the specified position in the dynamic array.
TElem *getElementOnPosition(DynamicArray *dynamicArray, int position)
{
    if (position < 0 || position >= getLengthOfDynamicArray(dynamicArray))
        return NULL;
    TElem *elements = getElements(dynamicArray);
    return elements[position];
}

// Destroys the dynamic array and its elements.
void destroyDynamicArray(DynamicArray *dynamicArray)
{
    for (int i = 0; i < getLengthOfDynamicArray(dynamicArray); i++)
    {
        if (dynamicArray->elements[i])
            dynamicArray->destroyElementFunctionPointer(dynamicArray->elements[i]);
    }
    free(dynamicArray->elements);
    free(dynamicArray);
}

// Updates the element at the specified position in the dynamic array.
void updateElementOnPosition(DynamicArray *dynamicArray, int position, TElem element)
{
    if (position < 0 || position >= getLengthOfDynamicArray(dynamicArray))
        return;
    DestroyElementFunctionPointer destroyElementFunctionPointer = getDestroyElementFunctionPointer(dynamicArray);
    TElem *elements = getElements(dynamicArray);
    destroyElementFunctionPointer(elements[position]);
    elements[position] = element;
}
// Swaps the elements at the specified positions in the dynamic array.
void swapElementsOnDynamicArray(DynamicArray *dynamicArray, int position1, int position2)
{
    int length = getLengthOfDynamicArray(dynamicArray);
    TElem *elements = getElements(dynamicArray);
    if (position1 < 0 || position1 >= length || position2 < 0 || position2 >= length)
        return;

    TElem aux = elements[position1];
    elements[position1] = elements[position2];
    elements[position2] = aux;
}
// Adds an element to the end of the dynamic array.
void addElementToDynamicArray(DynamicArray *dynamicArray, TElem element)
{
    int length = getLengthOfDynamicArray(dynamicArray);
    int capacity = getCapacityOfDynamicArray(dynamicArray);
    TElem *elements = getElements(dynamicArray);

    if (length == capacity)
        resizeDynamicArray(dynamicArray);

    elements[length] = element;
    dynamicArray->length += 1;
}

// Deletes the element at the specified position from the dynamic array.
void deleteElementFromDynamicArray(DynamicArray *dynamicArray, int position)
{
    DestroyElementFunctionPointer destroyElementFunctionPointer = getDestroyElementFunctionPointer(dynamicArray);
    TElem *elements = getElements(dynamicArray);
    int length = getLengthOfDynamicArray(dynamicArray);
    if (position < 0 || position >= length)
        return;

    destroyElementFunctionPointer(elements[position]);
    for (int i = position; i < length - 1; ++i)
        elements[i] = elements[i + 1];

    elements[length - 1] = NULL;
    setLengthOfDynamicArray(dynamicArray, length - 1);
}

// Creates a copy of the dynamic array
DynamicArray *copyDynamicArrayOfMaterials(DynamicArray *dynamicArray)
{
    int length = getLengthOfDynamicArray(dynamicArray);
    DynamicArray *copy = createDynamicArray(length, destroyMaterial);
    for (int i = 0; i < length; i++)
    {
        TElem *element = getElementOnPosition(dynamicArray, i);
        addElementToDynamicArray(copy, copyMaterial((Material *)element));
    }
    return copy;
}