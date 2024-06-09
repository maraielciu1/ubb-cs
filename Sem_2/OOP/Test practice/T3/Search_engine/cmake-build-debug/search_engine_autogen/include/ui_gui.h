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
    QLineEdit *searchLineEdit;
    QListWidget *documentListWidget;
    QPushButton *showPushButton;
    QLineEdit *bestMLineEdit;

    void setupUi(QWidget *gui)
    {
        if (gui->objectName().isEmpty())
            gui->setObjectName("gui");
        gui->resize(400, 300);
        verticalLayout = new QVBoxLayout(gui);
        verticalLayout->setObjectName("verticalLayout");
        searchLineEdit = new QLineEdit(gui);
        searchLineEdit->setObjectName("searchLineEdit");

        verticalLayout->addWidget(searchLineEdit);

        documentListWidget = new QListWidget(gui);
        documentListWidget->setObjectName("documentListWidget");

        verticalLayout->addWidget(documentListWidget);

        showPushButton = new QPushButton(gui);
        showPushButton->setObjectName("showPushButton");

        verticalLayout->addWidget(showPushButton);

        bestMLineEdit = new QLineEdit(gui);
        bestMLineEdit->setObjectName("bestMLineEdit");

        verticalLayout->addWidget(bestMLineEdit);


        retranslateUi(gui);

        QMetaObject::connectSlotsByName(gui);
    } // setupUi

    void retranslateUi(QWidget *gui)
    {
        gui->setWindowTitle(QCoreApplication::translate("gui", "gui", nullptr));
        showPushButton->setText(QCoreApplication::translate("gui", "Show best matched", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gui: public Ui_gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
