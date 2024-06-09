//
// Created by Mara Ielciu on 21.04.2024.
//

#ifndef REAL_ESTATE_UI_H
#define REAL_ESTATE_UI_H
#pragma once
#include <iostream>
using namespace std;
#include "RealEstateAgency.h"

class UI {
private:
    RealEstateAgency agency;
public:
    UI(){};
    ~UI(){};
    void run();
    void printMenu();
    void addDwelling();
    void removeClient();
    void getInterestedClients();
    void writeToFile();
    void printAll();
};


#endif //REAL_ESTATE_UI_H
