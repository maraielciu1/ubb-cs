#include "Repository.h"
#include <stdlib.h>
Repository* createRepo()
{
	Repository* repo;
	repo = (Repository*)malloc(sizeof(Repository));
	repo->arr = createDynamicArray(10,&destroyPlanet);
	return repo;
}
void destroyRepo(Repository* repo)
{
	if (getSize(repo->arr) == 0)
		return;
	
	destroyDynamicArray(repo->arr);
	free(repo);
}
void addRepo(Repository* repo, Planet* p)
{
	addElemToArray(repo->arr, p);
	
}
DynamicArray* get_all(Repository* repo)
{
	DynamicArray* new_arr;
	new_arr = copyArray(repo->arr);
	return new_arr;
	
}
int getSizeRepo(Repository* r)
{
	return getSize(r->arr);
}