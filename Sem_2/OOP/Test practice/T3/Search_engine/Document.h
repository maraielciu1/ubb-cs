//
// Created by Mara Ielciu on 27.05.2024.
//

#ifndef SEARCH_ENGINE_DOCUMENT_H
#define SEARCH_ENGINE_DOCUMENT_H
#include <iostream>

class Document {
private:
    std::string name;
    std::string keywords;
    std::string content;
public:
    Document(std::string name="",std::string key="",std::string content=""): name{name}, keywords{key}, content{content}{};
    ~Document(){};
    std::string getName(){ return this->name;};
    std::string getKeywords(){ return this->keywords;};
    std::string toString();
    static std::vector<std::string> tokenize(std::string str, char dekimiter);
    friend std::istream &operator>>(std::istream &is, Document &t);
};


#endif //SEARCH_ENGINE_DOCUMENT_H
