#include "Planet.h"
#include "DynamicArray.h"
//#include <crtdbg.h>
#include <assert.h>
#include <stdlib.h>
#include "Repository.h"
#include <stdio.h>
#include "Operation.h"

void destroyInt(int* i)
{
	if (i == NULL)
		return;
	free(i);
}

//void undo(DynamicArray* undoStack, Repository* repo)
//{
//	Operation* lastOp = get_element(undoStack, getSize(undoStack) - 1);
//	if (strcmp(getNameOp(lastOp), "add") == 0)
//		removeRepo(repo, getPlanet(lastOp));
//}

int main()
{
	Planet* p1 = createPlanet("Earth", "terrestrial", 0);
	Planet* p2 = createPlanet("Jupiter", "gas", 5);
	Planet* p3 = createPlanet("Saturn", "gas", 8);

	DynamicArray* undoStack = createDynamicArray(4, &DestructOp);

	Repository* repo = createRepo();
	Operation* op = createOp("add", p1, NULL);
	addElemToArray(undoStack, op);
	
	addRepo(repo,p1);
	addRepo(repo,p2);
	addRepo(repo,p3);
	
	DynamicArray* elems_array = get_all(repo);
	for (int i = 0; i < getSizeRepo(repo); i++)
		printf("%s\n", getName(get_element(elems_array, i)));
	destroyDynamicArray(elems_array);

	destroyRepo(repo);
	destroyDynamicArray(undoStack);

	///*DynamicArray* arrayOfInts = createDynamicArray(2, &destroyInt);
	//int* i1 = malloc(sizeof(int));
	//if (i1 != NULL)
	//	*i1 = 1;
	//int* i2 = malloc(sizeof(int));
	//if (i2 != NULL)
	//	*i2 = 2;
	//addElemToArray(arrayOfInts, i1);
	//addElemToArray(arrayOfInts, i2);

	//destroyDynamicArray(arrayOfInts);*/
	
	//_CrtDumpMemoryLeaks();
	return 0;
}