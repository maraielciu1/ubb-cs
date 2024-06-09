//
// Created by Mara Ielciu on 28.05.2024.
//

#ifndef TEST3_REPO_H
#define TEST3_REPO_H
#include "Equation.h"

class Repo {
private:
    std::vector<Equation> equations;
    void readFromFile();
    void sorting();
public:
    Repo();
    ~Repo(){};
    std::vector<Equation> getAll();
    void updateEq(int index, double a1, double b1, double c1);
    std::string getSolutions(int index);
};


#endif //TEST3_REPO_H
