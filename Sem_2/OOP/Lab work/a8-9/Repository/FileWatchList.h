//
// Created by Mara Ielciu on 19.04.2024.
//

#ifndef OOP_A6_7_MARAIELCIU1_FILEWATCHLIST_H
#define OOP_A6_7_MARAIELCIU1_FILEWATCHLIST_H

#pragma once
#include "WatchList.h"

class FileWatchList: public WatchList{
protected:
    std::string filename;
public:
    FileWatchList(const std::string &fileName) : filename{fileName} {}
    virtual void writeToFile() const = 0;
    void openInApp() const;
    virtual ~FileWatchList(){};
};


#endif //OOP_A6_7_MARAIELCIU1_FILEWATCHLIST_H
