//
// Created by Mara Ielciu on 20.04.2024.
//

#ifndef PRACTICE_TEST2_PERSON_H
#define PRACTICE_TEST2_PERSON_H
#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "../Domain/MedicalAnalysis.h"

class Person {
private:
    std::string name;
    std::vector<MedicalAnalysis*> arr;
public:
    Person(const std::string &name="");
    ~Person();
    void startingAnalysis();
    void addAnalysis(MedicalAnalysis *a);
    std::vector<MedicalAnalysis*> getAllAnalyses();
    std::vector<MedicalAnalysis*> getAnalysesByMonth(int month);
    bool isIll(int month);
    std::vector<MedicalAnalysis*> getAnalysesBetweenDates(const std::string &date1, const std::string &date2);
    void writeToFile(const std::string &filename, const std::string &date1, const std::string &date2);

};


#endif //PRACTICE_TEST2_PERSON_H
