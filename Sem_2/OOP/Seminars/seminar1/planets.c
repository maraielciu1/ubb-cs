//
// Created by Mara Ielciu on 29.02.2024.
//
#include <stdio.h>

typedef struct{
    char name[20];
    char type[30];
    double distance;
}Planet;

typedef struct{
    int size;
    Planet planet[30];
}Repo;

void add(Planet p, Repo *r)
{
    r->planet[r->size]=p; // sau (*r).size but it's better with ->
    r->size++;
}

int main()
{
    Planet p;
    Repo r;
    r.size=0;
    printf("Input name: \n");
    scanf("%s", p.name);
    printf("Input type: \n");
    scanf("%s", p.type);
    printf("Input distance: \n");
    scanf("%lf", &p.distance);

    add(p,&r);
    printf("%d\n",r.size);

}