//
// Created by Mara Ielciu on 14.03.2024.
//
#include "Domain.h"
#include <stdlib.h>
#include <string.h>

// create Date
Date createDate(int day, int month, int year)
{
    Date new;
    new.day=day;
    new.year=year;
    new.month=month;
    return new;
}

// create Material
Material* createMaterial(char* name, char* supplier, int quantity, Date exp_date)
{
    Material* m=malloc(sizeof(Material));
    if(m==NULL)
        return NULL;
    m->name=malloc(sizeof(char)*(strlen(name) + 1));
    if(m->name==NULL)
        return NULL;
    strcpy(m->name,name);
    m->supplier=malloc(sizeof(char)*(strlen(supplier) + 1));
    if (m->supplier==NULL)
        return NULL;
    strcpy(m->supplier,supplier);
    m->quantity=quantity;
    m->exp_date=exp_date;
    return m;
}

// destroy Material
void destroyMaterial(Material* m)
{
    free(m->supplier);
    free(m->name);

    free(m);
}

// get name
char* getName(Material* m)
{
    return m->name;
}

// get supplier
char* getSupplier(Material* m)
{
    return m->supplier;
}

// get quantity
int getQuantity(Material* m)
{
    return m->quantity;
}

// get expiration date
Date getExpDate(Material* m)
{
    return m->exp_date;
}

// copy Material
Material* copyMaterial(Material* m)
{
    Material* new_material=createMaterial(m->name,m->supplier,m->quantity,m->exp_date);
    return new_material;
}

// set quantity
void setQuantity(Material* m, int quantity)
{
    m->quantity=quantity;
}

// check if two materials are equal
int areMaterialsEqual(Material* m1, Material* m2) {
    if (strcmp(m1->name, m2->name) == 0 && strcmp(m1->supplier, m2->supplier) == 0 && compare_dates(m1->exp_date, m2->exp_date) == 0) {
        return 1; // True, materials are equal
    }
    return 0; // False, materials are not equal
}

// compare two dates
int compare_dates(Date d1, Date d2)
{
    if (d1.year != d2.year)
        return d1.year - d2.year;
    else if (d1.month != d2.month)
        return d1.month - d2.month;
    else
        return d1.day - d2.day;
}

// returns 1 if d1>d2, 0 otherwise
int past_date(Date d1, Date d2) {
    if (d1.year > d2.year) {
        return 1;
    } else if (d1.year == d2.year) {
        if (d1.month > d2.month) {
            return 1;
        } else if (d1.month == d2.month && d1.day > d2.day) {
            return 1;
        }
    }
    return 0;
}

// today's date
Date getCurrentDate()
{
    Date d;
    d.day=19;
    d.month=3;
    d.year=2024;
    return d;
}
