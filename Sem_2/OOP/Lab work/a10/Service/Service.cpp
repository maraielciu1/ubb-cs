//
// Created by Mara Ielciu on 24.03.2024.
//

#include "Service.h"

Service::Service() {
    this->adminMode = 0;
    this->repository = nullptr;
    this->watchList = nullptr;
}

Service::~Service() {
    delete this->repository;
    delete this->watchList;
}

void Service::addService(Tutorial tutorial) {
    if(this->repository->validatePosition(this->repository->returnPosition(tutorial)))
        throw DuplicateTutException();
    this->repository->addRepo(tutorial);
    std::unique_ptr<Action> a=std::make_unique<ActionAdd>(tutorial, *this->repository);
    this->undoStack.push_back(std::move(a));
}

void Service::deleteService(Tutorial tutorial) {
    if(!this->repository->validatePosition(this->repository->returnPosition(tutorial)))
        throw InvalidPositionException();
    this->repository->deleteRepo(tutorial);
    std::unique_ptr<Action> a=std::make_unique<ActionRemove>(tutorial, *this->repository);
    this->undoStack.push_back(std::move(a));
}

void Service::updateService(int position, Tutorial tutorial, Tutorial oldTutorial) {
    if(!this->repository->validatePosition(position))
        throw InvalidPositionException();
    this->repository->updateRepo(position, tutorial);
    std::unique_ptr<Action> a=std::make_unique<ActionUpdate>(oldTutorial, tutorial, *this->repository);
    this->undoStack.push_back(std::move(a));
}

void Service::deleteServicePosition(int position) {
    if(!this->repository->validatePosition(position))
        throw InvalidPositionException();
    Tutorial t=this->repository->getRepoElement(position);
    this->repository->deleteRepoPosition(position);
    std::unique_ptr<Action> a=std::make_unique<ActionRemove>(t, *this->repository);
    this->undoStack.push_back(std::move(a));
}

std::vector<Tutorial> Service::getAllService() const {
    return this->repository->getAllRepo();
}

int Service::getSizeService() const {
    return this->repository->getSizeRepo();
}

int Service::returnPositionService(const Tutorial &tutorial) {
    return this->repository->returnPosition(tutorial);
}

int Service::getAdminMode() const {
    return this->adminMode;
}

void Service::generateTutorials() {
    Tutorial t1{"Tutorial_for_beginners","Mosh", "https://www.youtube.com/watch?v=ZzaPdXTrSb8", 100, 1, 100};
    Tutorial t2{"Tutorial_cpp","Mosh", "https://youtu.be/gfkTfcpWqAY?si=UpMH2dlLmrzd3a6F", 100, 2, 200};
    Tutorial t3{"Namespaces_cpp","Neso", "https://youtu.be/ZZWTh142s4w?si=WBW3io0OSOyS2cSv", 12, 3, 300};
    Tutorial t4{"While_cpp","Neso", "https://youtu.be/iF4i423144E?si=zJP_fTU14CWKB6bG", 30, 4, 400};
    Tutorial t5{"For_cpp","Neso", "https://youtu.be/yS8DUrQy_ow?si=9MRuW3a55b6Fw-MN", 10, 5, 500};
    Tutorial t6{"Characters_cpp","Neso", "https://youtu.be/sb0EE68Yk0I?si=BkbXo4cUOO_mMIG2", 20, 6, 600};
    Tutorial t7{"Vectors_cpp","Neso", "https://youtu.be/H1cK1QXw6Zs?si=3wsjc_uGXdoJNV4E", 33, 7, 700};
    Tutorial t8{"Arrays_cpp","Tim", "https://youtu.be/1FVBeLD_FdE?si=aV9wgddL6yiR9VbV", 21, 8, 800};
    Tutorial t9{"If_cpp","Tim", "https://youtu.be/Zkr0aAdD1Ww?si=NN1GwUlHQuMIYQEM", 17, 9, 900};
    Tutorial t10{"DynamicArray","Eric", "https://youtu.be/MwwbgqG6bSk?si=jQyVzHwm4KoljgXK", 12, 10, 1000};
    this->addService(t1);
    this->addService(t2);
    this->addService(t3);
    this->addService(t4);
    this->addService(t5);
    this->addService(t6);
    this->addService(t7);
    this->addService(t8);
    this->addService(t9);
    this->addService(t10);
}

Tutorial Service::getServiceElement(int position) {
    if(!this->repository->validatePosition(position))
        throw InvalidPositionException();
    return this->repository->getRepoElement(position);

}

void Service::setAdminMode(int mode) {
    this->adminMode = mode;

}

std::vector<int> Service::getPositionsTutByPresenter(const std::string &presenter) {
    std::vector<int> positions;
    for(int i=0;i<this->repository->getSizeRepo();i++)
        if(this->repository->getRepoElement(i).getPresenter()==presenter || presenter.empty())
            positions.push_back(i);
    return positions;
}

std::vector<int> Service::getIndexesOfLikedTutorials() {
    return this->indexesOfLikedTutorials;
}

void Service::openTutorialInBrowser(int position) {
    Tutorial t=this->getServiceElement(position);
    std::string command = "open "+t.getLink();
    system(command.c_str());
}

void Service::addToWatchList(Tutorial tut) {
    this->watchList->addTutorial(tut);
}

void Service::removeWatchList(int pos) {
    this->watchList->removeTutorialFromPlaylist(pos);
}

void Service::increaseLikesTutorial(Tutorial &t){
    int pos=this->repository->returnPosition(t);
    if(pos!=-1)
        this->repository->modifyLikesPosition(pos,1);
}

void Service::setTypeOfRepo(Repo *r) {
    this->repository=r;
    this->repository->readFromFile();
}

void Service::setTypeOfWatchList(FileWatchList *w) {
    this->watchList=w;
}

void Service::displaylist() {
    this->watchList->openInApp();
}

void Service::checkIfTutorialIsAlreadyInTheList(const Tutorial &t) {
    this->watchList->checkIfTutorialIsAlreadyInTheList(t);
}

void Service::clearFile(const std::string &f) {
    std::ofstream file(f);
    file.close();
}

std::vector<Tutorial> Service::getTutorialsByPresenter(const std::string &presenter) {
    this->tutorialsFilteredByPresenter.clear();
    this->currentTutorialIndex=0;
    for(auto tut: this->repository->getAllRepo())
        if(tut.getPresenter()==presenter || presenter.empty())
            tutorialsFilteredByPresenter.push_back(tut);
    return tutorialsFilteredByPresenter;
}

std::vector<Tutorial> Service::getWatchList() {
    return this->watchList->getTutorials();
}

Tutorial Service::getCurrentTutorialToBeDisplayed() {
    this->openLinkOfTheCurrentTutorial();
    return this->tutorialsFilteredByPresenter[this->currentTutorialIndex];
}

void Service::incrementCurrentTutorialIndex() {
    this->currentTutorialIndex++;
    if(this->currentTutorialIndex==this->watchList->getTutorials().size())
        this->currentTutorialIndex=0;

}

void Service::openLinkOfTheCurrentTutorial() {
    Tutorial t=this->tutorialsFilteredByPresenter[this->currentTutorialIndex];
    std::string command = "open "+t.getLink();
    system(command.c_str());
}

std::map<std::string, int> Service::getMapOfTutorialsByPresenter() {
    std::map<std::string, int> mapOfTutorials;
    for (const auto &tut: this->repository->getAllRepo()) {
        std::string presenter = tut.getPresenter();
        if (mapOfTutorials.find(presenter) == mapOfTutorials.end())
            mapOfTutorials[presenter] = 1;
        else
            ++mapOfTutorials[presenter];
    }
    return mapOfTutorials;
}

void Service::undo() {
    if(this->undoStack.empty())
        throw RepositoryException("No more undos!");
    std::unique_ptr<Action> a=std::move(this->undoStack.back());
    a->executeUndo();
    this->undoStack.pop_back();
    this->redoStack.push_back(std::move(a));
}

void Service::redo() {
    if(this->redoStack.empty())
        throw RepositoryException("No more redos!");
    std::unique_ptr<Action> a=std::move(this->redoStack.back());
    a->executeRedo();
    this->redoStack.pop_back();
    this->undoStack.push_back(std::move(a));
}

void Service::clearRedoStack() {
    this->redoStack.clear();
}



