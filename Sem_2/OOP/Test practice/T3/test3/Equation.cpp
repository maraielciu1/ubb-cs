//
// Created by Mara Ielciu on 28.05.2024.
//

#include "Equation.h"
#include <sstream>

Equation::Equation(double a, double b, double c): a{a}, b{b}, c{c} {

}

int Equation::getDegree() {
    if(a!=0)
        return 2;
    else
        if(b!=0)
            return 1;
        else
            return 0;
}

std::string Equation::toString() {
    std::string res="";
    if(a!=0)
        res=std::to_string(a)+"x^2";
    if(b<0)
        res+=std::to_string(b)+"x";
    else{
        if(b>0)
            res+="+"+std::to_string(b)+"x";
    }
    if(c>0)
        res+="+"+std::to_string(c);
    else
        if(c<0)
            res+=std::to_string(c);
    return res;
}

std::istream &operator>>(std::istream &is, Equation &eq) {
    std::string line;
    getline(is,line);

    std::vector<std::string> result=Equation::tokenize(line,',');
    if( result.size()!=3)
        return is;
    eq.a=std::stod(result[0]);
    eq.b=std::stod(result[1]);
    eq.c=std::stod(result[2]);
    return is;
}

std::vector<std::string> Equation::tokenize(std::string str, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    std::stringstream ss(str);
    while(getline(ss,token,delimiter))
        result.push_back(token);
    return result;
}

void Equation::updateEq(double a1, double b1, double c1) {
    this->a=a1;
    this->b=b1;
    this->c=c1;
}

std::string Equation::getSolutions() {
    if(a==0&&b!=0)
        return "x="+std::to_string(-c/b);
    if(a==0&&b==0&&c==0)
        return "not an equation";
    if(a==0&&b==0&&c!=0)
        return "there are no solutions";
    double delta=b*b-4*a*c;
    if(delta==0){
        double x=-b/(2*a);
        return "x1=x2="+std::to_string(x);
    }
    else
    if(delta>0){
        double x1=(-b+ sqrt(delta))/(2*a);
        double x2=(-b- sqrt(delta))/(2*a);
        return "x1="+std::to_string(x1)+", x2="+std::to_string(x2);
    }
    else{
        std::string x1=std::to_string(-b/(2*a))+"+i*"+std::to_string(sqrt(-delta)/(2*a));
        std::string x2=std::to_string(-b/(2*a))+"-i*"+std::to_string(sqrt(-delta)/(2*a));
        return "x1="+x1+", x2="+x2;
    }
}
