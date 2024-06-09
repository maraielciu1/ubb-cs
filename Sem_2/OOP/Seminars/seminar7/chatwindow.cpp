//
// Created by Mara Ielciu on 30.05.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChatWindow.h" resolved

#include "chatwindow.h"
#include "ui_ChatWindow.h"


ChatWindow::ChatWindow(std::string name, ChatSession& chatSession, QWidget *parent)
        : QWidget(parent), name(std::move(name)), chatSession(chatSession), ui(new Ui::ChatWindow) {
    ui->setupUi(this);
    this->setWindowTitle(QString::fromStdString(this->name));
    chatSession.registerObserver(this);

    connect(ui->sendPushButton, &QPushButton::clicked, [this, &chatSession]() {
        std::string messageText = ui->messageLineEdit->text().toStdString();
        if (!messageText.empty()) {
            chatSession.addMessage({this->name, messageText});
            ui->messageLineEdit->clear();
        }
    });
    //this->update();
}


ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::update() {
    ui->listWidget->clear();
    qDebug() << "Updating chat window for:" << QString::fromStdString(this->name);
    for (const auto& message : chatSession.getAll()) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(message.first + ": " + message.second));
        if (message.first == this->name) {
            item->setTextAlignment(Qt::AlignRight);
        } else {
            item->setTextAlignment(Qt::AlignLeft);
        }
        ui->listWidget->addItem(item);
    }
}
