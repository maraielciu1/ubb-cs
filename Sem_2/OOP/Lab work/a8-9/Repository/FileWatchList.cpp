//
// Created by Mara Ielciu on 19.04.2024.
//

#include "FileWatchList.h"

void FileWatchList::openInApp() const {
    this->writeToFile();
    std::string cmd = "open " + this->filename;
    system(cmd.c_str());
}
