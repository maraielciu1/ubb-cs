#include <QApplication>
#include "ChatSesion.h"
#include "chatwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ChatSession chatSession;

    ChatWindow chatWindow1("Alice", chatSession);
    ChatWindow chatWindow2("Bob", chatSession);

    chatSession.addMessage(std::pair<std::string,std::string>{"Alice", "Hello"});
    chatSession.addMessage(std::pair<std::string,std::string>{"Bob", "Hi"});

    chatWindow1.show();
    chatWindow2.show();

    return QApplication::exec();
}
