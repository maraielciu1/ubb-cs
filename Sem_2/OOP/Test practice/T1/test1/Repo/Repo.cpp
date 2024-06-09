//
// Created by Mara Ielciu on 02.04.2024.
//

#include "Repo.h"

Repo::Repo() {
    this->players=DynamicArray<Player>();
}

Repo::~Repo() {

}

void Repo::addRepo(Player &p)
/*
 * parameter p is the player that needs to be added
 * after each element is added, the array is sorted in order to make things easier for point 3
 */
{
    this->players.addElementArray(p);
    this->sortGoals();
}

Player *Repo::get_all_epository() {
    return this->players.getAllElementsArray();
}

bool Repo::validatePosition(int position) {
    return (position>=0&&position<this->players.getSizeArray());
}

int Repo::getPositionRepo(const Player &p) {
    return this->players.returnPosition(p);
}

Player Repo::getElementRepo(int position) {
    return this->players.getElementArray(position);
}

int Repo::getSizeRepo() {
    return this->players.getSizeArray();
}

void Repo::sortGoals()
/*
 * sorting ascending by the number of goals
 */
{
    for(int i=0;i<this->players.getSizeArray()-1;i++)
        for(int j=i+1;j<this->players.getSizeArray();j++)
        if(this->players[i].getNrGoals()>this->players[j].getNrGoals())
        {
            Player aux=this->players[i];
            this->players[i]=this->players[j];
            this->players[j]=aux;
        }
}
