#include "DynamicArray.h"
#include <stdlib.h>

DynamicArray* createDynamicArray(int capacity, DestroyFunction op)
{
	DynamicArray* arr = malloc(sizeof(DynamicArray));
	if (arr == NULL)
		return NULL;
	arr->capacity = capacity;
	arr->size = 0;
	arr->elems = malloc(sizeof(TElem) * capacity);
	if (arr->elems == NULL)
	{
		free(arr);
		return NULL;
	}
	arr->destroyOp = op;
	return arr;
}

void destroyDynamicArray(DynamicArray* arr)
{
	if (arr == NULL)
		return;
	for (int i = 0; i < arr->size; i++)
		arr->destroyOp(arr->elems[i]);
	free(arr->elems);
	free(arr);
}

void resize(DynamicArray* arr)
{
	if (arr == NULL)
		return;
	arr->capacity *= 2;
	TElem* aux = realloc(arr->elems, sizeof(TElem) * arr->capacity);
	if (aux == NULL)
		return;
	arr->elems = aux;
}

void addElemToArray(DynamicArray* arr, TElem elem)
{
	if (arr == NULL)
		return;
	if (arr->size == arr->capacity)
		resize(arr);
	arr->elems[arr->size++] = elem;
}

int getSize(DynamicArray* arr)
{
	if (arr == NULL)
		return 0;
	return arr->size;
}
DynamicArray* copyArray(DynamicArray* arr)
{
	DynamicArray* new_array = createDynamicArray(arr->capacity,arr->destroyOp);
	new_array->size = arr->size;
	for (int i = 0; i < arr->size; i++)
		new_array->elems[i] = copyPlanet(arr->elems[i]);
	return new_array;

}
TElem get_element(DynamicArray* arr, int position)
{
	return arr->elems[position];
}