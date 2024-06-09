//
// Created by Mara Ielciu on 21.04.2024.
//

#include "Client.h"

std::string Client::toString(){
    return "Name "+this->name+ " - income: "+std::to_string(this->income);
}

bool Person::isInterested(Dwelling d) {
    return d.getPrice()/1000<=this->income;
}

std::string Company::toString() {
    return Client::toString()+" money from investment "+std::to_string(this->moneyFromInvestments);
}

bool Company::isInterested(Dwelling d) {
    return d.getPrice()/100<=this->totalIncome();
}
