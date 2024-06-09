//
// Created by Mara Ielciu on 25.04.2024.
//

#ifndef OOP_A6_7_MARAIELCIU1_DBREPO_H
#define OOP_A6_7_MARAIELCIU1_DBREPO_H
#pragma once
#include "Repo.h"
#include <sqlite3.h>

class DBRepo: public Repo {
private:
    sqlite3* dbWithTutorials;
    std::string dbName;
public:
    DBRepo(const std::string &dbname);
    virtual ~DBRepo();
    void readFromFile() override;
    void addRepo(const Tutorial& tutorial) override;
    void deleteRepo(const Tutorial& tutorial) override;
    void updateRepo(int position, const Tutorial& tutorial) override;
    void deleteRepoPosition(int position) override;
};


#endif //OOP_A6_7_MARAIELCIU1_DBREPO_H
