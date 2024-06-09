#pragma once

#include "Planet.h"

typedef void* TElem;
typedef void (*DestroyFunction)(TElem);

typedef struct
{
	int size, capacity;
	TElem* elems;
	DestroyFunction destroyOp;
} DynamicArray;

DynamicArray* createDynamicArray(int capacity, DestroyFunction op);
void destroyDynamicArray(DynamicArray* arr);
void addElemToArray(DynamicArray* arr, TElem elem);
int getSize(DynamicArray* arr);
DynamicArray* copyArray(DynamicArray* arr);
TElem get_element(DynamicArray* arr, int position);