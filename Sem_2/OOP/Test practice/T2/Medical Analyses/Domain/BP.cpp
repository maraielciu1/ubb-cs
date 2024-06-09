//
// Created by Mara Ielciu on 20.04.2024.
//

#include "BP.h"

bool BP::isResultOK() {
    bool ok1=this->systolicValue>=90&&this->systolicValue<=119;
    bool ok2=this->diastolicValue>=60&&this->diastolicValue<=79;
    return ok1&&ok2;
}

std::string BP::toString() const{
    return "BP "+MedicalAnalysis::toString()+" "+ std::to_string(this->systolicValue)+" "+std::to_string(this->diastolicValue);
}
