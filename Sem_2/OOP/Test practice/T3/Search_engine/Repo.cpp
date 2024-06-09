//
// Created by Mara Ielciu on 27.05.2024.
//

#include "Repo.h"
#include <fstream>

void Repo::readFromFile() {
    std::ifstream file{"../documents.txt"};
    Document d;
    while(file>>d)
        documents.push_back(d);
}

void Repo::sorting() {
    std::sort(this->documents.begin(), this->documents.end(), [](Document a,Document b) {
        return a.getName() < b.getName();
    });
}

Repo::Repo() {
    this->readFromFile();
    this->sorting();
}

