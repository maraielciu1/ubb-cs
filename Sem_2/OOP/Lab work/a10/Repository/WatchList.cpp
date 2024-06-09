//
// Created by Mara Ielciu on 18.04.2024.
//

#include "WatchList.h"

void WatchList::addTutorial(const Tutorial &tut) {
    this->tutorials.push_back(tut);
}

void WatchList::removeTutorialFromPlaylist(int position) {
    std::vector<Tutorial>::iterator pos=this->tutorials.begin();
    std::advance(pos,position);
    if(pos!=this->tutorials.end())
        this->tutorials.erase(pos);
}

bool WatchList::isEmpty() {
    return this->tutorials.size()==0;
}

void WatchList::checkIfTutorialIsAlreadyInTheList(const Tutorial &tutorial) {
    for(auto tut: this->tutorials)
        if(tut.getTitle()==tutorial.getTitle())
            throw DuplicateTutException();
}
