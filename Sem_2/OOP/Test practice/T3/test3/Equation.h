//
// Created by Mara Ielciu on 28.05.2024.
//

#ifndef TEST3_EQUATION_H
#define TEST3_EQUATION_H
#include <iostream>

class Equation {
private:
    double a,b,c;
public:
    Equation(double a=0, double b=0, double c=0);
    ~Equation(){};
    int getDegree();
    std::string toString();
    void updateEq(double a1, double b1, double c1);
    friend std::istream &operator>>(std::istream &is, Equation &eq);
    static std::vector<std::string> tokenize(std::string str,char delimiter);
    std::string getSolutions();

};


#endif //TEST3_EQUATION_H
