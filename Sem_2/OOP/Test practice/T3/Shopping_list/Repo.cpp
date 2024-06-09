//
// Created by Mara Ielciu on 27.05.2024.
//

#include "Repo.h"
#include <fstream>

void Repo::readFromFile() {
    std::ifstream file{"../items.txt"};
    Item i;
    while(file>>i)
        this->items.push_back(i);
}

void Repo::sorting() {
    std::sort(items.begin(),items.end(),[](Item a, Item b){
        return a.getName()<b.getName();
    });
    std::sort(items.begin(),items.end(),[](Item a, Item b){
        return a.getCategory()<b.getCategory();
    });
}

Repo::Repo() {
    this->readFromFile();
    this->sorting();
}
