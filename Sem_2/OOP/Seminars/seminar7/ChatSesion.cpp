//
// Created by Mara Ielciu on 30.05.2024.
//

#include "ChatSesion.h"

void ChatSession::addMessage(std::pair<std::string, std::string> x) {
    messages.push_back(x);
    this->notify();
}

std::vector<std::pair<std::string, std::string>> ChatSession::getAll() {
    std::cout << "Returning all messages. Count:" << messages.size();
    for (const auto& message : messages) {
        std::cout << (message.first + ": " + message.second);
    }
    return messages;
}

ChatSession::ChatSession() {

}

ChatSession::~ChatSession() {

}
