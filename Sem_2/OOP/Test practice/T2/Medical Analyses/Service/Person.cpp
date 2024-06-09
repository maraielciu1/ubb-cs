//
// Created by Mara Ielciu on 20.04.2024.
//

#include "Person.h"
#include "../Domain/BMI.h"
#include "../Domain/BP.h"

Person::Person(const std::string &name) {
    this->name=name;
    this->arr=std::vector<MedicalAnalysis*>();
}

Person::~Person() {
    for(auto p:this->arr)
        delete p;
}
void Person::addAnalysis(MedicalAnalysis* a) {
    this->arr.push_back(a);
}

std::vector<MedicalAnalysis*> Person::getAllAnalyses() {
    return this->arr;
}

std::vector<MedicalAnalysis*> Person::getAnalysesByMonth(int month) {
    std::vector<MedicalAnalysis*> v;
    for(auto p:this->arr)
        if(std::stoi(p->getDate().substr(5,2))==month)
            v.push_back(p);
    return v;
}

bool Person::isIll(int month) {
    for(auto p:this->arr)
        if(p->getDate().substr(5,2)==std::to_string(month) && !p->isResultOK())
            return true;
    return false;
}

std::vector<MedicalAnalysis *> Person::getAnalysesBetweenDates(const std::string &date1, const std::string &date2) {
    std::vector<MedicalAnalysis *> v;
    for(auto m:this->arr)
        if(m->getDate()>=date1&&m->getDate()<=date2)
            v.push_back(m);
    return v;
}

void Person::writeToFile(const std::string &filename, const std::string &date1, const std::string &date2) {
    std::vector<MedicalAnalysis *> v= getAnalysesBetweenDates(date1,date2);
    std::ofstream file("../data.txt",std::ios::trunc);
    for(auto a:v)
        file<<a->toString()<<"\n";
    file.close();
}

void Person::startingAnalysis() {
    BMI* b=new BMI("2024.03.12",20);
    BP* p=new BP("2024.03.10",95,66);
    this->addAnalysis(b);
    this->addAnalysis(p);
}






