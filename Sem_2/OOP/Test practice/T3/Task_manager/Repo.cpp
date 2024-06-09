//
// Created by Mara Ielciu on 27.05.2024.
//

#include "Repo.h"
#include <fstream>
#include <algorithm>

void Repo::readFromFile() {
    std::ifstream file{"../tasks.txt"};
    Task t;
    while(file>>t)
        this->tasks.push_back(t);
}

void Repo::sorting() {
    // Sort by duration first
    std::sort(this->tasks.begin(), this->tasks.end(), [](Task a,Task b) {
        return a.getDuration() < b.getDuration();
    });

    // Sort by priority second
    std::sort(this->tasks.begin(), this->tasks.end(), [](Task a, Task b) {
        return a.getPriority() < b.getPriority();
    });
}

Repo::Repo() {
    this->readFromFile();
    this->sorting();
}
