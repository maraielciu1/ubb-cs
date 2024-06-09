//
// Created by Mara Ielciu on 20.04.2024.
//

#ifndef PRACTICE_TEST2_BMI_H
#define PRACTICE_TEST2_BMI_H
#pragma once
#include <iostream>
#include "MedicalAnalysis.h"

class BMI: public MedicalAnalysis {
private:
    double value;
public:
    BMI(const std::string &date="",double v=0): value{v}, MedicalAnalysis(date){}
    ~BMI(){}
    bool isResultOK() override;
    std::string toString() const override;
};


#endif //PRACTICE_TEST2_BMI_H
