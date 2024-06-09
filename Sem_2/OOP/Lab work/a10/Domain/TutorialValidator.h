//
// Created by Mara Ielciu on 18.04.2024.
//

#ifndef OOP_A6_7_MARAIELCIU1_TUTORIALVALIDATOR_H
#define OOP_A6_7_MARAIELCIU1_TUTORIALVALIDATOR_H
#pragma once
#include <iostream>

class TutorialExceptions : public std::exception
{
private:
    std::vector<std::invalid_argument> errors;

public:
    TutorialExceptions(std::vector<std::invalid_argument> errors);
    TutorialExceptions(std::string error);
    std::vector<std::invalid_argument> returnErrors();
};

class TutorialValidator
{
public:
    static void validateTutorialT(const std::string &title);
    static void validateTutorialP(const std::string &presenter);
    static void validateTutorialL(const std::string &link);
    static void validateTutorialNr(int nr);
    static void validateTutorialM(int nr);
    static void validateTutorialS(int nr);
    static void validateTutorial(const std::string &title, const std::string &presenter, const std::string &link, int minutes, int seconds, int likes);
};


#endif //OOP_A6_7_MARAIELCIU1_TUTORIALVALIDATOR_H
