//
// Created by Mara Ielciu on 27.05.2024.
//

#ifndef TASK_MANAGER_GUI_H
#define TASK_MANAGER_GUI_H

#include <QWidget>
#include "Service.h"


QT_BEGIN_NAMESPACE
namespace Ui { class gui; }
QT_END_NAMESPACE

class gui : public QWidget {
Q_OBJECT

public:
    explicit gui(Service &service,QWidget *parent = nullptr);
    void populateList();
    ~gui() override;

private:
    Ui::gui *ui;
    Service &service;
public slots:
    void compute();
};


#endif //TASK_MANAGER_GUI_H
