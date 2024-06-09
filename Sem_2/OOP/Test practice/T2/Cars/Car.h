//
// Created by Mara Ielciu on 21.04.2024.
//

#ifndef PRACTICE_TEST2_CAR_H
#define PRACTICE_TEST2_CAR_H
#pragma once
#include "Engine.h"

class Car
{
private:
    std::string bodyStyle;
    Engine *engine;

public:
    Car(std::string bodyStyle = "", Engine *engine = nullptr) : bodyStyle{bodyStyle}, engine{engine} {};
    ~Car(){};
    double computePrice();
    std::string toString();
};


#endif //PRACTICE_TEST2_CAR_H
