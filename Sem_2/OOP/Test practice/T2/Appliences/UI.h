//
// Created by Mara Ielciu on 22.04.2024.
//

#ifndef APPLIENCES_UI_H
#define APPLIENCES_UI_H


#include "Service.h"

class UI {
private:
    Service service;
public:
    UI(){};
    ~UI(){};
    void run();
    void printMenu();
    void add();
    void printAll();
    void printLess();
    void writeToFile();

};


#endif //APPLIENCES_UI_H
