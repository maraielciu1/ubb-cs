//
// Created by Mara Ielciu on 28.05.2024.
//

#ifndef TEST3_SERVICE_H
#define TEST3_SERVICE_H
#include "Repo.h"

class Service {
private:
    Repo &repo;
public:
    Service(Repo &repo): repo{repo}{};
    ~Service(){};
    void updateEq(int index, double a1, double b1, double c1);
    std::vector<Equation> getAll();
    std::string computeSolution(int index);
};


#endif //TEST3_SERVICE_H
