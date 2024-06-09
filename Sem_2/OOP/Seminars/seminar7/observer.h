//
// Created by Mara Ielciu on 30.05.2024.
//

#ifndef SEMINAR7_OBSERVER_H
#define SEMINAR7_OBSERVER_H

#include <string>

class Observer {
public:
    Observer(){};
    virtual void update() = 0;
    virtual ~Observer() = default;

};


#endif //SEMINAR7_OBSERVER_H
