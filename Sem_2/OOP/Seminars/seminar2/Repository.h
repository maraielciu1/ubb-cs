#pragma once
#include "DynamicArray.h"

typedef struct
{
	DynamicArray* arr;
} Repository;
Repository* createRepo();
void destroyRepo(Repository* repo);
void addRepo(Repository* repo, Planet* p);
DynamicArray* get_all(Repository* repo);
int getSizeRepo(Repository* r);