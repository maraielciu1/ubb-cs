#ifndef OOP_A8_9_MARAIELCIU1_USERWINDOW_H
#define OOP_A8_9_MARAIELCIU1_USERWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include "Service.h"
#include "WatchListWindow.h"

class UserWindow : public QWidget {
Q_OBJECT

private:
    Service &service;
    WatchListWindow *watchListWindow;  // Member for WatchListWindow
    QPushButton *filterTutorialByPresenter,
            *deleteTutorialFromWatchlistButton,
            *giveLikeAndDeleteTutorial,
            *addTutorialToWatchList,
            *nextTutorial,
            *exportWatchlist,
            *viewWatchlistButton;  // Button to open WatchListWindow
    QLineEdit *tutorialPresenterInputFilter,
            *tutorialTitleInput,
            *tutorialPresenterInput,
            *tutorialLinkInput,
            *tutorialMinutesInput,
            *tutorialSecondsInput,
            *tutorialLikesInput,
            *indexOfTutorialInWatchlist;
    QComboBox *watchFileType;

public:
    UserWindow(Service &service, QWidget *parent = nullptr);
    ~UserWindow() {};
    QHBoxLayout *createComboBoxLayout();
    void createFilterSearch(QHBoxLayout *presenterLayout);
    QHBoxLayout *createPresenterLayout();
    QVBoxLayout *createDeleteFromWatchList();

    QHBoxLayout *createTitle();
    QHBoxLayout *createPresenter();
    QHBoxLayout *createLink();
    QHBoxLayout *createMinutes();
    QHBoxLayout *createSeconds();
    QHBoxLayout *createLikes();
    QHBoxLayout *createTutorialLayout(QHBoxLayout *layout);
    QHBoxLayout *createButtonsTutorials();
    QVBoxLayout *createWatchListInfo(QHBoxLayout *filterLayout, QVBoxLayout *deletedTutorialLayout);

    void connectSignalsAndSlots();

public slots:
    void filterTutorialsByPresenterHandler();
    void deleteTutorialFromWatchlistByTitleHandler();
    void addTutorialToWatchListHandler();
    void nextTutorialHandler();
    void exportWatchlistHandler();
    void watchlistFileTypeHandler();
    void likeTutorialAndDeleteIt();
    void openWatchListWindow();
};

#endif //OOP_A8_9_MARAIELCIU1_USERWINDOW_H
