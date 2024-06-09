//
// Created by Mara Ielciu on 21.04.2024.
//

#ifndef REAL_ESTATE_DWELLING_H
#define REAL_ESTATE_DWELLING_H
#pragma once
#include <iostream>

class Dwelling{
private:
    double price;
    bool isProfitable;
public:
    Dwelling(double price=0, bool prof=false): price{price}, isProfitable{prof}{};
    ~Dwelling(){};
    double getPrice(){ return this->price;};
    bool isDwellingProfitable(){return this->isProfitable;};
    std::string toString() { return "Price: " + std::to_string(this->price) + " profitable?: " + std::to_string(this->isProfitable); };

};



#endif //REAL_ESTATE_DWELLING_H

