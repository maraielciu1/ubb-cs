#include <QMessageBox>
#include "AdminWindow.h"
#include "qcustomplot.h"

using namespace CONSTANT;

AdminWindow::AdminWindow(Service &service, QWidget *parent) : service(service), QWidget(parent)
{
    try {
        auto *adminLayout = new QVBoxLayout;
        auto *tutorialInformation = new QVBoxLayout;

        QHBoxLayout *tutorialTitleGroup = createTitle();
        QHBoxLayout *tutorialPresenterGroup = createPresenter();
        QHBoxLayout *tutorialLinkGroup = createLink();
        QHBoxLayout *tutorialMinutesGroup = createMinutes();
        QHBoxLayout *tutorialSecondsGroup = createSeconds();
        QHBoxLayout *tutorialLikesGroup = createLikes();
        QHBoxLayout *buttons = createButtonsTutorials();

        tutorialInformation->setSpacing(20);
        tutorialInformation->addLayout(tutorialTitleGroup);
        tutorialInformation->addLayout(tutorialPresenterGroup);
        tutorialInformation->addLayout(tutorialLinkGroup);
        tutorialInformation->addLayout(tutorialMinutesGroup);
        tutorialInformation->addLayout(tutorialSecondsGroup);
        tutorialInformation->addLayout(tutorialLikesGroup);
        tutorialInformation->addLayout(buttons);

        createTable();
        adminLayout->addWidget(tutorialTable);
        adminLayout->addLayout(tutorialInformation);
        this->setLayout(adminLayout);

        QObject::connect(this->addTutorialButton, &QPushButton::clicked, this, &AdminWindow::addTutorial);
        QObject::connect(this->deleteTutorialButton, &QPushButton::clicked, this, &AdminWindow::deleteTutorial);
        QObject::connect(this->tutorialTable, &QTableWidget::itemSelectionChanged, this, &AdminWindow::displayTutorial);
        QObject::connect(this->updateTutorialButton, &QPushButton::clicked, this, &AdminWindow::updateTutorial);
        QObject::connect(this->undoButton, &QPushButton::clicked, this, &AdminWindow::undoUI);
        QObject::connect(this->redoButton, &QPushButton::clicked, this, &AdminWindow::redoUI);
        QObject::connect(this->displayPlotButton, &QPushButton::clicked, this, &AdminWindow::displayPlot);

        // keyboard shortcuts
        QShortcut *undoShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this);
        QShortcut *redoShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y), this);

        connect(undoShortcut,&QShortcut::activated,this,&AdminWindow::undoShortCut);
        connect(redoShortcut,&QShortcut::activated,this,&AdminWindow::redoShortCut);
    }
    catch (std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void *AdminWindow::createTable()
{
    tutorialTable = new QTableWidget(this);
    tutorialTable->setObjectName("TutorialTable");
    QStringList header = {"Title", "Presenter", "Link", "Minutes", "Seconds", "Likes"};
    tutorialTable->setColumnCount(header.size());
    tutorialTable->setHorizontalHeaderLabels(header);

    populateTable();
}

void AdminWindow::populateTable()
{
    try {
        const auto &tutorials = this->service.getAllService();
        tutorialTable->setRowCount(tutorials.size());

        int row = 0;
        for (const auto &tutorial : tutorials) {
            tutorialTable->setItem(row, TITLE, new QTableWidgetItem(QString::fromStdString(tutorial.getTitle())));
            tutorialTable->setItem(row, PRESENTER, new QTableWidgetItem(QString::fromStdString(tutorial.getPresenter())));
            tutorialTable->setItem(row, LINK, new QTableWidgetItem(QString::fromStdString(tutorial.getLink())));
            tutorialTable->setItem(row, MINUTES, new QTableWidgetItem(QString::number(tutorial.getMinutes())));
            tutorialTable->setItem(row, SECONDS, new QTableWidgetItem(QString::number(tutorial.getSeconds())));
            tutorialTable->setItem(row, LIKES, new QTableWidgetItem(QString::number(tutorial.getLikes())));
            ++row;
        }
    } catch (const std::exception &e) {
        qCritical() << "Failed to populate tutorial table:" << e.what();
    }
}

QHBoxLayout* AdminWindow::createButtonsTutorials()
{
    QHBoxLayout *buttons = new QHBoxLayout();
    this->addTutorialButton = new QPushButton("Add Tutorial", this);
    this->deleteTutorialButton = new QPushButton("Delete Tutorial", this);
    this->updateTutorialButton = new QPushButton("Update Tutorial", this);
    this->displayPlotButton = new QPushButton("Display Plot", this);
    this->undoButton = new QPushButton("Undo", this);
    this->redoButton = new QPushButton("Redo", this);

    buttons->addWidget(addTutorialButton);
    buttons->addWidget(deleteTutorialButton);
    buttons->addWidget(updateTutorialButton);
    buttons->addWidget(displayPlotButton);
    buttons->addWidget(undoButton);
    buttons->addWidget(redoButton);

    return buttons;
}

QHBoxLayout* AdminWindow::createTitle()
{
    QLabel *label = new QLabel("Title of the tutorial", this);
    this->tutorialTitleInput = new QLineEdit(this);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);
    layout->addWidget(tutorialTitleInput);

    return layout;
}

QHBoxLayout* AdminWindow::createPresenter()
{
    QLabel *label = new QLabel("Presenter of the tutorial", this);
    this->tutorialPresenterInput = new QLineEdit(this);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);
    layout->addWidget(tutorialPresenterInput);
    return layout;
}

QHBoxLayout* AdminWindow::createLink()
{
    QLabel *label = new QLabel("Link of the tutorial", this);
    this->tutorialLinkInput = new QLineEdit(this);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);
    layout->addWidget(tutorialLinkInput);
    return layout;
}

QHBoxLayout* AdminWindow::createMinutes()
{
    QLabel *label = new QLabel("Minutes of the tutorial", this);
    this->tutorialMinutesInput = new QLineEdit(this);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);
    layout->addWidget(tutorialMinutesInput);
    return layout;
}

QHBoxLayout* AdminWindow::createSeconds()
{
    QLabel *label = new QLabel("Seconds of the tutorial", this);
    this->tutorialSecondsInput = new QLineEdit(this);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);
    layout->addWidget(tutorialSecondsInput);
    return layout;
}

QHBoxLayout* AdminWindow::createLikes()
{
    QLabel *label = new QLabel("Likes of the tutorial", this);
    this->tutorialLikesInput = new QLineEdit(this);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);
    layout->addWidget(tutorialLikesInput);
    return layout;
}

void AdminWindow::addTutorial() {
    std::string title = tutorialTitleInput->text().toStdString();
    std::string presenter = tutorialPresenterInput->text().toStdString();
    std::string link = tutorialLinkInput->text().toStdString();
    int minutes = tutorialMinutesInput->text().toInt();
    int seconds = tutorialSecondsInput->text().toInt();
    int likes = tutorialLikesInput->text().toInt();

    try {
        TutorialValidator::validateTutorial(title, presenter, link, minutes, seconds, likes);
        Tutorial tutorial(title, presenter, link, minutes, seconds, likes);
        this->service.clearRedoStack();
        this->service.addService(tutorial);
        addTutorialToTable(tutorial);
        tutorialTitleInput->clear();
        tutorialPresenterInput->clear();
        tutorialLinkInput->clear();
        tutorialMinutesInput->clear();
        tutorialSecondsInput->clear();
        tutorialLikesInput->clear();
    } catch (std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void AdminWindow::addTutorialToTable(const Tutorial &tutorial) const {
    int row = tutorialTable->rowCount();
    tutorialTable->insertRow(row);
    tutorialTable->setItem(row, TITLE, new QTableWidgetItem(QString::fromStdString(tutorial.getTitle())));
    tutorialTable->setItem(row, PRESENTER, new QTableWidgetItem(QString::fromStdString(tutorial.getPresenter())));
    tutorialTable->setItem(row, LINK, new QTableWidgetItem(QString::fromStdString(tutorial.getLink())));
    tutorialTable->setItem(row, MINUTES, new QTableWidgetItem(QString::number(tutorial.getMinutes())));
    tutorialTable->setItem(row, SECONDS, new QTableWidgetItem(QString::number(tutorial.getSeconds())));
    tutorialTable->setItem(row, LIKES, new QTableWidgetItem(QString::number(tutorial.getLikes())));
}

void AdminWindow::deleteTutorial() {
    int selectedRow = tutorialTable->currentRow();
    if (selectedRow < 0) {
        QMessageBox::critical(this, "Error", "No tutorial selected!");
        return;
    }
    try {
        this->service.clearRedoStack();
        this->service.deleteServicePosition(selectedRow);
        tutorialTable->removeRow(selectedRow);
    } catch (std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void AdminWindow::updateTutorial() {
    int selectedRow = tutorialTable->currentRow();
    if (selectedRow < 0) {
        QMessageBox::critical(this, "Error", "No tutorial selected!");
        return;
    }
    Tutorial oldTutorial = this->service.getServiceElement(selectedRow);
    std::string title = tutorialTitleInput->text().toStdString();
    std::string presenter = tutorialPresenterInput->text().toStdString();
    std::string link = tutorialLinkInput->text().toStdString();
    int minutes = tutorialMinutesInput->text().toInt();
    int seconds = tutorialSecondsInput->text().toInt();
    int likes = tutorialLikesInput->text().toInt();
    try{
        TutorialValidator::validateTutorial(title, presenter, link, minutes, seconds, likes);
        Tutorial tutorial(title, presenter, link, minutes, seconds, likes);
        this->service.clearRedoStack();
        this->service.updateService(selectedRow, tutorial, oldTutorial);
        tutorialTable->setItem(selectedRow, TITLE, new QTableWidgetItem(QString::fromStdString(tutorial.getTitle())));
        tutorialTable->setItem(selectedRow, PRESENTER, new QTableWidgetItem(QString::fromStdString(tutorial.getPresenter())));
        tutorialTable->setItem(selectedRow, LINK, new QTableWidgetItem(QString::fromStdString(tutorial.getLink())));
        tutorialTable->setItem(selectedRow, MINUTES, new QTableWidgetItem(QString::number(tutorial.getMinutes())));
        tutorialTable->setItem(selectedRow, SECONDS, new QTableWidgetItem(QString::number(tutorial.getSeconds())));
        tutorialTable->setItem(selectedRow, LIKES, new QTableWidgetItem(QString::number(tutorial.getLikes())));
    }
    catch (std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void AdminWindow::updateTutorialInTable(int row, const Tutorial &tutorial) const {
    tutorialTable->setItem(row, TITLE, new QTableWidgetItem(QString::fromStdString(tutorial.getTitle())));
    tutorialTable->setItem(row, PRESENTER, new QTableWidgetItem(QString::fromStdString(tutorial.getPresenter())));
    tutorialTable->setItem(row, LINK, new QTableWidgetItem(QString::fromStdString(tutorial.getLink())));
    tutorialTable->setItem(row, MINUTES, new QTableWidgetItem(QString::number(tutorial.getMinutes())));
    tutorialTable->setItem(row, SECONDS, new QTableWidgetItem(QString::number(tutorial.getSeconds())));
    tutorialTable->setItem(row, LIKES, new QTableWidgetItem(QString::number(tutorial.getLikes())));
}

void AdminWindow::displayTutorial() {
    int selectedRow = tutorialTable->currentRow();
    if (selectedRow < 0) {
        return;
    }
    std::string title = tutorialTable->item(selectedRow, TITLE)->text().toStdString();
    std::string presenter = tutorialTable->item(selectedRow, PRESENTER)->text().toStdString();
    std::string link = tutorialTable->item(selectedRow, LINK)->text().toStdString();
    int minutes = tutorialTable->item(selectedRow, MINUTES)->text().toInt();
    int seconds = tutorialTable->item(selectedRow, SECONDS)->text().toInt();
    int likes = tutorialTable->item(selectedRow, LIKES)->text().toInt();

    tutorialTitleInput->setText(QString::fromStdString(title));
    tutorialPresenterInput->setText(QString::fromStdString(presenter));
    tutorialLinkInput->setText(QString::fromStdString(link));
    tutorialMinutesInput->setText(QString::number(minutes));
    tutorialSecondsInput->setText(QString::number(seconds));
    tutorialLikesInput->setText(QString::number(likes));
}

void AdminWindow::displayPlot() {
    std::map<std::string, int> genreCount = this->service.getMapOfTutorialsByPresenter();

    QCustomPlot *customPlot = new QCustomPlot;

    customPlot->setFixedWidth(800);
    customPlot->setFixedHeight(500);
    customPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QCPBars *presenter = new QCPBars(customPlot->xAxis, customPlot->yAxis);

    presenter->setAntialiased(false);
    presenter->setStackingGap(1);
    presenter->setName("Presenters of tutorials");
    presenter->setPen(QPen(QColor(0, 0, 0)));
    presenter->setBrush(QColor(255, 105, 180)); // Set brush color to pink

    QVector<double> ticks;
    QVector<double> moviesData;
    QVector<QString> labels;
    int positionOfBarChartInTheGraph = 0;
    for (auto &iteratorForBarChart : genreCount) {
        ticks << ++positionOfBarChartInTheGraph;
        labels << QString::fromStdString(iteratorForBarChart.first);
        moviesData << iteratorForBarChart.second;
    }
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);

    textTicker->addTicks(ticks, labels);
    presenter->setData(ticks, moviesData);

    customPlot->xAxis->setTicker(textTicker);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setLabel("Presenter");
    customPlot->xAxis->setSubTicks(false);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->setRange(0, 8);
    customPlot->xAxis->grid()->setVisible(true);

    customPlot->yAxis->setRange(0, 12.1);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel("Number of tutorials");
    customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignHCenter);
    customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    customPlot->legend->setBorderPen(Qt::NoPen);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    customPlot->show();
}


void AdminWindow::undoUI() {
    try {
        this->service.undo();
        populateTable(); // Refresh table after undo
    } catch (std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void AdminWindow::redoUI() {
    try {
        this->service.redo();
        populateTable(); // Refresh table after redo
    } catch (std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void AdminWindow::redoShortCut() {
    this->redoUI();
}

void AdminWindow::undoShortCut() {
    this->undoUI();
}
