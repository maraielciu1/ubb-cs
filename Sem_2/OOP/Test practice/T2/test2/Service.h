//
// Created by Mara Ielciu on 23.04.2024.
//

#ifndef OOP_T2_MARAIELCIU1_SERVICE_H
#define OOP_T2_MARAIELCIU1_SERVICE_H
#pragma once
#include "Building.h"
#include <vector>
#include <fstream>

class Service {
private:
    std::vector<Building*> buildings;
public:
    Service(){
        House* b1=new House(1900,true);
        House* b2=new House(2000,false);
        Block* b3=new Block(1980,100,81);
        Block* b4=new Block(2000,100,0);
        this->buildings.push_back(b1);
        this->buildings.push_back(b2);
        this->buildings.push_back(b3);
        this->buildings.push_back(b4);
    };
    ~Service(){
        for(auto b: this->buildings)
            delete b;
    };
    void addBuilding(Building* b);
    std::vector<Building*> getAllBuildings(){
        return this->buildings;
    };
    std::vector<Building*> getAllToBeRestored();
    std::vector<Building*> getAllToBeDemolished();
    void writeToFile(const std::string &filename="../data.txt",std::vector<Building*> b={});
};


#endif //OOP_T2_MARAIELCIU1_SERVICE_H
