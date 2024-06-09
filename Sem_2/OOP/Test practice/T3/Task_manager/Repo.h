//
// Created by Mara Ielciu on 27.05.2024.
//

#ifndef TASK_MANAGER_REPO_H
#define TASK_MANAGER_REPO_H
#include "Task.h"

class Repo {
private:
    std::vector<Task> tasks;
    void readFromFile();
    void sorting();
public:
    Repo();
    ~Repo(){};
    std::vector<Task> getAll(){return this->tasks;};


};


#endif //TASK_MANAGER_REPO_H
