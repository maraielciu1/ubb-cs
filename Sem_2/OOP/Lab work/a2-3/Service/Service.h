//
// Created by Mara Ielciu on 15.03.2024.
//

#ifndef A2_3_MARAIELCIU1_SERVICE_H
#define A2_3_MARAIELCIU1_SERVICE_H

#endif //A2_3_MARAIELCIU1_SERVICE_H
#pragma once
#include "Repo/Repository.h"
#include "UndoRedo/UndoRedoRepo.h"

typedef struct
{
    Repository* repo;
    UndoRedoRepo* undoRedo;
    int typeRepo;

}Service;

Service* createService(Repository* repo, UndoRedoRepo* undoRedo, int typeRepo);
void destroyService(Service* s);
int addMaterialService(Service* s, Material* m);
int deleteMaterialService(Service* s, Material* m);
int updateMaterialService(Service* s, Material* m1, Material* m2);
DynamicArray* get_all_service(Service* s);
int get_size_service(Service* s);
DynamicArray* pastExpirationDate(Service* s, char* str);
DynamicArray* pastExpirationDateBg(Service* s, char* str);
void destroyExpDate(DynamicArray* arr);
DynamicArray* getQforSupplier(Service* s, char* str, int q);
void sortMaterialsByQuantityA(DynamicArray* materials);
void sortMaterialsByQuantityD(DynamicArray* materials);
DynamicArray* returnAscending(Service* s, char* str, int q);
DynamicArray* returnDescending(Service* s, char* str, int q);
UndoRedoRepo *getUndoRedo(Service *s);
typedef DynamicArray* (*SortFunction)(Service*, char*, int);
DynamicArray* sortMaterials(Service* s, char* supplier, int quantity, SortFunction sortFunc);
Repository *getRepository(Service *s);
int undoRedoService(Service *service, DynamicArray *undoFromThisArray, DynamicArray *redoToThisArray);

