//
// Created by Mara Ielciu on 28.05.2024.
//

#include "Repo.h"
#include <fstream>

void Repo::readFromFile() {
    std::ifstream file{"../eq.txt"};
    Equation e;
    while(file>>e)
        this->equations.push_back(e);
}

void Repo::sorting() {
    std::sort(equations.begin(),equations.end(),[](Equation a, Equation b){
        return a.getDegree()>b.getDegree();
    });
}

Repo::Repo() {
    this->readFromFile();
    this->sorting();
}

std::vector<Equation> Repo::getAll() {
    return this->equations;
}

void Repo::updateEq(int index, double a1, double b1, double c1) {
    this->equations[index].updateEq(a1,b1,c1);
}

std::string Repo::getSolutions(int index) {
    return this->equations[index].getSolutions();
}
