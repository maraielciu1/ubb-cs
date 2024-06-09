//
// Created by Mara Ielciu on 30.05.2024.
//

#ifndef SEMINAR7_CHATSESION_H
#define SEMINAR7_CHATSESION_H
#include "Subject.h"

class ChatSession: public Subject{
private:
    std::vector<std::pair<std::string,std::string>> messages;
public:
    ChatSession();
    ~ChatSession();
    void addMessage(std::pair<std::string,std::string> x);
    std::vector<std::pair<std::string,std::string>> getAll();
};


#endif //SEMINAR7_CHATSESION_H
