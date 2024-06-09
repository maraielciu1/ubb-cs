//
// Created by Mara Ielciu on 15.03.2024.
//

#ifndef A2_3_MARAIELCIU1_REPOSITORY_H
#define A2_3_MARAIELCIU1_REPOSITORY_H

#endif //A2_3_MARAIELCIU1_REPOSITORY_H
#pragma once
#include "Domain/DynamicArray.h"

typedef struct
{
    DynamicArray* arr;
//    DynamicArray* undo;
//    DynamicArray* redo;
}Repository;

Repository* createRepo();
void destroyRepo(Repository* repo);
void addRepo(Repository* repo, Material* m);
void removeRepo(Repository* repo, int pos);
void updateRepo(Repository* repo, int pos,Material* m2);
DynamicArray* get_all(Repository* repo);
int getSizeRepo(Repository* repo);
int checkIfMaterialIsInRepository(Repository *, Material *);
Material *getMaterialByPosition(Repository *, int);
Repository *copyRepository(Repository *);
void setRepository(Repository *, Repository *);
DynamicArray *getDynamicArray(Repository *);

