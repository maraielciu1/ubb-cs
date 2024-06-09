//
// Created by Mara Ielciu on 20.04.2024.
//

#include "MedicalAnalysis.h"

std::string MedicalAnalysis::toString() const {
    return this->date;
}

std::ostream &operator<<(std::ostream &os, const MedicalAnalysis &m) {
    os<<m.toString();
    return os;
}
