//
// Created by Mara Ielciu on 02.04.2024.
//

#ifndef T1_MARAIELCIU1_1_UI_H
#define T1_MARAIELCIU1_1_UI_H


#include "../Service/Service.h"
#pragma once

class UI {
private:
    Service service;
public:
    UI();
    ~UI();
    void printMenu();
    void run();
    void addPlayer();
    void print_all();
    void print_sorted();

};


#endif //T1_MARAIELCIU1_1_UI_H
