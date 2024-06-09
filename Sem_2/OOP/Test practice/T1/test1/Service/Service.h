//
// Created by Mara Ielciu on 02.04.2024.
//

#ifndef T1_MARAIELCIU1_1_SERVICE_H
#define T1_MARAIELCIU1_1_SERVICE_H


#include "../Repo/Repo.h"
#pragma once

class Service {
private:
    Repo repo;
public:
    Service();
    ~Service();
    bool addPlayerService(Player& p);
    Player* getAll();
    int getSizeServ();
    void generatePlayers();
    DynamicArray<Player> getTeamS(const std::string team, int& nr_total);
    Player get_player(int pos);

};


#endif //T1_MARAIELCIU1_1_SERVICE_H
