//
// Created by Mara Ielciu on 28.05.2024.
//

#ifndef OOP_A8_9_MARAIELCIU1_WATCHLISTMODEL_H
#define OOP_A8_9_MARAIELCIU1_WATCHLISTMODEL_H
#include <QAbstractTableModel>
#include "Service.h"

class WatchListModel : public QAbstractTableModel {
Q_OBJECT

private:
    Service &service;

public:
    explicit WatchListModel(Service &service, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void refresh();
};


#endif //OOP_A8_9_MARAIELCIU1_WATCHLISTMODEL_H
