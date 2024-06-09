//
// Created by Mara Ielciu on 18.04.2024.
//

#include <regex>
#include "TutorialValidator.h"

TutorialExceptions::TutorialExceptions(std::vector<std::invalid_argument> errors) {
    this->errors=errors;
}

TutorialExceptions::TutorialExceptions(std::string error)
{
    this->errors.push_back(std::invalid_argument(error));
}

std::vector<std::invalid_argument> TutorialExceptions::returnErrors() {
    return this->errors;
}

void TutorialValidator::validateTutorialT(const std::string &title) {
    if(title.empty())
        throw std::invalid_argument("Title cannot be empty");
}

void TutorialValidator::validateTutorialP(const std::string &presenter) {
    if(presenter.empty())
        throw std::invalid_argument("Presenter cannot be empty");
}

void TutorialValidator::validateTutorialL(const std::string &link) {
    if(link.empty())
        throw std::invalid_argument("Link cannot be empty");
    const std::regex pattern("((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");
    if (!std::regex_match(link, pattern))
        throw std::invalid_argument("Invalid link");
}

void TutorialValidator::validateTutorialNr(int nr) {
    if(nr < 0)
        throw std::invalid_argument("Likes cannot be negative");
}

void TutorialValidator::validateTutorialM(int nr) {
    if(nr < 0)
        throw std::invalid_argument("Minutes cannot be negative");
}

void TutorialValidator::validateTutorialS(int nr) {
    if(nr < 0 || nr >= 60)
        throw std::invalid_argument("Seconds cannot be negative or more than 59");
}

void
TutorialValidator::validateTutorial(const std::string &title, const std::string &presenter, const std::string &link,
                                    int minutes, int seconds, int likes) {
    std::vector<std::invalid_argument> errors;
    try
    {
        validateTutorialT(title);
    }
    catch (const std::invalid_argument &error)
    {
        errors.push_back(error);
    }

    try
    {
        validateTutorialP(presenter);
    }
    catch (const std::invalid_argument &error)
    {
        errors.push_back(error);
    }

    try
    {
        validateTutorialL(link);
    }
    catch (const std::invalid_argument &error)
    {
        errors.push_back(error);
    }

    try
    {
        validateTutorialNr(likes);
    }
    catch (const std::invalid_argument &error)
    {
        errors.push_back(error);
    }

    try
    {
        validateTutorialM(minutes);
    }
    catch (const std::invalid_argument &error)
    {
        errors.push_back(error);
    }

    try
    {
        validateTutorialS(seconds);
    }
    catch (const std::invalid_argument &error)
    {
        errors.push_back(error);
    }

    if (!errors.empty())
        throw TutorialExceptions(errors);

}
