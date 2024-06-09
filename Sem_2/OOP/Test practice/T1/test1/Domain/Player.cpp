//
// Created by Mara Ielciu on 02.04.2024.
//

#include "Player.h"

Player::Player(const std::string name, const std::string nationality, const std::string team, int nr): name{name}, nationality{nationality}, team{team}, nr_goals{nr} {

}

Player::~Player() {

}

Player::Player(const Player &p) {
    this->name = p.name;
    this->nationality=p.nationality;
    this->team=p.team;
    this->nr_goals=p.nr_goals;
}

void Player::validatePlayer() {
    if(this->name.empty()||this->nationality.empty()||this->team.empty())
        throw std::invalid_argument("Invalid Player");
}

Player &Player::operator=(const Player &p) {
   if(this==&p)
       return *this;
    this->name = p.name;
    this->nationality=p.nationality;
    this->team=p.team;
    this->nr_goals=p.nr_goals;
    return *this;
}

bool Player::operator==(const Player &s) {
    return this->name==s.name;
}

std::string Player::str() {
    std::string result="";
    result+=this->name;
    result+="|";
    result+=this->nationality;
    result+="|";
    result+=this->team;
    result+="|";
    result+=std::to_string(this->nr_goals);
    return result;
}

int Player::getNrGoals() {
    return this->nr_goals;
}

std::string Player::getTeam() {
    return this->team;
}
