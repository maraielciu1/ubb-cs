//
// Created by Mara Ielciu on 19.04.2024.
//

#ifndef OOP_A6_7_MARAIELCIU1_CSVWATCHLIST_H
#define OOP_A6_7_MARAIELCIU1_CSVWATCHLIST_H

#pragma once
#include "FileWatchList.h"
#include <fstream>

class CSVWatchList: public FileWatchList{
public:
    CSVWatchList(const std::string &fileName) : FileWatchList{fileName} {}
    void writeToFile() const override;
};


#endif //OOP_A6_7_MARAIELCIU1_CSVWATCHLIST_H
