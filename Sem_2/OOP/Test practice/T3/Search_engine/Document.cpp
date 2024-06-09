//
// Created by Mara Ielciu on 27.05.2024.
//

#include "Document.h"
#include <sstream>

std::istream &operator>>(std::istream &is, Document &t) {
    std::string line;
    getline(is,line);
    std::vector<std::string> res=Document::tokenize(line,'|');
    if(res.size()!=3)
        return is;
    t.name=res[0];
    t.keywords=res[1];
    t.content=res[2];
    return is;
}

std::vector<std::string> Document::tokenize(std::string str, char dekimiter) {
    std::vector<std::string> res;
    std::string token;
    std::stringstream ss(str);
    while(getline(ss,token,dekimiter))
        res.push_back(token);
    return res;
}

std::string Document::toString() {
    return this->name+"|"+this->keywords;
}