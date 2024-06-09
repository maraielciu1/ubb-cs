//
// Created by Mara Ielciu on 24.03.2024.
//

#ifndef OOP_A4_5_MARAIELCIU1_REPO_H
#define OOP_A4_5_MARAIELCIU1_REPO_H
#pragma ONCE
#include "../Domain/Tutorial.h"

class Repo {
protected:
    std::vector<Tutorial> tutorials;
public:
    Repo();
    virtual ~Repo();
    virtual void addRepo(const Tutorial& tutorial);
    virtual void deleteRepo(const Tutorial& tutorial);
    virtual void updateRepo(int position, const Tutorial& tutorial);
    virtual void deleteRepoPosition(int position);
    virtual void writeToFile() {};
    virtual void readFromFile() {};

    std::vector<Tutorial> getAllRepo() const;
    bool validatePosition(int position);
    int getSizeRepo() const;
    int returnPosition(const Tutorial& tutorial) const;
    Tutorial getRepoElement(int position);
    void modifyLikesPosition(int position, int new_likes);

};


#endif //OOP_A4_5_MARAIELCIU1_REPO_H
