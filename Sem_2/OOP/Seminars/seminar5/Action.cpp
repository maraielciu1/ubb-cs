//
// Created by Mara Ielciu on 25.04.2024.
//

#include "Action.h"

void ActionAdd::executeUndo() {
    this->repo.removeSong(this->addedSong);
}

void ActionAdd::executeRedo() {
    this->repo.addSong(this->addedSong);
}

void ActionRemove::executeUndo() {
    this->repo.addSong(this->deletedSong);
}

void ActionRemove::executeRedo() {
    this->repo.removeSong(this->deletedSong);
}

void ActionUpdate::executeUndo() {
    this->repo.updateSong(this->newSong, this->oldSong);
}

void ActionUpdate::executeRedo() {
    this->repo.updateSong(this->oldSong, this->newSong);
}
