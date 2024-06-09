//
// Created by Mara Ielciu on 27.05.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_gui.h" resolved

#include "gui.h"
#include "ui_gui.h"


gui::gui(Service &service,QWidget *parent) : service{service},
        QWidget(parent), ui(new Ui::gui) {
    ui->setupUi(this);
    this->populateList();
    QObject::connect(ui->searchLineEdit,&QLineEdit::textChanged,this,&gui::search);
    QObject::connect(ui->filterPushButton,&QPushButton::clicked,this,&gui::filter);
}

gui::~gui() {
    delete ui;
}

void gui::populateList() {
    ui->itemListWidget->clear();
    for(auto &i: this->service.getAll())
    {
        QListWidgetItem* item= new QListWidgetItem(QString::fromStdString(i.toString()));
        ui->itemListWidget->addItem(item);
    }
}

void gui::filter() {
    ui->filteredListWidget->clear();
    std::string category=ui->categoryLineEdit->text().toStdString();
    std::vector<Item> res=this->service.getFilteredCategory(category);
    ui->filteredListWidget->clear();
    for(auto &i: res)
    {
        QListWidgetItem* item= new QListWidgetItem(QString::fromStdString(i.toString()));
        ui->filteredListWidget->addItem(item);
    }
}

void gui::search() {
    ui->itemListWidget->clear();
    std::string category=ui->searchLineEdit->text().toStdString();
    std::vector<Item> res=this->service.getFiltered(category);
    ui->itemListWidget->clear();
    for(auto &i: res)
    {
        QListWidgetItem* item= new QListWidgetItem(QString::fromStdString(i.toString()));
        ui->itemListWidget->addItem(item);
    }
}
