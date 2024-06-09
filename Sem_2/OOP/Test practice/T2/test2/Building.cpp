//
// Created by Mara Ielciu on 23.04.2024.
//

#include "Building.h"

std::string Building::toString() {
    return " building from: "+std::to_string(this->constructionYear);
}

bool Block::mustBeRestored() {
    bool ok1=(2024-this->constructionYear)>40;
    bool ok2=this->occupiedApartments>4/5*this->totalApartments;
    return ok1&&ok2;
}

bool Block::canBeDemolished() {
    double percent=(100*double(this->occupiedApartments))/double(this->totalApartments);
    if(percent<=(1/20*double(this->totalApartments)))
        return true;
    return false;
}

std::string Block::toString() {
    return "Block with "+std::to_string(this->totalApartments)+" total apartments and "+std::to_string(this->occupiedApartments)+" occupied aps, "+Building::toString();
}

bool House::canBeDemolished() {
    return !this->isHistorical;
}

bool House::mustBeRestored() {
    return (2024-this->constructionYear)>=100;
}

std::string House::toString() {
    if(this->isHistorical)
        return "Historical house, "+Building::toString();
    return "Non historical house, "+Building::toString();
}
