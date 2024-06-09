//
// Created by Mara Ielciu on 27.05.2024.
//

#include "Task.h"
#include <sstream>

Task::Task(std::string description, int duration, int priority): description{description}, duration{duration}, priority{priority} {

}

std::istream &operator>>(std::istream &is, Task &t) {
    std::string line;
    getline(is,line);
    std::vector<std::string> res=Task::tokenize(line,'|');
    if(res.size()!=3)
        return is;
    t.description=res[0];
    t.duration=std::stoi(res[1]);
    t.priority=std::stoi(res[2]);
    return is;
}

std::vector<std::string> Task::tokenize(std::string str, char dekimiter) {
    std::vector<std::string> res;
    std::string token;
    std::stringstream ss(str);
    while(getline(ss,token,dekimiter))
        res.push_back(token);
    return res;
}

std::string Task::toString() {
    return this->description+"|"+std::to_string(this->duration)+"|"+std::to_string(this->priority);
}
