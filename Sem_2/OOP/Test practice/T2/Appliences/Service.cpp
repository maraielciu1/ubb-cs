//
// Created by Mara Ielciu on 22.04.2024.
//

#include "Service.h"
#include <fstream>

void Service::addAp(Appliance *a) {
    this->app.push_back(a);
}

std::vector<Appliance *> Service::getLess(double maxel) {
    std::vector<Appliance *> res;
    for(auto a: this->app)
        if(a->consumedElectricity()<maxel)
            res.push_back(a);
    return res;
}

std::vector<Appliance *> Service::getMore(double maxel) {
    std::vector<Appliance *> res;
    for(auto a: this->app)
        if(a->consumedElectricity()>maxel)
            res.push_back(a);
    return res;
}

void Service::writeToFile(const std::string filename,double el) {
    std::ofstream file(filename);
    if(file.is_open())
    {
        for(auto a: this->getMore(el))
            file<<a->toString()<<"\n";
    }
    file.close();
}
