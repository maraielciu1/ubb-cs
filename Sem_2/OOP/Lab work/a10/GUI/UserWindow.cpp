//
// Created by Mara Ielciu on 08.05.2024.
//

#include "QtWidgets/qlabel.h"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include "UserWindow.h"
#include "HTMLWatchList.h"
#include "CSVWatchList.h"
#include <QMessageBox>

QHBoxLayout *UserWindow::createComboBoxLayout() {
    QHBoxLayout *comboBoxLayout = new QHBoxLayout();
    QLabel *label = new QLabel("Format of Watchlist:", this);
    watchFileType = new QComboBox(this);
    watchFileType->addItem("Select format");
    watchFileType->addItem("CSV");
    watchFileType->addItem("HTML");
    comboBoxLayout->addWidget(label);
    comboBoxLayout->addWidget(watchFileType);
    return comboBoxLayout;
}

QHBoxLayout *UserWindow::createPresenterLayout() {
    QHBoxLayout *presenterGroupFilter = new QHBoxLayout();
    QLabel *presenterLabel = new QLabel("Presenter:", this);
    tutorialPresenterInputFilter = new QLineEdit(this);
    filterTutorialByPresenter = new QPushButton("Get the tutorials", this);
    presenterGroupFilter->addWidget(presenterLabel);
    presenterGroupFilter->addWidget(tutorialPresenterInputFilter);
    presenterGroupFilter->addWidget(filterTutorialByPresenter);
    return presenterGroupFilter;
}

QVBoxLayout *UserWindow::createDeleteFromWatchList() {
    QVBoxLayout *deleteTutorialFromWatchList = new QVBoxLayout;
    QLabel *deleteLabel=new QLabel("Enter the index of the tutorial you have just watched", this);
    this->indexOfTutorialInWatchlist = new QLineEdit(this);
    this->deleteTutorialFromWatchlistButton=new QPushButton("Remove from watchlist", this);
    this->giveLikeAndDeleteTutorial = new QPushButton("Like and remove from watchlist", this);
    deleteTutorialFromWatchList->addWidget(deleteLabel);
    deleteTutorialFromWatchList->addWidget(indexOfTutorialInWatchlist);

    auto *layoutButtons = new QHBoxLayout;
    layoutButtons->addWidget(deleteTutorialFromWatchlistButton);
    layoutButtons->addWidget(giveLikeAndDeleteTutorial);

    deleteTutorialFromWatchList->addLayout(layoutButtons);

    return deleteTutorialFromWatchList;
}

QHBoxLayout *UserWindow::createTitle() {
    QLabel *title=new QLabel("Title of the tutorial", this);
    this->tutorialTitleInput = new QLineEdit(this);
    QHBoxLayout *TitleGroup = new QHBoxLayout;
    TitleGroup->addWidget(title);
    TitleGroup->addWidget(tutorialTitleInput);

    return TitleGroup;
}

QHBoxLayout *UserWindow::createPresenter() {
    QLabel *title=new QLabel("Presenter", this);
    this->tutorialPresenterInput = new QLineEdit(this);
    QHBoxLayout *TitleGroup = new QHBoxLayout;
    TitleGroup->addWidget(title);
    TitleGroup->addWidget(tutorialPresenterInput);

    return TitleGroup;
}

QHBoxLayout *UserWindow::createLink() {
    QLabel *title=new QLabel("Link", this);
    this->tutorialLinkInput = new QLineEdit(this);
    QHBoxLayout *TitleGroup = new QHBoxLayout;
    TitleGroup->addWidget(title);
    TitleGroup->addWidget(tutorialLinkInput);

    return TitleGroup;
}

QHBoxLayout *UserWindow::createMinutes() {
    QLabel *title=new QLabel("Minutes", this);
    this->tutorialMinutesInput = new QLineEdit(this);
    QHBoxLayout *TitleGroup = new QHBoxLayout;
    TitleGroup->addWidget(title);
    TitleGroup->addWidget(tutorialMinutesInput);

    return TitleGroup;
}

QHBoxLayout *UserWindow::createSeconds() {
    QLabel *title=new QLabel("Seconds", this);
    this->tutorialSecondsInput = new QLineEdit(this);
    QHBoxLayout *TitleGroup = new QHBoxLayout;
    TitleGroup->addWidget(title);
    TitleGroup->addWidget(tutorialSecondsInput);

    return TitleGroup;
}

QHBoxLayout *UserWindow::createLikes() {
    QLabel *title=new QLabel("Likes", this);
    this->tutorialLikesInput = new QLineEdit(this);
    QHBoxLayout *TitleGroup = new QHBoxLayout;
    TitleGroup->addWidget(title);
    TitleGroup->addWidget(tutorialLikesInput);

    return TitleGroup;
}

QHBoxLayout *UserWindow::createButtonsTutorials() {
    QHBoxLayout *buttons = new QHBoxLayout;
    this->addTutorialToWatchList = new QPushButton("Add tutorial to watch list", this);
    this->nextTutorial = new QPushButton("Show next tutorial",this);
    this->exportWatchlist = new QPushButton("Export your watchlist", this);

    buttons->addWidget(addTutorialToWatchList);
    buttons->addWidget(nextTutorial);
    buttons->addWidget(exportWatchlist);

    return buttons;
}

QVBoxLayout *UserWindow::createWatchListInfo(QHBoxLayout *filterLayout, QVBoxLayout *deletedTutorialLayout) {
    QVBoxLayout *tutorialInfo = new QVBoxLayout;
    QHBoxLayout *buttons = createButtonsTutorials();
    QHBoxLayout *title = createTitle();
    QHBoxLayout *presenter = createPresenter();
    QHBoxLayout *link = createLink();
    QHBoxLayout *minutes = createMinutes();
    QHBoxLayout *seconds = createSeconds();
    QHBoxLayout *likes = createLikes();

    tutorialInfo->setSpacing(20);
    tutorialInfo->addLayout(filterLayout);
    tutorialInfo->addLayout(title);
    tutorialInfo->addLayout(presenter);
    tutorialInfo->addLayout(link);
    tutorialInfo->addLayout(minutes);
    tutorialInfo->addLayout(seconds);
    tutorialInfo->addLayout(likes);
    tutorialInfo->addLayout(buttons);
    tutorialInfo->addLayout(deletedTutorialLayout);

    viewWatchlistButton = new QPushButton("View Watch List", this);
    tutorialInfo->addWidget(viewWatchlistButton);

    return tutorialInfo;
}

UserWindow::UserWindow(Service &service, QWidget *parent)
        : QWidget(parent), service(service), watchListWindow(nullptr) {
    QHBoxLayout *secondLayout = new QHBoxLayout();
    QVBoxLayout *firstHalf = new QVBoxLayout;
    QHBoxLayout *comboBox = createComboBoxLayout();
    QHBoxLayout *filter = createPresenterLayout();
    QVBoxLayout *deleted = createDeleteFromWatchList();

    QVBoxLayout *watchListInfo = createWatchListInfo(filter, deleted);

    firstHalf->addLayout(comboBox);
    secondLayout->addLayout(firstHalf);
    secondLayout->addLayout(watchListInfo);

    this->setLayout(secondLayout);
    this->connectSignalsAndSlots();
}

void UserWindow::connectSignalsAndSlots() {
    QObject::connect(this->filterTutorialByPresenter, &QPushButton::clicked, this, &UserWindow::filterTutorialsByPresenterHandler);
    QObject::connect(this->deleteTutorialFromWatchlistButton, &QPushButton::clicked, this, &UserWindow::deleteTutorialFromWatchlistByTitleHandler);
    QObject::connect(this->addTutorialToWatchList, &QPushButton::clicked, this, &UserWindow::addTutorialToWatchListHandler);
    QObject::connect(this->nextTutorial, &QPushButton::clicked, this, &UserWindow::nextTutorialHandler);
    QObject::connect(this->exportWatchlist, &QPushButton::clicked, this, &UserWindow::exportWatchlistHandler);
    QObject::connect(this->watchFileType, QOverload<int>::of(&QComboBox::activated), this, &UserWindow::watchlistFileTypeHandler);
    QObject::connect(this->giveLikeAndDeleteTutorial, &QPushButton::clicked, this, &UserWindow::likeTutorialAndDeleteIt);
    QObject::connect(this->viewWatchlistButton, &QPushButton::clicked, this, &UserWindow::openWatchListWindow);
}

void UserWindow::filterTutorialsByPresenterHandler() {
    this->tutorialTitleInput->setText("");
    this->tutorialPresenterInput->setText("");
    this->tutorialLinkInput->setText("");
    this->tutorialMinutesInput->setText("");
    this->tutorialSecondsInput->setText("");
    this->tutorialLikesInput->setText("");

    QString presenter = this->tutorialPresenterInputFilter->text();
    if(presenter.isEmpty())
        return;
    auto v=this->service.getTutorialsByPresenter(presenter.toStdString());
    if(v.empty())
        QMessageBox::information(this, "Info", "No tutorials found");
    else {
        Tutorial t=this->service.getCurrentTutorialToBeDisplayed();
        this->tutorialTitleInput->setText(QString::fromStdString(t.getTitle()));
        this->tutorialPresenterInput->setText(QString::fromStdString(t.getPresenter()));
        this->tutorialLinkInput->setText(QString::fromStdString(t.getLink()));
        this->tutorialMinutesInput->setText(QString::number(t.getMinutes()));
        this->tutorialSecondsInput->setText(QString::number(t.getSeconds()));
        this->tutorialLikesInput->setText(QString::number(t.getLikes()));
    }
}

void UserWindow::deleteTutorialFromWatchlistByTitleHandler() {
    int index = this->indexOfTutorialInWatchlist->text().toInt();
    if(index < 0 || index >= this->service.getWatchList().size()) {
        QMessageBox::critical(this, "Error", "Invalid index");
        return;
    }
    this->service.removeWatchList(index);
    if (watchListWindow) {
        watchListWindow->refreshWatchList();
    }
}

void UserWindow::addTutorialToWatchListHandler() {
    if(this->tutorialTitleInput->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "You have to select a tutorial first!");
        return;
    }
    Tutorial t = this->service.getCurrentTutorialToBeDisplayed();
    this->service.addToWatchList(t);
    QMessageBox::information(this, "Info", "Tutorial added to watchlist");
    if (watchListWindow) {
        watchListWindow->refreshWatchList();
    }
}

void UserWindow::nextTutorialHandler() {
    if(this->tutorialTitleInput->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "You have to select a tutorial first!");
        return;
    }
    this->service.incrementCurrentTutorialIndex();
    Tutorial t = this->service.getCurrentTutorialToBeDisplayed();
    this->tutorialTitleInput->setText(QString::fromStdString(t.getTitle()));
    this->tutorialPresenterInput->setText(QString::fromStdString(t.getPresenter()));
    this->tutorialLinkInput->setText(QString::fromStdString(t.getLink()));
    this->tutorialMinutesInput->setText(QString::number(t.getMinutes()));
    this->tutorialSecondsInput->setText(QString::number(t.getSeconds()));
    this->tutorialLikesInput->setText(QString::number(t.getLikes()));
}

void UserWindow::exportWatchlistHandler() {
    try {
        this->service.displaylist();
    } catch(FileException &e) {
        QMessageBox::critical(this, "Error", e.what());
        return;
    }
}

void UserWindow::watchlistFileTypeHandler() {
    if(this->watchFileType->currentText().toStdString() == "CSV")
        this->service.setTypeOfWatchList(new CSVWatchList{"watchlist.csv"});
    else if(this->watchFileType->currentText().toStdString() == "HTML")
        this->service.setTypeOfWatchList(new HTMLWatchList{"watchlist.html"});
}

void UserWindow::likeTutorialAndDeleteIt() {
    int index = this->indexOfTutorialInWatchlist->text().toInt();
    if(index < 0 || index >= this->service.getWatchList().size()) {
        QMessageBox::critical(this, "Error", "Invalid index");
        return;
    }
    this->service.increaseLikesTutorial(this->service.getWatchList()[index]);
    this->service.removeWatchList(index);
    if (watchListWindow) {
        watchListWindow->refreshWatchList();
    }
}

void UserWindow::openWatchListWindow() {
    //qDebug() << "Opening Watch List Window...";
    if (!watchListWindow) {
        watchListWindow = new WatchListWindow(service, nullptr);  //  nullptr to make it a top-level window
        //qDebug() << "Watch List Window created.";
    }
    watchListWindow->show();
    watchListWindow->refreshWatchList();
    //qDebug() << "Watch List Window shown.";
}