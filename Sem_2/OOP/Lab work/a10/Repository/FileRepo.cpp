//
// Created by Mara Ielciu on 11.04.2024.
//

#include "FileRepo.h"
#include "Exceptions.h"
#include <fstream>

FileRepo::FileRepo(const std::string &filename): file_name{filename}{

}

FileRepo::~FileRepo() {
    this->writeToFile();
}

void FileRepo::writeToFile() {
    std::ofstream file(this->file_name);
    if(!file.is_open())
        throw FileException("The file could not be opened!");
    for(auto &tutorial: this->tutorials)
        file << tutorial<<'\n';
    file.close();
}

void FileRepo::readFromFile() {
    std::ifstream file(this->file_name);
    if(!file.is_open())
        throw FileException("The file could not be opened!");
    Tutorial tutorial;
    while(file >> tutorial)
        this->addRepo(tutorial);
}

void FileRepo::addRepo(const Tutorial &tutorial) {
    Repo::addRepo(tutorial);
    this->writeToFile();
}

void FileRepo::deleteRepo(const Tutorial &tutorial) {
    Repo::deleteRepo(tutorial);
    this->writeToFile();
}

void FileRepo::updateRepo(int position, const Tutorial &tutorial) {
    Repo::updateRepo(position, tutorial);
    this->writeToFile();
}

void FileRepo::deleteRepoPosition(int position) {
    Repo::deleteRepoPosition(position);
    this->writeToFile();
}
