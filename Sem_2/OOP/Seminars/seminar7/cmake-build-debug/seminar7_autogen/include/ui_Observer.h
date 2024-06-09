/********************************************************************************
** Form generated from reading UI file 'Observer.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBSERVER_H
#define UI_OBSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Observer
{
public:

    void setupUi(QWidget *Observer)
    {
        if (Observer->objectName().isEmpty())
            Observer->setObjectName("Observer");
        Observer->resize(400, 300);

        retranslateUi(Observer);

        QMetaObject::connectSlotsByName(Observer);
    } // setupUi

    void retranslateUi(QWidget *Observer)
    {
        Observer->setWindowTitle(QCoreApplication::translate("Observer", "Observer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Observer: public Ui_Observer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBSERVER_H
