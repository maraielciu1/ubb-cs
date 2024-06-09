//
// Created by Mara Ielciu on 20.04.2024.
//

#ifndef PRACTICE_TEST2_UI_H
#define PRACTICE_TEST2_UI_H
#pragma once
#include <iostream>
#include "../Domain/BMI.h"
#include "../Domain/BP.h"
using namespace std;

#include "../Service/Person.h"

class UI {
private:
    Person p;
public:
    UI();
    ~UI();
    void print_menu();
    void add();
    void printAll();
    void isIll();
    void writeToFileM();
    void run();
};


#endif //PRACTICE_TEST2_UI_H
