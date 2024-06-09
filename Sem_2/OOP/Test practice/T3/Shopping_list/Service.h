//
// Created by Mara Ielciu on 27.05.2024.
//

#ifndef SHOPPING_LIST_SERVICE_H
#define SHOPPING_LIST_SERVICE_H
#include "Repo.h"

class Service {
private:
    Repo &repo;
public:
    Service(Repo &repo): repo{repo}{};
    ~Service(){};
    std::vector<Item> getAll(){ return this->repo.getAll();};
    std::vector<Item> getFiltered(std::string searched);
    std::vector<Item> getFilteredCategory(std::string searched);

};


#endif //SHOPPING_LIST_SERVICE_H
