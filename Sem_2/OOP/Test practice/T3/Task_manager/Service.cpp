//
// Created by Mara Ielciu on 27.05.2024.
//

#include "Service.h"

std::vector<Task> Service::getAll() {
    return this->repo.getAll();
}

std::vector<Task> Service::getFiltered(int priority) {
    std::vector<Task> res;
    for(auto &i: this->getAll())
        if(i.getPriority()==priority)
            res.push_back(i);
    return res;
}

int Service::computeDuration(std::vector<Task> t) {
    int sum=0;
    for(auto &i: t)
        sum+=i.getDuration();
    return sum;
}
