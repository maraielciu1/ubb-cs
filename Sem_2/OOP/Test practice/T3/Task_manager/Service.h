//
// Created by Mara Ielciu on 27.05.2024.
//

#ifndef TASK_MANAGER_SERVICE_H
#define TASK_MANAGER_SERVICE_H


#include "Repo.h"

class Service {
private:
    Repo &repo;
public:
    Service(Repo &repo): repo{repo}{};
    ~Service(){};
    std::vector<Task> getAll();
    std::vector<Task> getFiltered(int priority);
    int computeDuration(std::vector<Task> t);

};


#endif //TASK_MANAGER_SERVICE_H
