//
// Created by Mara Ielciu on 24.03.2024.
//

#include "Ui.h"
#include <iostream>
#include <string>

Ui::Ui() {
    this->service = Service();
    this->service.generateTutorials();
}

Ui::~Ui() {

}

void Ui::printMenu() {
    std::cout << "Welcome to Master c++\n";
    std::cout << "1. Admin mode\n";
    std::cout << "2. User mode\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

void Ui::printMenuAdmin() {
    std::cout << "Admin mode\n";
    std::cout << "1. Add tutorial\n";
    std::cout << "2. Delete tutorial\n";
    std::cout << "3. Update tutorial\n";
    std::cout << "4. Print all tutorials\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

void Ui::printMenuUser() {
    std::cout << "User mode\n";
    std::cout << "0. Exit\n";
    std::cout << "1. Print tutorials from a presenter\n";
    std::cout << "2. See watchlist\n";
    std::cout << "3. Delete from watchlist\n";
    std::cout << "Choose an option: ";
}

void Ui::printAllTutorials() {
    DynamicArray<Tutorial> arr = this->service.getAllService();
    for(int i=0;i<this->service.getSizeService();i++)
        std::cout << arr.getElementArray(i).getTitle() << " " << arr.getElementArray(i).getPresenter() << " " << arr.getElementArray(i).getLink() << " " << arr.getElementArray(i).getMinutes() << " " << arr.getElementArray(i).getSeconds() << " " << arr.getElementArray(i).getLikes() << "\n";
}

void Ui::addTutorial() {
    std::string title, presenter, link;
    int minutes,seconds, number_of_likes;
    std::cout << "Title: ";
    std::cin >> title;
    std::cout << "Presenter: ";
    std::cin >> presenter;
    std::cout << "Link: ";
    std::cin >> link;
    std::cout << "Minutes: ";
    std::cin >> minutes;
    std::cout << "Seconds: ";
    std::cin >> seconds;
    std::cout << "Number of likes: ";
    std::cin >> number_of_likes;
    Tutorial tutorial = Tutorial(title, presenter, link, minutes, seconds, number_of_likes);
    try{
        tutorial.validateTutorial();
    }
    catch(std::exception&){
        std::cout << "Invalid tutorial\n";
        return;
    }
    if(this->service.addService(tutorial))
        std::cout << "Tutorial added\n";
    else
        std::cout << "Tutorial already exists\n";
}

void Ui::deleteTutorial(){
    this->printAllTutorials();
    int position;
    std::cout << "Enter the position of the tutorial you want to delete: ";
    std::cin >> position;
    if(this->service.deleteServicePosition(position))
        std::cout << "Tutorial deleted\n";
    else
        std::cout << "Invalid position\n";
}

void Ui::updateTutorial() {
    this->printAllTutorials();
    int position;
    std::cout << "Enter the position of the tutorial you want to update: ";
    std::cin >> position;
    std::string title, presenter, link;
    int minutes,seconds, number_of_likes;
    std::cout << "Title: ";
    std::cin >> title;
    std::cout << "Presenter: ";
    std::cin >> presenter;
    std::cout << "Link: ";
    std::cin >> link;
    std::cout << "Minutes: ";
    std::cin >> minutes;
    std::cout << "Seconds: ";
    std::cin >> seconds;
    std::cout << "Number of likes: ";
    std::cin >> number_of_likes;
    Tutorial tutorial = Tutorial(title, presenter, link, minutes, seconds, number_of_likes);
    try{
        tutorial.validateTutorial();
    }
    catch(std::exception&){
        std::cout << "Invalid tutorial\n";
        return;
    }
    if(this->service.updateService(position, tutorial))
        std::cout << "Tutorial updated\n";
    else
        std::cout << "Invalid position\n";
}

void Ui::run() {
    int option=0;
    this->printMenu();
    std::cin >> option;
    if(option==0) {
        std::cout << "Goodbye\n";
        return;
    }
    else if(option==1)
        this->service.setAdminMode(1);
    else if(option==2)
        this->service.setAdminMode(0);
    while(true){
        if(this->service.getAdminMode()==1){
            this->printMenuAdmin();
            std::cin >> option;
            if(option==0){
                std::cout << "Goodbye\n";
                break;
            }
            else if(option==1)
                this->addTutorial();
            else if(option==2)
                this->deleteTutorial();
            else if(option==3)
                this->updateTutorial();
            else if(option==4)
                this->printAllTutorials();
        }
        else{
            this->printMenuUser();
            std::cin >> option;
            if(option==0){
                std::cout << "Goodbye\n";
                break;
            }
            else if(option==1)
                this->getTutorialByPresenter();
            else if(option==2)
                this->seeWatchList();
            else if(option==3)
                this->deleteFromWatchList();
            else
                std::cout << "Invalid option\n";
        }
    }

}

void Ui::getTutorialByPresenter() {
    std::string presenter;
    std::cin.ignore();
    std::cout << "Please enter the presenter of the tutorial you want to see: ";
    std::getline(std::cin, presenter);
    DynamicArray<int> arr = this->service.getPositionsTutByPresenter(presenter);
    if(arr.getSizeArray()==0)
    {
        std::cout<<"There are no tutorials by that presenter\n";
        return;
    }
    int current_index=0;
    while(arr.getSizeArray())
    {
        Tutorial t=this->service.getServiceElement(arr.getElementArray(current_index));
        std::cout << "Title: " << t.getTitle() << '\n';
        std::cout << "Presenter: " << t.getPresenter() << '\n';
        std::cout << "Link: " << t.getLink() << '\n';
        std::cout << "Duration: " << t.getMinutes() << "minutes and " << t.getSeconds() << "seconds\n";
        std::cout << "Number of likes: " << t.getLikes() << '\n';
        this->service.openTutorialInBrowser(arr.getElementArray(current_index));

        std::cout << "Do you want to add the tutorial on your watch list? (yes/no)\n";
        std::string choice;
        std::cin >> choice;
        if(choice=="yes")
        {
            this->service.addToWatchList(arr.getElementArray(current_index));
            std::cout << "Movie added to your wishlist!\n";
            arr.deleteElementArray(current_index);
            --current_index;
        }
        else if(choice=="no")
             std::cout << "Tutorial not added to watch list\n";
        else
            std::cout<<"Invalid option\n";

        std::cout << "Do you want to see the next movie? (yes/no): ";
        std::cin >> choice;
        if (choice == "yes")
        {
            ++current_index;
            if (current_index >= arr.getSizeArray())
                current_index = 0;
        }
        else
            break;
    }
    std::cout << "Done!\n";
}

void Ui::seeWatchList() {
    DynamicArray<int> arr = this->service.getIndexesOfLikedTutorials();
    if (arr.getSizeArray() == 0)
    {
        std::cout << "There are no movies in your wishlist!\n";
        return;
    }
    for (int index = 0; index < arr.getSizeArray(); ++index)
    {
        Tutorial t = this->service.getServiceElement(arr.getElementArray(index));
        std::cout << index <<".";
        std::cout << "Title: " << t.getTitle() << '\n';
        std::cout << "Presenter: " << t.getPresenter() << '\n';
        std::cout << "Link: " << t.getLink() << '\n';
        std::cout << "Duration: " << t.getMinutes() << "minutes and " << t.getSeconds() << "seconds\n";
        std::cout << "Number of likes: " << t.getLikes() << '\n';
    }
}

void Ui::deleteFromWatchList() {
    DynamicArray<int> arr = this->service.getIndexesOfLikedTutorials();
    if (arr.getSizeArray() == 0)
    {
        std::cout << "There are no movies in your wishlist!\n";
        return;
    }
    for (int index = 0; index < arr.getSizeArray(); ++index)
    {
        Tutorial t = this->service.getServiceElement(arr.getElementArray(index));
        std::cout << index <<".";
        std::cout << "Title: " << t.getTitle() << '\n';
        std::cout << "Presenter: " << t.getPresenter() << '\n';
        std::cout << "Link: " << t.getLink() << '\n';
        std::cout << "Duration: " << t.getMinutes() << "minutes and " << t.getSeconds() << "seconds\n";
        std::cout << "Number of likes: " << t.getLikes() << '\n';
    }
    int position;
    std::cout << "Enter the position of the tutorial you want to delete: \n";
    std::cin >> position;
    std::cout << "Was the tutorial helpful? (yes/no): ";
    std::string choiceOfUser;
    std::cin >> choiceOfUser;
    if (choiceOfUser == "yes")
    {
        this->service.increaseLikesByPos(arr.getElementArray(position));
        std::cout << "Number of likes increased successfully!\n";
    }
    this->service.removeWatchList(position);
    std::cout << "Movie deleted successfully!\n";
}



