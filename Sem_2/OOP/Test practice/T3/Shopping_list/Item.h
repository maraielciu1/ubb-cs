//
// Created by Mara Ielciu on 27.05.2024.
//

#ifndef SHOPPING_LIST_ITEM_H
#define SHOPPING_LIST_ITEM_H
#include <iostream>

class Item {
private:
    std::string category;
    std::string name;
    int quantity;
public:
    Item(std::string category="", std::string name="", int quantity=0);
    ~Item(){};
    std::string getCategory(){ return this->category;};
    friend std::istream &operator>>(std::istream &is, Item &t);
    static std::vector<std::string> tokenize(std::string str, char dekimiter);
    std::string toString();
    std::string getName(){ return this->name;};
    int getQuantity(){ return this->quantity;};
};


#endif //SHOPPING_LIST_ITEM_H
