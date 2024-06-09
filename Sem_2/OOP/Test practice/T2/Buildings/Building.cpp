//
// Created by Mara Ielciu on 22.04.2024.
//

#include "Building.h"

bool Block::mustBeRestored() {
    return (2024 - constructionYear) > 40&&(4/5*totalApartments<=occupiedApartments);
}

bool Block::canBeDemolished() {
    return 1/20*totalApartments>occupiedApartments;
}

string Block::toString() {
    return "Block with nrofap: "+ to_string(this->totalApartments)+" si nrocap: "+ to_string(this->occupiedApartments)+Building::toString();
}

bool House::mustBeRestored() {
    return (2024 - constructionYear) > 100;
}

bool House::canBeDemolished() {
    return !this->isHistorical;
}

string House::toString() {
    if(this->isHistorical)
        return "Historical House "+Building::toString();
    return "Non historical house "+Building::toString();
}
