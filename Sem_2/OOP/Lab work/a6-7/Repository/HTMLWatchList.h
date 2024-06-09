//
// Created by Mara Ielciu on 19.04.2024.
//

#ifndef OOP_A6_7_MARAIELCIU1_HTMLWATCHLIST_H
#define OOP_A6_7_MARAIELCIU1_HTMLWATCHLIST_H

#pragma once
#include "FileWatchList.h"
#include <fstream>

class HTMLWatchList: public FileWatchList{
public:
    HTMLWatchList(const std::string &fileName) : FileWatchList{fileName} {}
    void writeToFile() const override;
};


#endif //OOP_A6_7_MARAIELCIU1_HTMLWATCHLIST_H
