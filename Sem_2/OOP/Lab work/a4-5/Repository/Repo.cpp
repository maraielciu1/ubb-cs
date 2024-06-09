//
// Created by Mara Ielciu on 24.03.2024.
//

#include "Repo.h"

Repo::Repo() {
    this->tutorials = DynamicArray<Tutorial>();
}

Repo::~Repo() {

}

void Repo::addRepo(const Tutorial &tutorial) {
    this->tutorials.addElementArray(tutorial);
}

void Repo::deleteRepo(const Tutorial &tutorial) {
    int position = this->tutorials.returnPosition(tutorial);
    this->tutorials.deleteElementArray(position);
}

void Repo::updateRepo(int position, const Tutorial &tutorial) {
    this->tutorials.updateElementArray(position, tutorial);
}

void Repo::deleteRepoPosition(int position) {
    this->tutorials.deleteElementArray(position);
}

DynamicArray<Tutorial> Repo::getAllRepo() const {
    return this->tutorials;
}

bool Repo::validatePosition(int position) {
    return position >= 0 && position < this->tutorials.getSizeArray();
}

int Repo::getSizeRepo() const {
    return this->tutorials.getSizeArray();
}

int Repo::returnPosition(const Tutorial &tutorial) {
    return this->tutorials.returnPosition(tutorial);
}

Tutorial Repo::getRepoElement(int position) {
    return this->tutorials.getElementArray(position);
}

void Repo::modifyLikesPosition(int position, int new_likes) {
    Tutorial t=this->tutorials.getElementArray(position);
    t.setLikes(t.getLikes()+new_likes);
    this->tutorials.updateElementArray(position,t);
}

