//
// Created by Mara Ielciu on 24.03.2024.
//

#ifndef OOP_A4_5_MARAIELCIU1_REPO_H
#define OOP_A4_5_MARAIELCIU1_REPO_H
#pragma ONCE
#include "../Domain/Tutorial.h"
#include "DynamicArray.h"

class Repo {
private:
    DynamicArray<Tutorial> tutorials;
public:
    Repo();
    ~Repo();
    void addRepo(const Tutorial& tutorial);
    void deleteRepo(const Tutorial& tutorial);
    void updateRepo(int position, const Tutorial& tutorial);
    void deleteRepoPosition(int position);
    DynamicArray<Tutorial> getAllRepo() const;
    bool validatePosition(int position);
    int getSizeRepo() const;
    int returnPosition(const Tutorial& tutorial);
    Tutorial getRepoElement(int position);
    void modifyLikesPosition(int position, int new_likes);

};


#endif //OOP_A4_5_MARAIELCIU1_REPO_H
