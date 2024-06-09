//
// Created by Mara Ielciu on 18.04.2024.
//

#ifndef OOP_A6_7_MARAIELCIU1_WATCHLIST_H
#define OOP_A6_7_MARAIELCIU1_WATCHLIST_H

#pragma once
#include "Tutorial.h"
#include "Exceptions.h"

class WatchList {
protected:
    std::vector<Tutorial> tutorials;

public:
    WatchList() {};
    virtual ~WatchList() {}
    void addTutorial(const Tutorial &);
    void removeTutorialFromPlaylist(int);
    bool isEmpty();
    void checkIfTutorialIsAlreadyInTheList(const Tutorial &);
    std::vector<Tutorial> getTutorials() const { return this->tutorials; }
};


#endif //OOP_A6_7_MARAIELCIU1_WATCHLIST_H
