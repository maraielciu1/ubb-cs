//
// Created by Mara Ielciu on 22.04.2024.
//

#ifndef BUILDINGS_BUILDING_H
#define BUILDINGS_BUILDING_H
#pragma once
#include <iostream>

using namespace std;

class Building {
protected:
    string address;
    int constructionYear;
public:
    Building(const string &address="", int constructionYear=0): address{address}, constructionYear{constructionYear} {};
    virtual ~Building(){};
    virtual bool mustBeRestored() = 0;
    virtual bool canBeDemolished()=0;
    virtual string toString(){
        return "Address: " + address + ", Construction Year: " + to_string(constructionYear);
    };
    int getConstructionYear(){
        return constructionYear;
    };
};

class Block: public Building{
private:
    int totalApartments;
    int occupiedApartments;
public:
    Block(const string &address="", int constructionYear=0, int ap=0, int oc=0): Building(address, constructionYear), totalApartments{ap}, occupiedApartments{oc}{};
    ~Block(){};
    bool mustBeRestored() override;
    bool canBeDemolished() override;
    string toString() override;
};

class House: public Building{
private:
    bool isHistorical;
public:
    House(const string &address="", int constructionYear=0, bool h= true): Building(address, constructionYear), isHistorical{h}{};
    ~House(){};
    bool mustBeRestored() override;
    bool canBeDemolished() override;
    string toString() override;
};

#endif //BUILDINGS_BUILDING_H
