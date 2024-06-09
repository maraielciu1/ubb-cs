//
// Created by Mara Ielciu on 27.05.2024.
//

#ifndef SEARCH_ENGINE_REPO_H
#define SEARCH_ENGINE_REPO_H
#include "Document.h"

class Repo {
private:
    std::vector<Document> documents;
    void readFromFile();
    void sorting();
public:
    Repo();
    ~Repo(){};
    std::vector<Document> getAll(){return this->documents;};
};


#endif //SEARCH_ENGINE_REPO_H
