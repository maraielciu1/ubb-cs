//
// Created by Mara Ielciu on 23.04.2024.
//

#ifndef OOP_T2_MARAIELCIU1_UI_H
#define OOP_T2_MARAIELCIU1_UI_H
#pragma once
#include "Service.h"

using namespace std;
class UI {
private:
    Service service;
public:
    UI(){};
    ~UI(){};
    void printMenu();
    void add();
    void printAll();
    void printDemolished();
    void writeToFile();
    void run();
};


#endif //OOP_T2_MARAIELCIU1_UI_H
