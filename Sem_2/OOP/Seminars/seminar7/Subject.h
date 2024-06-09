//
// Created by Mara Ielciu on 30.05.2024.
//

#ifndef SEMINAR7_SUBJECT_H
#define SEMINAR7_SUBJECT_H
#include <utility>
#include <vector>
#include <iostream>

#include "observer.h"

class Subject {
private:
    std::vector<Observer*> observers;
public:
    Subject(){};
    ~Subject(){ };
    void registerObserver(Observer* obs);
    void unregisterObserver(Observer* obs);
    void notify();
};


#endif //SEMINAR7_SUBJECT_H
