//
// Created by Mara Ielciu on 02.04.2024.
//

#ifndef T1_MARAIELCIU1_1_PLAYER_H
#define T1_MARAIELCIU1_1_PLAYER_H
#pragma once
#include <iostream>

class Player {
private:
    std::string name,nationality,team;
    int nr_goals;
public:
    Player(const std::string name="", const std::string nationality="", const std::string team="", int nr=0);
    ~Player();
    Player(const Player& p);
    void validatePlayer();
    Player& operator=(const Player& p);
    bool operator==(const Player& s);
    std::string str();
    int getNrGoals();
    std::string getTeam();


};


#endif //T1_MARAIELCIU1_1_PLAYER_H
