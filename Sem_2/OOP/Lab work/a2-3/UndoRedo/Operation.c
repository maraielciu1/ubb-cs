//
// Created by Mara Ielciu on 16.03.2024.
//

#include "Operation.h"
#include "Domain/Domain.h"
#include <stdlib.h>
#include <string.h>

// create Operation
Operation* createOperation(int type, Material* old_m, Material* new_m)
{
    Operation* op = malloc(sizeof(Operation));
    if (op == NULL)
        return NULL;
    op->type = type;

    if(old_m) op->old_m = copyMaterial(old_m);
    else op->old_m = NULL;
    if(new_m) op->new_m = copyMaterial(new_m);
    else op->new_m = NULL;

    return op;
}

// destroy Operation
void destroyOperation(Operation* op)
{
    if(op->old_m) destroyMaterial(op->old_m);
    if(op->new_m) destroyMaterial(op->new_m);
    free(op);
}

// get type
int get_type(Operation* op)
{
    return op->type;
}

// get old_m
Material* get_old_m(Operation* op)
{
    return op->old_m;
}

// get new_m
Material* get_new_m(Operation* op)
{
    return op->new_m;
}