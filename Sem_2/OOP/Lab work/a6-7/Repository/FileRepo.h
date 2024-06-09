//
// Created by Mara Ielciu on 11.04.2024.
//

#ifndef OOP_A6_7_MARAIELCIU1_FILEREPO_H
#define OOP_A6_7_MARAIELCIU1_FILEREPO_H
#pragma once

#include "Repo.h"

class FileRepo: public Repo{
private:
    std::string file_name;
public:
    FileRepo(const std::string &filename);
    virtual ~FileRepo();
    void writeToFile() override;
    void readFromFile() override;
    void addRepo(const Tutorial& tutorial) override;
    void deleteRepo(const Tutorial& tutorial) override;
    void updateRepo(int position, const Tutorial& tutorial) override;
    void deleteRepoPosition(int position) override;



};


#endif //OOP_A6_7_MARAIELCIU1_FILEREPO_H
