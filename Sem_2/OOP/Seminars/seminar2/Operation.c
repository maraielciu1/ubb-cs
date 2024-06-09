#include "Operation.h"
#include <stdlib.h>
#include <string.h>

Operation* createOp(char* name, Planet* p1, Planet* p2)
{
	Operation* op = malloc(sizeof(Operation));
	if (op == NULL)
		return;
	op->name = malloc(sizeof(char) * (strlen(name) + 1));
	if (op->name == NULL)
	{
		free(op);
		return;
	}
	strcpy(op->name, name);
	op->old_p = copyPlanet(p1);
	op->new_p = copyPlanet(p2);
	return op;
}

void DestructOp(Operation* op)
{
	if (op == NULL)
		return;
	free(op->name);
	destroyPlanet(op->old_p);
	destroyPlanet(op->new_p);
	free(op);
}
