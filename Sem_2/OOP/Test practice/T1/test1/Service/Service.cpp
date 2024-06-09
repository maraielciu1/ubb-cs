//
// Created by Mara Ielciu on 02.04.2024.
//

#include "Service.h"

Service::Service() {
    this->repo=Repo();
}

Service::~Service() {

}

bool Service::addPlayerService(Player &p)
/*
 * the parameter p in the player that needs to be added
 * if the player is already in the repo, return false, add it otherwise and return true
 */
{
    if(this->repo.getPositionRepo(p)==-1)
    {
        this->repo.addRepo(p);
        return true;
    }
    else
        return false;
}

Player *Service::getAll()
/*
 * gets all the players with their information, in a sorted order of the nr of goals
 */
{
    return this->repo.get_all_epository();
}

int Service::getSizeServ() {
    return this->repo.getSizeRepo();
}

void Service::generatePlayers() {
    Player p1{"Nora_Mork","NOR","Larvik",83};
    Player p2{"Gullden", "SWE", "CSM_Bucuresti", 80};
    Player p3{"Neagu", "ROU", "CSM_Bucuresti",63};
    Player p4{"Pineau", "FRA", "HCM", 82};
    Player p5{"Pena","ESP","Cargo",59 };
    this->repo.addRepo(p1);
    this->repo.addRepo(p2);
    this->repo.addRepo(p3);
    this->repo.addRepo(p4);
    this->repo.addRepo(p5);
}

DynamicArray<Player> Service::getTeamS(const std::string team, int &nr_total) {
    Player* all=this->getAll();
    nr_total=0;
    DynamicArray<Player> arr{10};
    for(int i=0;i<this->getSizeServ();i++)
        if(all[i].getTeam()==team)
        {
            arr.addElementArray(all[i]);
            nr_total+=all[i].getNrGoals();
        }
    return arr;
}

Player Service::get_player(int pos) {
    return this->repo.getElementRepo(pos);
}

