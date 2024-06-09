//
// Created by Mara Ielciu on 24.03.2024.
//

#ifndef OOP_A4_5_MARAIELCIU1_UI_H
#define OOP_A4_5_MARAIELCIU1_UI_H


#include "../Service/Service.h"

class Ui {
private:
    Service service;
public:
    Ui();
    ~Ui();
    void printMenu();
    void printMenuAdmin();
    void printMenuUser();
    void addTutorial();
    void deleteTutorial();
    void updateTutorial();
    void printAllTutorials();
    void run();
    void getTutorialByPresenter();
    void seeWatchList();
    void deleteFromWatchList();

};


#endif //OOP_A4_5_MARAIELCIU1_UI_H
