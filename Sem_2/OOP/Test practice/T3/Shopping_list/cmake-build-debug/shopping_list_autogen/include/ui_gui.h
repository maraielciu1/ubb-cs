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
    QLineEdit *searchLineEdit;
    QListWidget *itemListWidget;
    QListWidget *filteredListWidget;
    QLabel *label;
    QLineEdit *categoryLineEdit;
    QPushButton *filterPushButton;

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

        itemListWidget = new QListWidget(gui);
        itemListWidget->setObjectName("itemListWidget");

        verticalLayout->addWidget(itemListWidget);

        filteredListWidget = new QListWidget(gui);
        filteredListWidget->setObjectName("filteredListWidget");

        verticalLayout->addWidget(filteredListWidget);

        label = new QLabel(gui);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        categoryLineEdit = new QLineEdit(gui);
        categoryLineEdit->setObjectName("categoryLineEdit");

        verticalLayout->addWidget(categoryLineEdit);

        filterPushButton = new QPushButton(gui);
        filterPushButton->setObjectName("filterPushButton");

        verticalLayout->addWidget(filterPushButton);


        retranslateUi(gui);

        QMetaObject::connectSlotsByName(gui);
    } // setupUi

    void retranslateUi(QWidget *gui)
    {
        gui->setWindowTitle(QCoreApplication::translate("gui", "gui", nullptr));
        label->setText(QCoreApplication::translate("gui", "Category", nullptr));
        filterPushButton->setText(QCoreApplication::translate("gui", "Filter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gui: public Ui_gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
