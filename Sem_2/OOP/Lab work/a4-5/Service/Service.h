//
// Created by Mara Ielciu on 24.03.2024.
//

#ifndef OOP_A4_5_MARAIELCIU1_SERVICE_H
#define OOP_A4_5_MARAIELCIU1_SERVICE_H
#pragma once
#include "../Repository/Repo.h"


class Service {
private:
    Repo repository;
    int adminMode;
    DynamicArray<int> indexesOfLikedTutorials;
public:
    Service();
    ~Service();
    int getAdminMode() const;
    void generateTutorials();
    bool addService(Tutorial tutorial);
    bool deleteService(Tutorial tutorial);
    bool updateService(int position, Tutorial tutorial);
    bool deleteServicePosition(int position);
    DynamicArray<Tutorial> getAllService() const;
    int getSizeService() const;
    int returnPositionService(const Tutorial& tutorial);
    Tutorial getServiceElement(int position);
    void setAdminMode(int mode);
    DynamicArray<int> getPositionsTutByPresenter(const std::string &presenter);
    DynamicArray<int> getIndexesOfLikedTutorials();
    void openTutorialInBrowser(int position);
    void addToWatchList(int position);
    void removeWatchList(int position);
    void increaseLikesByPos(int position);
};


#endif //OOP_A4_5_MARAIELCIU1_SERVICE_H
