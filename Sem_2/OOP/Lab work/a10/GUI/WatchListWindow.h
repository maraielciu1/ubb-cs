//
// Created by Mara Ielciu on 03.06.2024.
//

#ifndef OOP_A10_MARAIELCIU1_WATCHLISTWINDOW_H
#define OOP_A10_MARAIELCIU1_WATCHLISTWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include "Service.h"
#include "WatchListModel.h"

class WatchListWindow : public QWidget {
Q_OBJECT

private:
    Service &service;
    QTableView *watchListView;
    WatchListModel *model;

public:
    WatchListWindow(Service &service, QWidget *parent = nullptr);
    ~WatchListWindow() {};
    void refreshWatchList();

};


#endif //OOP_A10_MARAIELCIU1_WATCHLISTWINDOW_H
