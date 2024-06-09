//
// Created by Mara Ielciu on 24.03.2024.
//

#include <iostream>
#include "GUI/mainWindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication mainApplication(argc, argv);
    QCoreApplication::setApplicationName("Mater C++");
    Service service;
    MainWindow mainWindow(service);
    mainWindow.show();
    return mainApplication.exec();
}