//
// Created by Mara Ielciu on 27.05.2024.
//

#include "Service.h"

std::vector<Document> Service::getFiltered(std::string searchedS) {
    std::vector<Document> res;
    for(auto &i:this->getAll())
        if(i.getName().find(searchedS)!=std::string::npos||i.getKeywords().find(searchedS)!=std::string::npos)
            res.push_back(i);
    return res;
}

std::string Service::bestMatched(std::string searchedS) {
    std::string res="";
    for(auto &i: this->getAll())
        if(i.getName().find(searchedS)!=std::string::npos)
        {
            if(res.empty()||res.length()>i.getName().length())
                res=i.getName();
        }
    return res;
}


