//
// Created by Mara Ielciu on 20.04.2024.
//

#ifndef PRACTICE_TEST2_BP_H
#define PRACTICE_TEST2_BP_H
#pragma once
#include <iostream>
#include "MedicalAnalysis.h"

class BP: public MedicalAnalysis {
private:
    int systolicValue;
    int diastolicValue;
public:
    BP(const std::string &date="",int v1=0, int v2=0): systolicValue{v1}, diastolicValue{v2}, MedicalAnalysis(date){}
    ~BP(){}
    bool isResultOK() override;
    std::string toString() const override;
};


#endif //PRACTICE_TEST2_BP_H
