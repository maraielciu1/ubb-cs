//
// Created by Mara Ielciu on 23.04.2024.
//

#ifndef OOP_T2_MARAIELCIU1_BUILDING_H
#define OOP_T2_MARAIELCIU1_BUILDING_H
#pragma once
#include <iostream>

class Building {
protected:
    int constructionYear;
public:
    Building(int year=1900): constructionYear{year}{};
    virtual ~Building(){};
    virtual bool mustBeRestored()=0;
    virtual bool canBeDemolished()=0;
    virtual std::string toString();
    int getYear(){
        return this->constructionYear;
    };
};

class Block: public Building{
private:
    int totalApartments;
    int occupiedApartments;
public:
    Block(int year=1900,int ap=0, int oc=0): Building(year), totalApartments{ap}, occupiedApartments{oc}{};
    ~Block(){};
    bool mustBeRestored() override;
    bool canBeDemolished() override;
    std::string toString() override;

};

class House: public Building{
private:
    bool isHistorical;
public:
    House(int year=1900,bool his=true): Building(year), isHistorical{his}{};
    ~House(){};
    bool mustBeRestored() override;
    bool canBeDemolished() override;
    std::string toString() override;
};


#endif //OOP_T2_MARAIELCIU1_BUILDING_H
