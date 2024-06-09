//
// Created by Mara Ielciu on 25.03.2024.
//

#include "Domain/DynamicArray.h"
#include "Operation.h"
#pragma once

#ifndef A2_3_MARAIELCIU1_UNDOREDOREPO_H
#define A2_3_MARAIELCIU1_UNDOREDOREPO_H

#endif //A2_3_MARAIELCIU1_UNDOREDOREPO_H
typedef struct
{
    DynamicArray *undoOperations;
    DynamicArray *redoOperations;
} UndoRedoRepo;

UndoRedoRepo *createUndoRedoRepo(int);
Operation *getUndoOperation(UndoRedoRepo *);
Operation *getRedoOperation(UndoRedoRepo *);
DynamicArray *getUndoDynamicArray(UndoRedoRepo *);
DynamicArray *getRedoDynamicArray(UndoRedoRepo *);
DynamicArray *getUndoOperations(UndoRedoRepo *);
DynamicArray *getRedoOperations(UndoRedoRepo *);
void destroyUndoOperations(UndoRedoRepo *);
void destroyRedoOperations(UndoRedoRepo *);
void resetRedoOperations(UndoRedoRepo *);
void resetRedoDynamicArray(UndoRedoRepo *);
void destroyUndoRedoRepo(UndoRedoRepo *);
void addUndoOperation(UndoRedoRepo *, Operation *);
void addRedoOperation(UndoRedoRepo *, Operation *);
void addUndoDynamicArray(UndoRedoRepo *, DynamicArray *);
void addRedoDynamicArray(UndoRedoRepo *, DynamicArray *);
void removeUndoOperation(UndoRedoRepo *);
void removeRedoOperation(UndoRedoRepo *);