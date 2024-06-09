//
// Created by Mara Ielciu on 27.05.2024.
//

#ifndef SHOPPING_LIST_REPO_H
#define SHOPPING_LIST_REPO_H
#include "Item.h"

class Repo {
private:
    std::vector<Item> items;
    void readFromFile();
    void sorting();
public:
    Repo();
    ~Repo(){};
    std::vector<Item> getAll(){ return this->items;};
};


#endif //SHOPPING_LIST_REPO_H
