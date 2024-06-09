//
// Created by Mara Ielciu on 20.04.2024.
//

#ifndef PRACTICE_TEST2_MEDICALANALYSIS_H
#define PRACTICE_TEST2_MEDICALANALYSIS_H
#pragma once
#include <iostream>
#include <fstream>

class MedicalAnalysis {
protected:
    std::string date;
public:
    MedicalAnalysis(const std::string &date): date{date}{}
    ~MedicalAnalysis(){}
    std::string getDate() const { return this->date; }
    virtual bool isResultOK() = 0;
    virtual std::string toString() const;
    friend std::ostream &operator<<(std::ostream &os, const MedicalAnalysis &m);
};


#endif //PRACTICE_TEST2_MEDICALANALYSIS_H
