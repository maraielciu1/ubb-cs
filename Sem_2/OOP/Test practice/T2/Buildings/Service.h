//
// Created by Mara Ielciu on 22.04.2024.
//

#ifndef BUILDINGS_SERVICE_H
#define BUILDINGS_SERVICE_H
#pragma once
#include <vector>
#include "Building.h"
#include <fstream>

class Service {
private:
    std::vector<Building*> buildings;
public:
    Service(){
      House* h1 = new House("Strada1", 1900, true);
      House* h2 = new House("Strada2", 2000, false);
      Block* b1 = new Block("Strada3", 2000, 100, 50);
      Block* b2 = new Block("Strada4", 1900, 100, 81);
        buildings.push_back(h1);
        buildings.push_back(h2);
        buildings.push_back(b1);
        buildings.push_back(b2);
    };
    ~Service(){
        for(auto &building: buildings)
            delete building;
    };
    std::vector<Building*> getBuildings(){
        return buildings;
    };
    std::vector<Building*> getBuildingsToBeRestored(){
        std::vector<Building*> toBeRestored;
        for(auto &building: buildings)
            if(building->mustBeRestored())
                toBeRestored.push_back(building);
        return toBeRestored;
    };
    std::vector<Building*> getBuildingsToBeDemolished(){
        std::vector<Building*> toBeDemolished;
        for(auto &building: buildings)
            if(building->canBeDemolished())
                toBeDemolished.push_back(building);
        return toBeDemolished;
    };
    void addBuilding(Building* building){
        buildings.push_back(building);
    };
    void writeToFile(string &filename, std::vector<Building*> buildings){
       ofstream file(filename);
         for(auto &building: buildings)
              file << building->toString() << endl;
        file.close();
    };
};


#endif //BUILDINGS_SERVICE_H
