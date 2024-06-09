//
// Created by Mara Ielciu on 21.04.2024.
//

#ifndef PRACTICE_TEST2_UI_H
#define PRACTICE_TEST2_UI_H


#include "Controller.h"

class UI
{
private:
    Service service;

public:
    UI(){};
    ~UI(){};
    void addCar();
    void showAllCars();
    void writeCarsWithSmallerPriceToFile();
    void runApp();
};


#endif //PRACTICE_TEST2_UI_H
