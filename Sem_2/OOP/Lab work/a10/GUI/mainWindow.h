//
// Created by Mara Ielciu on 08.05.2024.
//

#ifndef OOP_A8_9_MARAIELCIU1_MAINWINDOW_H
#define OOP_A8_9_MARAIELCIU1_MAINWINDOW_H
#include <QMainWindow>
#include "Service.h"
#include "FileRepo.h"
#include "Exceptions.h"
#include "AdminWindow.h"
#include "UserWindow.h"

namespace CONSTANTS
{
    const std::string FILENAME = "/Users/maraielciu/Desktop/project-oop/oop-a8-9-maraielciu1/tutorial.txt";
    const int INITIAL_WIDTH = 300;
    const int INITIAL_HEIGHT = 300;
    const int ADMIN_WIDTH = 1500;
    const int ADMIN_HEIGHT = 400;
    const int USER_WIDTH = 800;
    const int USER_HEIGHT = 400;
    const int ADMIN_LAYOUT_INDEX = 1;
    const int USER_LAYOUT_INDEX = 2;
}


QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT
private:
    Service &service;

public:
    MainWindow(Service &service, QWidget *parent = Q_NULLPTR);
    ~MainWindow();

public slots:
    void addTutorial(Tutorial t);

private:
    Ui::MainWindow *ui;

};


#endif //OOP_A8_9_MARAIELCIU1_MAINWINDOW_H
