//
// Created by Mara Ielciu on 24.03.2024.
//

#include "Repo.h"

Repo::Repo() {
    this->tutorials = std::vector<Tutorial>();
}

Repo::~Repo() {

}

void Repo::addRepo(const Tutorial &tutorial) {
    this->tutorials.push_back(tutorial);
}

void Repo::deleteRepo(const Tutorial &tutorial) {
    auto it = std::find_if(tutorials.begin(), tutorials.end(),
                           [&](const Tutorial& t) { return t == tutorial; });
    if (it != tutorials.end()) {
        tutorials.erase(it);
    }
}

void Repo::updateRepo(int position, const Tutorial &tutorial) {
    if(this->validatePosition(position))
        this->tutorials[position] = tutorial;
}

void Repo::deleteRepoPosition(int position) {
    if(this->validatePosition(position))
        this->tutorials.erase(tutorials.begin() + position);
}

std::vector<Tutorial> Repo::getAllRepo() const {
    return this->tutorials;
}

bool Repo::validatePosition(int position) {
    return position >= 0 && position < this->tutorials.size();
}

int Repo::getSizeRepo() const {
    return this->tutorials.size();
}

int Repo::returnPosition(const Tutorial &tutorial) const{
    auto it = std::find_if(tutorials.begin(), tutorials.end(),
                           [&](const Tutorial& t) { return t == tutorial; });
    if (it != tutorials.end()) {
        return std::distance(tutorials.begin(), it);
    }
    return -1;
}

Tutorial Repo::getRepoElement(int position) {
    return this->tutorials[position];
}

void Repo::modifyLikesPosition(int position, int new_likes) {
    if(this->validatePosition(position))
        this->tutorials[position].setLikes(this->tutorials[position].getLikes() + new_likes);
}

