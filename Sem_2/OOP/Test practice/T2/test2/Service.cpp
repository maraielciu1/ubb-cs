//
// Created by Mara Ielciu on 23.04.2024.
//

#include "Service.h"

void Service::addBuilding(Building *b) {
    this->buildings.push_back(b);
}

std::vector<Building *> Service::getAllToBeRestored() {
    std::vector<Building *> restored;
    for(auto b: this->buildings)
        if(b->mustBeRestored())
            restored.push_back(b);
    return restored;
}

std::vector<Building *> Service::getAllToBeDemolished() {
    std::vector<Building *> demolished;
    for(auto b: this->buildings)
        if(b->canBeDemolished())
            demolished.push_back(b);
    return demolished;
}

void Service::writeToFile(const std::string &filename, std::vector<Building *> b) {
    std::ofstream file(filename);
    if(file.is_open())
    {
        for(auto c: b)
            file<<c->toString()<<"\n";
    }
    file.close();
}
