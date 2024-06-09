//
// Created by Mara Ielciu on 16.03.2024.
//

#ifndef A2_3_MARAIELCIU1_OPERATION_H
#define A2_3_MARAIELCIU1_OPERATION_H

#endif //A2_3_MARAIELCIU1_OPERATION_H
#include "Domain/Domain.h"
#pragma once

typedef struct
{
    int type;
    Material* old_m;
    Material* new_m;
}Operation;

Operation* createOperation(int type, Material* old_m, Material* new_m);
void destroyOperation(Operation* op);
int get_type(Operation* op);
Material* get_old_m(Operation* op);
Material* get_new_m(Operation* op);

