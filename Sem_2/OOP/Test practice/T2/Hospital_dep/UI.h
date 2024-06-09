//
// Created by Mara Ielciu on 21.04.2024.
//

#ifndef HOSPITAL_DEP_UI_H
#define HOSPITAL_DEP_UI_H


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
    void prntEFF();
    void writeToFile();

};


#endif //HOSPITAL_DEP_UI_H
