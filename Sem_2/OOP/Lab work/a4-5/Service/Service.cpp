//
// Created by Mara Ielciu on 24.03.2024.
//

#include "Service.h"

Service::Service() {
    this->adminMode = 0;
    this->repository = Repo();
}

Service::~Service() {

}

bool Service::addService(Tutorial tutorial) {
    if(this->repository.validatePosition(this->repository.returnPosition(tutorial)))
        return false;
    this->repository.addRepo(tutorial);
    return true;
}

bool Service::deleteService(Tutorial tutorial) {
    if(!this->repository.validatePosition(this->repository.returnPosition(tutorial)))
        return false;
    this->repository.deleteRepo(tutorial);
    return true;
}

bool Service::updateService(int position, Tutorial tutorial) {
    if(!this->repository.validatePosition(position))
        return false;
    this->repository.updateRepo(position, tutorial);
    return true;
}

bool Service::deleteServicePosition(int position) {
    if(!this->repository.validatePosition(position))
        return false;
    this->repository.deleteRepoPosition(position);
    return true;
}

DynamicArray<Tutorial> Service::getAllService() const {
    return this->repository.getAllRepo();
}

int Service::getSizeService() const {
    return this->repository.getSizeRepo();
}

int Service::returnPositionService(const Tutorial &tutorial) {
    return this->repository.returnPosition(tutorial);
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
    if(!this->repository.validatePosition(position))
        throw std::invalid_argument("Invalid position");
    return this->repository.getRepoElement(position);

}

void Service::setAdminMode(int mode) {
    this->adminMode = mode;

}

DynamicArray<int> Service::getPositionsTutByPresenter(const std::string &presenter) {
    DynamicArray<int> positions;
    for(int i=0;i<this->repository.getSizeRepo();i++)
        if(this->repository.getRepoElement(i).getPresenter()==presenter || presenter.empty())
            positions.addElementArray(i);
    return positions;
}

DynamicArray<int> Service::getIndexesOfLikedTutorials() {
    return this->indexesOfLikedTutorials;
}

void Service::openTutorialInBrowser(int position) {
    Tutorial t=this->getServiceElement(position);
    std::string command = "open "+t.getLink();
    system(command.c_str());
}

void Service::addToWatchList(int position) {
    this->indexesOfLikedTutorials.addElementArray(position);
}

void Service::removeWatchList(int position) {
    this->indexesOfLikedTutorials.deleteElementArray(position);
}

void Service::increaseLikesByPos(int position) {
    this->repository.modifyLikesPosition(position,1);
}



