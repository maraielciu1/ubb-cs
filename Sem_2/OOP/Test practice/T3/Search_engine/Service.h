//
// Created by Mara Ielciu on 27.05.2024.
//

#ifndef SEARCH_ENGINE_SERVICE_H
#define SEARCH_ENGINE_SERVICE_H
#include "Repo.h"

class Service {
private:
    Repo &repo;
public:
    Service(Repo &repo): repo{repo}{};
    ~Service(){};
    std::vector<Document> getAll(){return this->repo.getAll();};
    std::vector<Document> getFiltered(std::string searchedS);
    std::string bestMatched(std::string searchedS);
};


#endif //SEARCH_ENGINE_SERVICE_H
