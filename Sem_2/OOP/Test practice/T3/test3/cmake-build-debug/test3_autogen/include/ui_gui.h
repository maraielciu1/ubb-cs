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
    QListWidget *equationListWidget;
    QLabel *label;
    QLineEdit *alineEdit;
    QLabel *label_2;
    QLineEdit *blineEdit;
    QLabel *label_3;
    QLineEdit *clineEdit;
    QPushButton *updatePushButton;
    QPushButton *solutionPushButton;
    QLineEdit *solutionsLineEdit;

    void setupUi(QWidget *gui)
    {
        if (gui->objectName().isEmpty())
            gui->setObjectName("gui");
        gui->resize(400, 366);
        verticalLayout = new QVBoxLayout(gui);
        verticalLayout->setObjectName("verticalLayout");
        equationListWidget = new QListWidget(gui);
        equationListWidget->setObjectName("equationListWidget");

        verticalLayout->addWidget(equationListWidget);

        label = new QLabel(gui);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        alineEdit = new QLineEdit(gui);
        alineEdit->setObjectName("alineEdit");

        verticalLayout->addWidget(alineEdit);

        label_2 = new QLabel(gui);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        blineEdit = new QLineEdit(gui);
        blineEdit->setObjectName("blineEdit");

        verticalLayout->addWidget(blineEdit);

        label_3 = new QLabel(gui);
        label_3->setObjectName("label_3");

        verticalLayout->addWidget(label_3);

        clineEdit = new QLineEdit(gui);
        clineEdit->setObjectName("clineEdit");

        verticalLayout->addWidget(clineEdit);

        updatePushButton = new QPushButton(gui);
        updatePushButton->setObjectName("updatePushButton");

        verticalLayout->addWidget(updatePushButton);

        solutionPushButton = new QPushButton(gui);
        solutionPushButton->setObjectName("solutionPushButton");

        verticalLayout->addWidget(solutionPushButton);

        solutionsLineEdit = new QLineEdit(gui);
        solutionsLineEdit->setObjectName("solutionsLineEdit");

        verticalLayout->addWidget(solutionsLineEdit);


        retranslateUi(gui);

        QMetaObject::connectSlotsByName(gui);
    } // setupUi

    void retranslateUi(QWidget *gui)
    {
        gui->setWindowTitle(QCoreApplication::translate("gui", "gui", nullptr));
        label->setText(QCoreApplication::translate("gui", "a: ", nullptr));
        label_2->setText(QCoreApplication::translate("gui", "b: ", nullptr));
        label_3->setText(QCoreApplication::translate("gui", "c: ", nullptr));
        updatePushButton->setText(QCoreApplication::translate("gui", "Update", nullptr));
        solutionPushButton->setText(QCoreApplication::translate("gui", "Compute solutions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gui: public Ui_gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
