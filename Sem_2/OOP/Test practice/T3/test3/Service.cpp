//
// Created by Mara Ielciu on 28.05.2024.
//

#include "Service.h"

void Service::updateEq(int index, double a1, double b1, double c1) {
    this->repo.updateEq(index,a1,b1,c1);
}

std::vector<Equation> Service::getAll() {
    return this->repo.getAll();
}

std::string Service::computeSolution(int index) {
    return this->repo.getSolutions(index);
}
