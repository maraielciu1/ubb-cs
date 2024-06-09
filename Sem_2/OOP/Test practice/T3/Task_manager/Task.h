//
// Created by Mara Ielciu on 27.05.2024.
//

#ifndef TASK_MANAGER_TASK_H
#define TASK_MANAGER_TASK_H
#include <iostream>

class Task {
private:
    std::string description;
    int duration;
    int priority;
public:
    Task(std::string description="",int duration=0,int priority=1);
    ~Task(){};
    int getPriority(){ return this->priority;};
    int getDuration(){ return this->duration;};
    friend std::istream &operator>>(std::istream &is, Task &t);
    static std::vector<std::string> tokenize(std::string str, char dekimiter);
    std::string toString();
};


#endif //TASK_MANAGER_TASK_H
