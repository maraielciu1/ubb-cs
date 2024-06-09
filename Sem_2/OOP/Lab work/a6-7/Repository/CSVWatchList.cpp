//
// Created by Mara Ielciu on 19.04.2024.
//

#include "CSVWatchList.h"

void CSVWatchList::writeToFile() const {
    std::ofstream file(this->filename);
    if (!file.is_open())
        throw FileException("The CSV file could not be opened!");
    file << "ID, Title, Presenter, Link, Minutes, Seconds, Likes\n";
    int id=0;
    for (auto &tutorial: this->tutorials)
        file << id++ << "," << tutorial.getTitle() << "," << tutorial.getPresenter() << "," << tutorial.getLink() << "," << tutorial.getMinutes() << "," << tutorial.getSeconds() << "," << tutorial.getLikes() << "\n";
    file.close();
}
