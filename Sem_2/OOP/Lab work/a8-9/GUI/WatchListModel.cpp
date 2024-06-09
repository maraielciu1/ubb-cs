//
// Created by Mara Ielciu on 28.05.2024.
//

#include "WatchListModel.h"

WatchListModel::WatchListModel(Service &service, QObject *parent)
        : QAbstractTableModel(parent), service(service) {}

int WatchListModel::rowCount(const QModelIndex &parent) const {
    return service.getWatchList().size();
}

int WatchListModel::columnCount(const QModelIndex &parent) const {
    return 6; // Columns for Title, Presenter, Minutes, Seconds, Likes
}

QVariant WatchListModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        const auto& tutorials = service.getWatchList();
        const auto& tutorial = tutorials[index.row()];

        switch (index.column()) {
            case 0:
                return QString::fromStdString(tutorial.getTitle());
            case 1:
                return QString::fromStdString(tutorial.getPresenter());
            case 2:
                return tutorial.getMinutes();
            case 3:
                return tutorial.getSeconds();
            case 4:
                return tutorial.getLikes();
            case 5:
                return QString::fromStdString(tutorial.getLink());
            default:
                return QVariant();
        }
    }
    return QVariant();
}

QVariant WatchListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return "Title";
                case 1:
                    return "Presenter";
                case 2:
                    return "Minutes";
                case 3:
                    return "Seconds";
                case 4:
                    return "Likes";
                case 5:
                    return "Link";
                default:
                    return QVariant();
            }
        }
    }
    return QVariant();
}

void WatchListModel::refresh() {
    beginResetModel();
    endResetModel();
}

