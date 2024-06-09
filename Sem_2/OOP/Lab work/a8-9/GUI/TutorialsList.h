//
// Created by Mara Ielciu on 08.05.2024.
//

#ifndef OOP_A8_9_MARAIELCIU1_TUTORIALSLIST_H
#define OOP_A8_9_MARAIELCIU1_TUTORIALSLIST_H

#include <QWidget>
#include "Service.h"
#include <vector>
#include <QTableWidget>

class TutorialsList:  public QTableWidget{
Q_OBJECT
private:
    std::function<std::vector<Tutorial>()> getTutorials;
public:
    TutorialsList(std::function<std::vector<Tutorial>()> getTutorials, QWidget *parent = nullptr){};
    void setTutorials(std::function<std::vector<Tutorial>()> getTutorials){};
public slots:
    void reload(){};
};


#endif //OOP_A8_9_MARAIELCIU1_TUTORIALSLIST_H
