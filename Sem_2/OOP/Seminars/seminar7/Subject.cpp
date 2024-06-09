//
// Created by Mara Ielciu on 30.05.2024.
//

#include "Subject.h"

void Subject::registerObserver(Observer *obs) {
    observers.push_back(obs);
}

void Subject::unregisterObserver(Observer *obs) {
    observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
}

void Subject::notify() {
    for(auto obs: this->observers)
        obs->update();
}
