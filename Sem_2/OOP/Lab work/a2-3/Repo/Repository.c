//
// Created by Mara Ielciu on 15.03.2024.
//

#include "Repository.h"
#include "Domain/DynamicArray.h"
#include <stdlib.h>
#include "UndoRedo/Operation.h"
#include "Domain/Domain.h"
#include <string.h>

// create Repository
Repository* createRepo()
{
    Repository* repo;
    repo = (Repository*)malloc(sizeof(Repository));
    repo->arr = createDynamicArray(10,&destroyMaterial);
    return repo;
}

// destroy Repository
void destroyRepo(Repository* repo)
{
    if(getLengthOfDynamicArray(repo->arr)==0)
        return;
    destroyDynamicArray(repo->arr);
    free(repo);
}

// add Material to Repository
void addRepo(Repository* repo, Material* m)
{
    addElementToDynamicArray(repo->arr,m);
}

// remove Material from Repository
void removeRepo(Repository* repo, int pos)
{
    deleteElementFromDynamicArray(repo->arr,pos);
}

// update Material from Repository
void updateRepo(Repository* repo, int pos,Material* m2)
{
    updateElementOnPosition(repo->arr,pos,m2);
}

// get all Materials from Repository
DynamicArray* get_all(Repository* repo)
{
    DynamicArray* new_arr;
    new_arr = copyDynamicArrayOfMaterials(repo->arr);
    return new_arr;
}

// get size of Repository
int getSizeRepo(Repository* repo)
{
    return getLengthOfDynamicArray(repo->arr);
}

int checkIfMaterialIsInRepository(Repository *repository, Material *material)
{
    for (int i = 0; i < getSizeRepo(repository); ++i)
    {
        Material *new_material = getMaterialByPosition(repository, i);
        if (new_material && areMaterialsEqual(new_material, material) == 1)
            return i;
    }
    return -1;
}

Repository *copyRepository(Repository *repository)
{
    Repository *newRepository = createRepo();
    int length = getSizeRepo(repository);
    for (int i = 0; i < length; ++i)
    {
        Material *m = getMaterialByPosition(repository, i);
        addRepo(newRepository, copyMaterial(m));
    }
    newRepository->arr->length = length;
    return newRepository;
}
// Replaces the content of one repository with another.
void setRepository(Repository *repository, Repository *newRepository)
{
    destroyRepo(repository);
    repository = copyRepository(newRepository);
}

DynamicArray *getDynamicArray(Repository *repository)
{
    return repository->arr;
}

Material *getMaterialByPosition(Repository *repository, int position)
{
    return (Material *)getElementOnPosition(getDynamicArray(repository), position);
}

