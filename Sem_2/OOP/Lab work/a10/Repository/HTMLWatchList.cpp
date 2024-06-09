//
// Created by Mara Ielciu on 19.04.2024.
//

#include "HTMLWatchList.h"

void HTMLWatchList::writeToFile() const {
    std::ofstream f(this->filename);
    if(!f.is_open())
        throw FileException("The file could not be opened!");
    f << "<!DOCTYPE html>\n";
    f << "<html>\n";
    f << "<head>\n";
    f << "<title>Watchlist</title>\n";
    f << "</head>\n";
    f << "<body>\n";
    f << "<table border=\"1\">\n";
    f << "<tr>\n";
    f << "<td>ID</td>\n";
    f << "<td>Title</td>\n";
    f << "<td>Presenter</td>\n";
    f << "<td>Link</td>\n";
    f << "<td>Minutes</td>\n";
    f << "<td>Seconds</td>\n";
    f << "<td>Likes</td>\n";
    f << "</tr>\n";
    int id=0;
    for (auto & m : this->tutorials)
    {
        f << "<tr>\n";
        f << "<td>" << id++ << "</td>\n";
        f << "<td>" << m.getTitle() << "</td>\n";
        f << "<td>" << m.getPresenter() << "</td>\n";
        f << "<td><a href=\"" << m.getLink() << "\">Link</a></td>\n";
        f << "<td>" << m.getMinutes() << "</td>\n";
        f << "<td>" << m.getSeconds() << "</td>\n";
        f << "<td>" << m.getLikes() << "</td>\n";
        f << "</tr>\n";
    }
    f << "</table>\n";
    f << "</body>\n";
    f << "</html>\n";
    f.close();
}
