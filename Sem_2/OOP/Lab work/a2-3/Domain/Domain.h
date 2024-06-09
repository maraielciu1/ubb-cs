//
// Created by Mara Ielciu on 14.03.2024.
//

#ifndef A2_3_MARAIELCIU1_DOMAIN_H
#define A2_3_MARAIELCIU1_DOMAIN_H

#endif //A2_3_MARAIELCIU1_DOMAIN_H
#pragma once

// struct for date
typedef struct{
    int day;
    int month;
    int year;
} Date;

// struct for material
typedef struct{
    char* name;
    char* supplier;
    int quantity;
    Date exp_date;
} Material;

Material* createMaterial(char* name, char* supplier, int quantity, Date exp_date);
Date createDate(int day, int month, int year);
void destroyMaterial(Material* m);
char* getName(Material* m);
char* getSupplier(Material* m);
int getQuantity(Material* m);
Date getExpDate(Material* m);
Material* copyMaterial(Material* m);
void setQuantity(Material* m, int quantity);
int areMaterialsEqual(Material* m1, Material* m2);
int compare_dates(Date d1, Date d2);
int past_date(Date d1, Date d2);
Date getCurrentDate();
