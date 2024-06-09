//
// Created by Mara Ielciu on 27.05.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_gui.h" resolved

#include <QMessageBox>
#include "gui.h"
#include "ui_gui.h"


gui::gui(Service &service,QWidget *parent) : service{service},
        QWidget(parent), ui(new Ui::gui) {
    ui->setupUi(this);
    this->populateList();
    QObject::connect(ui->searchLineEdit,&QLineEdit::textChanged,this,&gui::search);
    QObject::connect(ui->showPushButton,&QPushButton::clicked,this,&gui::findBest);
}

gui::~gui() {
    delete ui;
}

void gui::populateList() {
    ui->documentListWidget->clear();
    for(auto &i: this->service.getAll())
    {
        QListWidgetItem* item=new QListWidgetItem(QString::fromStdString(i.toString()));
        ui->documentListWidget->addItem(item);
    }
}

void gui::search() {
    ui->documentListWidget->clear();
    std::string find_str=ui->searchLineEdit->text().toStdString();
    std::vector<Document> res=this->service.getFiltered(find_str);
    for(auto &i: res)
    {
        QListWidgetItem* item=new QListWidgetItem(QString::fromStdString(i.toString()));
        ui->documentListWidget->addItem(item);
    }
}

void gui::findBest() {
    ui->bestMLineEdit->clear();
    std::string find_str=ui->searchLineEdit->text().toStdString();
    std::string res=this->service.bestMatched(find_str);
    if(res.empty())
    {
        QMessageBox::critical(this,"Error","No document found!");
    }
    else
    {
        ui->bestMLineEdit->setText(QString::fromStdString(res));
    }
}
