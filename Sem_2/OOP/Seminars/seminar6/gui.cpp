//
// Created by Mara Ielciu on 16.05.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GUI.h" resolved

#include <QListWidgetItem>
#include "gui.h"
#include "ui_GUI.h"


GUI::GUI(Service &service,QWidget *parent) : service{service},
        QWidget(parent), ui(new Ui::GUI) {
    ui->setupUi(this);
}

GUI::~GUI() {
    delete ui;
}

void GUI::populateList() {
    for (const auto &tutorial : this->service.getAllSongs()) {
//        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(tutorial.toString()));
//        this->ui->songsListWidget->addItem(item);
    }
}
