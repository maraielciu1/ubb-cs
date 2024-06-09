//
// Created by Mara Ielciu on 27.05.2024.
//

#include "Item.h"
#include <sstream>

Item::Item(std::string category, std::string name, int quantity): category{category}, name{name}, quantity{quantity} {

}
std::istream &operator>>(std::istream &is, Item &t) {
    std::string line;
    getline(is,line);
    std::vector<std::string> res=Item::tokenize(line,'|');
    if(res.size()!=3)
        return is;
    t.category=res[0];
    t.name=res[1];
    t.quantity=std::stoi(res[2]);
    return is;
}

std::vector<std::string> Item::tokenize(std::string str, char dekimiter) {
    std::vector<std::string> res;
    std::string token;
    std::stringstream ss(str);
    while(getline(ss,token,dekimiter))
        res.push_back(token);
    return res;
}

std::string Item::toString() {
    return this->category+"|"+this->name+"|"+std::to_string(this->quantity);
}
