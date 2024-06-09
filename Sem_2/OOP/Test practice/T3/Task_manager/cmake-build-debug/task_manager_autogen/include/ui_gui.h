/********************************************************************************
** Form generated from reading UI file 'gui.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gui
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *taskListWidget;
    QLabel *label;
    QLineEdit *priorityLineEdit;
    QPushButton *showPushButton;
    QLineEdit *durationLineEdit;
    QPushButton *backPushButton;

    void setupUi(QWidget *gui)
    {
        if (gui->objectName().isEmpty())
            gui->setObjectName("gui");
        gui->resize(400, 300);
        verticalLayout = new QVBoxLayout(gui);
        verticalLayout->setObjectName("verticalLayout");
        taskListWidget = new QListWidget(gui);
        taskListWidget->setObjectName("taskListWidget");

        verticalLayout->addWidget(taskListWidget);

        label = new QLabel(gui);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        priorityLineEdit = new QLineEdit(gui);
        priorityLineEdit->setObjectName("priorityLineEdit");

        verticalLayout->addWidget(priorityLineEdit);

        showPushButton = new QPushButton(gui);
        showPushButton->setObjectName("showPushButton");

        verticalLayout->addWidget(showPushButton);

        durationLineEdit = new QLineEdit(gui);
        durationLineEdit->setObjectName("durationLineEdit");

        verticalLayout->addWidget(durationLineEdit);

        backPushButton = new QPushButton(gui);
        backPushButton->setObjectName("backPushButton");

        verticalLayout->addWidget(backPushButton);


        retranslateUi(gui);

        QMetaObject::connectSlotsByName(gui);
    } // setupUi

    void retranslateUi(QWidget *gui)
    {
        gui->setWindowTitle(QCoreApplication::translate("gui", "gui", nullptr));
        label->setText(QCoreApplication::translate("gui", "Priority", nullptr));
        showPushButton->setText(QCoreApplication::translate("gui", "Show", nullptr));
        backPushButton->setText(QCoreApplication::translate("gui", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gui: public Ui_gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
