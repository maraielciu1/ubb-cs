//
// Created by Mara Ielciu on 25.03.2024.
//

#include "UndoRedoRepo.h"
#include <stdlib.h>
UndoRedoRepo *createUndoRedoRepo(int typeOfUndoRedo)
{
    UndoRedoRepo *undoRedoRepo= (UndoRedoRepo *)malloc(sizeof(UndoRedoRepo));
    if (typeOfUndoRedo == 1)
    {
        undoRedoRepo->undoOperations = createDynamicArray(1000, destroyOperation);
        undoRedoRepo->redoOperations = createDynamicArray(1000, destroyOperation);
    }
    else
    {
        undoRedoRepo->undoOperations = createDynamicArray(1000, destroyDynamicArray);
        undoRedoRepo->redoOperations = createDynamicArray(1000, destroyDynamicArray);
    }
    return undoRedoRepo;
}
DynamicArray *getRedoOperations(UndoRedoRepo *undoRedoRepository)
{
    return undoRedoRepository->redoOperations;
}

DynamicArray *getUndoOperations(UndoRedoRepo *undoRedoRepository)
{
    return undoRedoRepository->undoOperations;
}
void destroyUndoOperations(UndoRedoRepo*undoRedoRepository)
{
    destroyDynamicArray(getUndoOperations(undoRedoRepository));
}

void destroyRedoOperations(UndoRedoRepo *undoRedoRepository)
{
    destroyDynamicArray(getRedoOperations(undoRedoRepository));
}


void resetRedoOperations(UndoRedoRepo *undoRedoRepository)
{
    destroyRedoOperations(undoRedoRepository);
    undoRedoRepository->redoOperations = createDynamicArray(1000, destroyOperation);
}

void resetRedoDynamicArray(UndoRedoRepo *undoRedoRepository)
{
    destroyRedoOperations(undoRedoRepository);
    undoRedoRepository->redoOperations = createDynamicArray(1000, destroyDynamicArray);
}

void destroyUndoRedoRepo(UndoRedoRepo *undoRedoRepository)
{
    destroyUndoOperations(undoRedoRepository);
    destroyRedoOperations(undoRedoRepository);
    free(undoRedoRepository);
}

void addUndoOperation(UndoRedoRepo *undoRedoRepository, Operation *operation)
{
    addElementToDynamicArray(getUndoOperations(undoRedoRepository), operation);
}

void addRedoOperation(UndoRedoRepo *undoRedoRepository, Operation *operation)
{
    addElementToDynamicArray(getRedoOperations(undoRedoRepository), operation);
}

void addUndoDynamicArray(UndoRedoRepo *undoRedoRepository, DynamicArray *dynamicArray)
{
    addElementToDynamicArray(getUndoOperations(undoRedoRepository), dynamicArray);
}

void addRedoDynamicArray(UndoRedoRepo *UndoRedoRepository, DynamicArray *dynamicArray)
{
    addElementToDynamicArray(getRedoOperations(UndoRedoRepository), dynamicArray);
}

Operation *getUndoOperation(UndoRedoRepo *undoRedoRepository)
{
    DynamicArray *undoOperations = getUndoOperations(undoRedoRepository);
    return (Operation *)getElementOnPosition(undoOperations, getLengthOfDynamicArray(undoOperations) - 1);
}

Operation *getRedoOperation(UndoRedoRepo *undoRedoRepository)
{
    DynamicArray *redoOperations = getRedoOperations(undoRedoRepository);
    return (Operation *)getElementOnPosition(redoOperations, getLengthOfDynamicArray(redoOperations) - 1);
}

DynamicArray *getUndoDynamicArray(UndoRedoRepo *undoRedoRepository)
{
    DynamicArray *undoOperations = getUndoOperations(undoRedoRepository);
    return (DynamicArray *)getElementOnPosition(undoOperations, getLengthOfDynamicArray(undoOperations) - 1);
}

DynamicArray *getRedoDynamicArray(UndoRedoRepo *undoRedoRepository)
{
    DynamicArray *redoOperations = getRedoOperations(undoRedoRepository);
    return (DynamicArray *)getElementOnPosition(redoOperations, getLengthOfDynamicArray(redoOperations) - 1);
}

void removeUndoOperation(UndoRedoRepo *undoRedoRepository)
{
    DynamicArray *undoOperations = getUndoOperations(undoRedoRepository);
    deleteElementFromDynamicArray(undoOperations, getLengthOfDynamicArray(undoOperations) - 1);
}

void removeRedoOperation(UndoRedoRepo *undoRedoRepository) {
    DynamicArray *redoOperations = getRedoOperations(undoRedoRepository);
    deleteElementFromDynamicArray(redoOperations, getLengthOfDynamicArray(redoOperations) - 1);
}
