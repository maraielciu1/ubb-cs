//
// Created by Mara Ielciu on 20.04.2024.
//

#include "BMI.h"

bool BMI::isResultOK() {
    return this->value>=18.5&&this->value<25;
}

std::string BMI::toString() const {
    return "BMI "+MedicalAnalysis::toString()+" "+ std::to_string(this->value);
}
