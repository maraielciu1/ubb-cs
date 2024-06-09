//
// Created by Mara Ielciu on 22.04.2024.
//

#ifndef APPLIENCES_SERVICE_H
#define APPLIENCES_SERVICE_H
#pragma once
#include <vector>
#include "Appliance.h"

class Service {
private:
    std::vector<Appliance*> app;
public:
    Service(){
        Refrigerator* a1=new Refrigerator("1.",30,"A+",true);
        Refrigerator* a2=new Refrigerator("2.",30,"A++",false);
        DishWasher* a3=new DishWasher("3.",20,2,30);
        this->app.push_back(a1);
        this->app.push_back(a2);
        this->app.push_back(a3);
    };
    ~Service(){
        for(auto &a: app)
            delete a;
    };
    void addAp(Appliance* a);
    std::vector<Appliance*> getAll(){return this->app;};
    std::vector<Appliance*> getLess(double maxel);
    std::vector<Appliance*> getMore(double maxel);
    void writeToFile(const std::string filename="../data.txt", double el=0);

};


#endif //APPLIENCES_SERVICE_H
