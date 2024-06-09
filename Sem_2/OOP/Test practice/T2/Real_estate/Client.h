//
// Created by Mara Ielciu on 21.04.2024.
//

#ifndef REAL_ESTATE_CLIENT_H
#define REAL_ESTATE_CLIENT_H
#pragma once
#include <iostream>
#include "Dwelling.h"

class Client {
protected:
    std::string name;
    double income;
public:
    Client(const std::string &name="", double income=0): name{name},income{income}{};
    virtual ~Client(){};
    virtual double totalIncome() {return this->income;};
    virtual std::string toString();
    virtual bool isInterested(Dwelling d)=0;
    std::string getName(){return this->name;};
};

class Person: public Client{
public:
    Person(const std::string &name="",double income=0): Client(name,income){};
    ~Person(){};
    bool isInterested(Dwelling d) override;
};

class Company: public Client{
private:
    double moneyFromInvestments;
public:
    Company(const std::string &name="",double income=0, double money=0): Client(name,income), moneyFromInvestments{money}{};
    ~Company(){};
    double totalIncome(){return this->income+this->moneyFromInvestments;} ;
    std::string toString() override;
    bool isInterested(Dwelling d);

};


#endif //REAL_ESTATE_CLIENT_H
