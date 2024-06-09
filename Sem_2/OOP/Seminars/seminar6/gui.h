//
// Created by Mara Ielciu on 16.05.2024.
//

#ifndef SEMINAR6_914_GUI_H
#define SEMINAR6_914_GUI_H

#include <QWidget>
#include "Service.h"


QT_BEGIN_NAMESPACE
namespace Ui { class GUI; }
QT_END_NAMESPACE

class GUI : public QWidget {
Q_OBJECT
private:
    Service &service;

public:
    explicit GUI(Service &service, QWidget *parent = nullptr);

    ~GUI() override;

private:
    Ui::GUI *ui;
    void populateList();
};


#endif //SEMINAR6_914_GUI_H
