//
// Created by Mara Ielciu on 25.04.2024.
//

#ifndef PLAYLIST___POLYMOPRHIC_UNDO___STUB_ACTION_H
#define PLAYLIST___POLYMOPRHIC_UNDO___STUB_ACTION_H


#include "Song.h"
#include "Repository.h"

class Action {
public:
    Action(){};
    virtual ~Action(){};
    virtual void executeUndo()=0;
    virtual void executeRedo()=0;
};

class ActionAdd: public Action{
private:
    Song addedSong;
    Repository &repo;
public:
    ActionAdd(Song song, Repository &repository_n): addedSong{song}, repo{repository_n}{};
    ~ActionAdd(){};
    void executeUndo() override;
    void executeRedo() override;
};

class ActionRemove: public Action{
private:
    Song deletedSong;
    Repository& repo;
public:
    ActionRemove(Song song, Repository& repository_n): deletedSong{song}, repo{repository_n}{};
    ~ActionRemove(){};
    void executeUndo() override;
    void executeRedo() override;
};

class ActionUpdate: public Action{
private:
    Song oldSong;
    Song newSong;
    Repository& repo;
public:
    ActionUpdate(Song osong,Song nsong, Repository& repository_n): oldSong{osong},newSong{nsong}, repo{repository_n}{};
    ~ActionUpdate(){};
    void executeUndo() override;
    void executeRedo() override;
};

#endif //PLAYLIST___POLYMOPRHIC_UNDO___STUB_ACTION_H
