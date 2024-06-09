//
// Created by Mara Ielciu on 27.05.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_gui.h" resolved

#include "gui.h"
#include "ui_gui.h"
#include <QMessageBox>


gui::gui(Service &service,QWidget *parent) : service{service},
        QWidget(parent), ui(new Ui::gui) {
    ui->setupUi(this);
    this->populateList();
    QObject::connect(ui->showPushButton,&QPushButton::clicked,this,&gui::compute);
    QObject::connect(ui->backPushButton,&QPushButton::clicked,this,&gui::populateList);
}

gui::~gui() {
    delete ui;
}

void gui::populateList() {
    ui->taskListWidget->clear();
    ui->durationLineEdit->clear();
    ui->priorityLineEdit->clear();
    for(auto &t: this->service.getAll())
    {
        QListWidgetItem* item=new QListWidgetItem(QString::fromStdString(t.toString()));
        ui->taskListWidget->addItem(item);
        if(t.getPriority()==1)
        {
            QFont font=item->font();
            font.setBold(true);
            item->setFont(font);
        }
        ui->taskListWidget->addItem(item);
    }
}

void gui::compute() {
    int priority=ui->priorityLineEdit->text().toInt();
    std::vector<Task> res=this->service.getFiltered(priority);
    if(res.size()==0)
    {
        QMessageBox::critical(this,"Error","No tasks with the given priority!");
    }
    else
    {
        int duration=this->service.computeDuration(res);
        ui->durationLineEdit->setText(QString::number(duration));
        ui->taskListWidget->clear();
        for(auto &t: res)
        {
            QListWidgetItem* item=new QListWidgetItem(QString::fromStdString(t.toString()));
            ui->taskListWidget->addItem(item);
            if(t.getPriority()==1)
            {
                QFont font=item->font();
                font.setBold(true);
                item->setFont(font);
            }
            ui->taskListWidget->addItem(item);
        }
    }
}
