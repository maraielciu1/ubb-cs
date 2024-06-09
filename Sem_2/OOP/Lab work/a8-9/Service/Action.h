//
// Created by Mara Ielciu on 22.05.2024.
//

#ifndef OOP_A8_9_MARAIELCIU1_ACTION_H
#define OOP_A8_9_MARAIELCIU1_ACTION_H


#include "Tutorial.h"
#include "Repo.h"

class Action {
public:
    Action(){};
    virtual ~Action(){};
    virtual void executeUndo()=0;
    virtual void executeRedo()=0;
};

class ActionAdd: public Action{
private:
    Tutorial addedTutorial;
    Repo &repo;
public:
    ActionAdd(Tutorial tutorial, Repo &repository_n): addedTutorial{tutorial}, repo{repository_n}{};
    ~ActionAdd(){};
    void executeUndo() override;
    void executeRedo() override;
};

class ActionRemove: public Action{
private:
    Tutorial deletedTutorial;
    Repo& repo;
public:
    ActionRemove(Tutorial tutorial, Repo& repository_n): deletedTutorial{tutorial}, repo{repository_n}{};
    ~ActionRemove(){};
    void executeUndo() override;
    void executeRedo() override;

};


class ActionUpdate: public Action{
private:
    Tutorial oldTutorial;
    Tutorial newTutorial;
    Repo& repo;
public:
    ActionUpdate(Tutorial otutorial,Tutorial ntutorial, Repo& repository_n): oldTutorial{otutorial},newTutorial{ntutorial}, repo{repository_n}{};
    ~ActionUpdate(){};
    void executeUndo() override;
    void executeRedo() override;

};

#endif //OOP_A8_9_MARAIELCIU1_ACTION_H
