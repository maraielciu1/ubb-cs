#include "mainWindow.h"
#include "ui_mainWindow.h"
#include <iostream>
#include "QtWidgets/qstackedlayout.h"
#include "QtWidgets/qtoolbar.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QAction>
#include <QLabel>

using namespace CONSTANTS;

MainWindow::MainWindow(Service &service, QWidget *parent) : service{service}, QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    try {
        this->service.setTypeOfRepo(new FileRepo{FILENAME});
    }
    catch (FileException &exception) {
        std::cout << exception.what() << "\n";
    }
    this->setWindowTitle("Master C++");

    QToolBar *toolbar = new QToolBar(this);
    addToolBar(toolbar);

    QAction *adminButton = new QAction("Admin", this);
    QAction *userButton = new QAction("User", this);
    toolbar->addAction(adminButton);
    toolbar->addAction(userButton);

    try {
        QWidget *adminWidget = new AdminWindow(this->service);
        QWidget *userWidget = new UserWindow(this->service);

        QLabel *welcomeLabel = new QLabel("Welcome! Please select the credentials for this application");
        welcomeLabel->setAlignment(Qt::AlignCenter);
        QStackedLayout *stackedLayout = new QStackedLayout();
        stackedLayout->addWidget(welcomeLabel);
        stackedLayout->addWidget(adminWidget);
        stackedLayout->addWidget(userWidget);

        this->centralWidget()->setLayout(stackedLayout);
        this->resize(INITIAL_WIDTH, INITIAL_HEIGHT);

        connect(adminButton, &QAction::triggered, this, [=](){
            this->resize(ADMIN_WIDTH, ADMIN_HEIGHT);
            stackedLayout->setCurrentIndex(ADMIN_LAYOUT_INDEX);
        });

        connect(userButton, &QAction::triggered, this, [=](){
            this->resize(USER_WIDTH, USER_HEIGHT);
            stackedLayout->setCurrentIndex(USER_LAYOUT_INDEX);
        });
    } catch (const std::exception &e) {
        qDebug() << "Exception caught during UserWindow initialization: " << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught during UserWindow initialization.";
    }
}

void MainWindow::addTutorial(Tutorial t) {
    this->service.addService(t);
}

MainWindow::~MainWindow() {
    delete ui;
}
