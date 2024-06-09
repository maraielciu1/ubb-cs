//
// Created by Mara Ielciu on 27.05.2024.
//

#include "Service.h"

std::vector<Item> Service::getFilteredCategory(std::string searched) {
    std::vector<Item> res;
    for(auto &i: this->getAll())
        if(i.getCategory()==searched)
            res.push_back(i);
    std::sort(res.begin(),res.end(),[](Item a, Item b){
        return a.getQuantity()<b.getQuantity();
    });
    return res;
}

std::vector<Item> Service::getFiltered(std::string searched) {
    std::vector<Item> res;
    for(auto &i: this->getAll())
        if(i.getCategory().find(searched)!=std::string::npos||i.getName().find(searched)!=std::string::npos)
            res.push_back(i);
    return res;
}
