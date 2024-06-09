//
// Created by Mara Ielciu on 22.04.2024.
//

#include "Appliance.h"

double Refrigerator::consumedElectricity() {
    double x;
    if(this->electricityUsageClass=="A")
        x=3;
    else if(this->electricityUsageClass=="A+")
        x=2.5;
    else
        x=2;
    if(!this->hasFreezer)
        return 30*x;
    else
        return 30*x+2;
}

std::string Refrigerator::toString() {
    if(this->hasFreezer)
        return "Refrigerator->class: "+this->electricityUsageClass+" with freezer "+Appliance::toString();
    return "Refrigerator->class: "+this->electricityUsageClass+" without freezer "+Appliance::toString();
}

double DishWasher::consumedElectricity() {
    return this->washingCycleLength*this->consumedElectricityForOneHour*20;
}

std::string DishWasher::toString() {
    return "Dish washer-> "+Appliance::toString()+" washingCycleLength: "+std::to_string(this->washingCycleLength)+" consumedElectricityForOneHour: "+std::to_string(this->consumedElectricityForOneHour);
}
