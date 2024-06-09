//
// Created by Mara Ielciu on 28.05.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_gui.h" resolved

#include "gui.h"
#include "ui_gui.h"


gui::gui(Service &service,QWidget *parent) : service{service},
        QWidget(parent), ui(new Ui::gui) {
    ui->setupUi(this);
    this->populateList();
    QObject::connect(ui->updatePushButton,&QPushButton::clicked,this,&gui::updateEq);
    QObject::connect(ui->solutionPushButton,&QPushButton::clicked,this,&gui::computeSolution);
}

gui::~gui() {
    delete ui;
}

void gui::populateList() {
    ui->equationListWidget->clear();
    for(auto &i: this->service.getAll())
    {
        QListWidgetItem* item=new QListWidgetItem(QString::fromStdString(i.toString()));
        ui->equationListWidget->addItem(item);
    }
}

void gui::updateEq() {
    int index=ui->equationListWidget->currentRow();
    double a=ui->alineEdit->text().toDouble();
    double b=ui->blineEdit->text().toDouble();
    double c=ui->clineEdit->text().toDouble();
    this->service.updateEq(index,a,b,c);
    this->populateList();
}

void gui::computeSolution() {
    ui->solutionsLineEdit->clear();
    int index=ui->equationListWidget->currentRow();
    std::string res=this->service.computeSolution(index);
    ui->solutionsLineEdit->setText(QString::fromStdString(res));
}
