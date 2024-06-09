//
// Created by Mara Ielciu on 22.04.2024.
//

#ifndef APPLIENCES_APPLIANCE_H
#define APPLIENCES_APPLIANCE_H
#pragma once
#include <iostream>

class Appliance {
protected:
    std::string id;
    double weight;
public:
    Appliance(const std::string &id="", double weight=0): id{id}, weight{weight} {};
    virtual ~Appliance() {};
    virtual double consumedElectricity()=0;
    virtual std::string toString(){
        return " id: "+this->id+" weight: "+std::to_string(this->weight);
    };

};

class Refrigerator:public Appliance{
private:
    std::string electricityUsageClass;
    bool hasFreezer;
public:
    Refrigerator(const std::string &id="", double weight=0, const std::string &u="A",bool freezer=false): Appliance(id,weight),electricityUsageClass{u},hasFreezer{freezer}{};
    ~Refrigerator(){};
    double consumedElectricity() override;
    std::string toString() override;
};

class DishWasher: public Appliance{
private:
    double washingCycleLength;
    double consumedElectricityForOneHour;
public:
    DishWasher(const std::string &id="", double weight=0,double w=0,double c=0): Appliance(id,weight), washingCycleLength{w},consumedElectricityForOneHour{c}{};
    ~DishWasher(){};
    double consumedElectricity() override;
    std::string toString() override;
};

#endif //APPLIENCES_APPLIANCE_H
