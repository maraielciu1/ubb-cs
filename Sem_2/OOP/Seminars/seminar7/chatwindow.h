//
// Created by Mara Ielciu on 30.05.2024.
//

#ifndef SEMINAR7_CHATWINDOW_H
#define SEMINAR7_CHATWINDOW_H

#include <QWidget>
#include "observer.h"
#include "ChatSesion.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

class ChatWindow : public QWidget, public Observer{
Q_OBJECT

public:
    explicit ChatWindow(std::string name,ChatSession& chatSession,QWidget *parent = nullptr);
    void update() override;
    ~ChatWindow() override;

private:
    std::string name;
    Ui::ChatWindow *ui;
    ChatSession& chatSession;
};


#endif //SEMINAR7_CHATWINDOW_H
