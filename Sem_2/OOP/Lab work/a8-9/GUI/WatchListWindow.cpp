#include "WatchListWindow.h"

WatchListWindow::WatchListWindow(Service &service, QWidget *parent)
        : QWidget(parent), service(service) {
    this->setWindowTitle("Watch List");
    QVBoxLayout *layout = new QVBoxLayout(this);

    model = new WatchListModel(service, this);
    watchListView = new QTableView(this);
    watchListView->setModel(model);

    layout->addWidget(watchListView);

    this->setLayout(layout);
}

void WatchListWindow::refreshWatchList() {
    model->refresh();
}