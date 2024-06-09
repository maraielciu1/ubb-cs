//
// Created by Mara Ielciu on 02.04.2024.
//

#ifndef T1_MARAIELCIU1_1_REPO_H
#define T1_MARAIELCIU1_1_REPO_H


#include "../Domain/Player.h"
#include "../Domain/DynamicArray.h"
#pragma once

class Repo {
private:
    DynamicArray<Player> players;
public:
    Repo();
    ~Repo();
    void addRepo(Player &p);
    Player* get_all_epository();
    bool validatePosition(int position);
    int getPositionRepo(const Player& p);
    Player getElementRepo(int position);
    int getSizeRepo();
    void sortGoals();
};


#endif //T1_MARAIELCIU1_1_REPO_H
