#include "Planet.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Planet* createPlanet(char* name, char* type, double distanceFromEarth)
{
	Planet* p = malloc(sizeof(Planet));
	if (p == NULL)
		return NULL;
	p->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	if (p->name == NULL)
	{
		free(p);
		return NULL;
	}
	strcpy(p->name, name);
	p->type = (char*)malloc(sizeof(char) * (strlen(type) + 1));
	if (p->type == NULL)
	{
		free(p->name);
		free(p);
		return NULL;
	}
	strcpy(p->type, type);
	
	p->distanceFromEarth = distanceFromEarth;

	return p;
}

void destroyPlanet(Planet* p)
{
	if (p == NULL)
		return;

	// free the memory which was allocated for the component fields
	free(p->name);
	free(p->type);

	// free the memory which was allocated for the planet structure
	free(p);
}

char* getName(Planet* p)
{
	return p->name;
}

char* getType(Planet* p)
{
	return p->type;
}

double getDistanceFromEarth(Planet* p)
{
	return p->distanceFromEarth;
}

void toString(Planet* p, char str[])
{
	sprintf(str, "Planet %s is a %s planet and its distance from Earth is %.2lf.", p->name, p->type, p->distanceFromEarth);
}

Planet* copyPlanet(Planet* planet)
{
	if (planet == NULL)
		return NULL;
	Planet* new_p = createPlanet(planet->name, planet->type, planet->distanceFromEarth);
	return new_p;
}
