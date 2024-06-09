#pragma once
#include "Planet.h"

typedef struct
{
	char* name;
	Planet* old_p;
	Planet* new_p;
}Operation;

Operation* createOp(char* name, Planet* p1, Planet* p2);
void DestructOp(Operation* op);