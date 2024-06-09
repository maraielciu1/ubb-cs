//
// Created by Mara Ielciu on 07.05.2024.
//

#ifndef OOP_A8_9_MARAIELCIU1_ADMINWINDOW_H
#define OOP_A8_9_MARAIELCIU1_ADMINWINDOW_H

#include <QWidget>
#include "QtWidgets/qlabel.h"
#include "QtWidgets/qtablewidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <vector>
#include "Tutorial.h"
#include "Service.h"

namespace CONSTANT{
    const int TITLE=0;
    const int PRESENTER=1;
    const int LINK=2;
    const int MINUTES=3;
    const int SECONDS=4;
    const int LIKES=5;
    const int NR_COLUMNS=6;
}
class AdminWindow: public QWidget {
    Q_OBJECT
private:
    Service &service;
    public:
    AdminWindow(Service &service,QWidget *parent = nullptr );
    void *createTable();
    ~AdminWindow(){};

    QTableWidget *tutorialTable;
    QPushButton *addTutorialButton, *deleteTutorialButton, *updateTutorialButton, *displayPlotButton, *undoButton, *redoButton;
    QLineEdit *tutorialTitleInput, *tutorialPresenterInput, *tutorialLinkInput, *tutorialMinutesInput, *tutorialSecondsInput, *tutorialLikesInput;


    QHBoxLayout *createButtonsTutorials();
    QHBoxLayout *createTitle();
    QHBoxLayout *createPresenter();
    QHBoxLayout *createLink();
    QHBoxLayout *createMinutes();
    QHBoxLayout *createSeconds();
    QHBoxLayout *createLikes();
    void createTutorialLayout(QVBoxLayout *layout);
    void populateTable();

public slots:
    void addTutorial();
    void addTutorialToTable(const Tutorial &tutorial) const;
    void updateTutorialInTable(int row, const Tutorial &tutorial) const;
    void deleteTutorial();
    void updateTutorial();
    void displayTutorial();
    void displayPlot();
    void undoUI();
    void redoUI();
    void undoShortCut();
    void redoShortCut();

};


#endif //OOP_A8_9_MARAIELCIU1_ADMINWINDOW_H
