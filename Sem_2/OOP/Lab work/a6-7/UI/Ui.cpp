//
// Created by Mara Ielciu on 24.03.2024.
//

#include "Ui.h"
#include "FileRepo.h"
#include "CSVWatchList.h"
#include "HTMLWatchList.h"
#include "TutorialValidator.h"
#include "DBRepo.h"
#include <iostream>
#include <string>

Ui::Ui() {
    //this->service = Service();
    //this->service.generateTutorials();
    //this->service.setTypeOfRepo(new FileRepo("/Users/maraielciu/Desktop/project-oop/oop-a6-7-maraielciu1/tutorial.txt"));
    this->chooseRepoType();
    this->chooseWLtype();
}

Ui::~Ui() {

}

void Ui::chooseRepoType() {

    while(true) {
        std::cout << "Choose the type of repository you want to use:\n";
        std::cout << "1. File\n";
        std::cout << "2. Database\n";
        int option;
        std::cin >> option;
        if (option == 1)
        {
            try {
                this->service.setTypeOfRepo(new FileRepo("/Users/maraielciu/Desktop/project-oop/oop-a6-7-maraielciu1/tutorial.txt"));
                return;
            } catch (RepositoryException& e) {
                std::cout<< e.what() << std::endl;
            }

        }
        else if (option == 2)
        {
            try {
                this->service.setTypeOfRepo(new DBRepo("/Users/maraielciu/Desktop/project-oop/oop-a6-7-maraielciu1/tutorials.db"));
                return;
            } catch (RepositoryException& e) {
                std::cerr << "Database error: " << e.what() << std::endl;
                // Handle the failure appropriately, perhaps by allowing the user to retry or choose a different repo type
            }
        }
        else
            std::cout << "Invalid option\n";
    }
}

void Ui::chooseWLtype() {
    std::cout << "Choose the type of watchlist you want to use:\n";
    std::cout << "1. CSV\n";
    std::cout << "2. HTML\n";
    int option;
    while(true) {
        std::cin >> option;
        if (option == 1)
        {
            this->service.setTypeOfWatchList(
                    new CSVWatchList("/Users/maraielciu/Desktop/project-oop/oop-a6-7-maraielciu1/watchlist.csv"));
            return;
        }
        else if (option == 2)
        {
            this->service.setTypeOfWatchList(
                    new HTMLWatchList("/Users/maraielciu/Desktop/project-oop/oop-a6-7-maraielciu1/watchlist.html"));
            return;
        }
        else
            std::cout << "Invalid option\n";
    }
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
    std::cout << "5. Enter user mode\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

void Ui::printMenuUser() {
    std::cout << "User mode\n";
    std::cout << "0. Exit\n";
    std::cout << "1. Print tutorials from a presenter\n";
    std::cout << "2. See watchlist\n";
    std::cout << "3. Delete from watchlist\n";
    std::cout << "4. Enter admin mode\n";
    std::cout << "Choose an option: ";
}

void Ui::printAllTutorials() {
    std::vector<Tutorial> arr = this->service.getAllService();
    for(int i=0;i<this->service.getSizeService();i++)
        std::cout << arr[i].getTitle() << " " << arr[i].getPresenter() << " " << arr[i].getLink() << " " << arr[i].getMinutes() << " " << arr[i].getSeconds() << " " << arr[i].getLikes() << "\n";
}

void Ui::addTutorial() {
    std::cout << "Enter the data of the tutorial you want to add! The format is important!\nThe format is: title, presenter, link, minutes, seconds, number of likes\n";
    std::cin.ignore();
    Tutorial t;
    try
    {
        std::cin >> t;
    }
    catch (TutorialExceptions &errorsCaught)
    {
        std::cout << "There were some errors, try again!\n";
        for (const auto &errorCaught : errorsCaught.returnErrors())
            std::cout << errorCaught.what() << "\n";
        return;
    }
    try
    {
        this->service.addService(t);
        std::cout << "Tutorial added\n";
    }
    catch (DuplicateTutException &caughtException)
    {
        std::cout << caughtException.what() << "\n";
        return;
    }
    catch (RepositoryException &caughtException)
    {
        std::cout << caughtException.what() << "\n";
        return;
    }
}

void Ui::deleteTutorial(){
    this->printAllTutorials();
    int position;
    std::cout << "Enter the position of the tutorial you want to delete: ";
    std::cin >> position;
    try{
        this->service.deleteServicePosition(position);
        std::cout << "Tutorial deleted\n";
    }
    catch(std::exception&){
        std::cout << "Invalid position\n";
    }
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
    catch(TutorialExceptions &ex){
        std::cout << "There were some errors, try again!\n";
        for(const auto &errorCaught: ex.returnErrors())
            std::cout << errorCaught.what() << "\n";
        return;
    }
    try {
        this->service.updateService(position, tutorial);
        std::cout << "Tutorial updated\n";
    }
    catch (InvalidPositionException &caughtException)
    {
        std::cout << caughtException.what() << "\n";
        return;
    }
    catch (DuplicateTutException &caughtException)
    {
        std::cout << caughtException.what() << "\n";
        return;
    }
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
            else if(option==5)
                this->service.setAdminMode(0);
            else
                std::cout << "Invalid option\n";
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
            else if(option==4)
                this->service.setAdminMode(1);
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
    std::vector<Tutorial> arr = this->service.getTutorialsByPresenter(presenter);
    if(arr.empty())
    {
        std::cout<<"There are no tutorials by that presenter\n";
        return;
    }
    bool ok=true;
    while(ok) {
        ok = false;
        for (auto tut: arr) {
            try {
                this->service.checkIfTutorialIsAlreadyInTheList(tut);
                ok = true;
                std::cout << "Title: " << tut.getTitle() << "\n";
                std::cout << "Presenter: " << tut.getPresenter() << "\n";
                std::cout << "Link: " << tut.getLink() << "\n";
                std::cout << "Minutes: " << tut.getMinutes() << "\n";
                std::cout << "Seconds: " << tut.getSeconds() << "\n";
                std::cout << "Number of likes: " << tut.getLikes() << "\n";
                this->service.openTutorialInBrowser(this->service.returnPositionService(tut));
                std::cout << "Do you want to add the tutorial to your WATCH LIST? (yes/no): ";
                std::string choiceOfUser;
                std::cin >> choiceOfUser;
                if (choiceOfUser == "yes")
                    this->service.addToWatchList(tut);
                else
                    std::cout << "Tutorial not added to your WATCH LIST!\n";
                std::cout << "Do you want to see the next tutorial? (yes/no): ";
                std::cin >> choiceOfUser;
                if (choiceOfUser == "no") {
                    std::cout << "That's it! You can still search for other movies!\n";
                    return;
                }
            }
            catch (DuplicateTutException &caughtException) {
                std::cout << caughtException.what() << "\n";
            }

        }
    }
    std::cout << "Done!\n";
}

void Ui::seeWatchList() {
    try{
        this->service.displaylist();
    }
    catch(FileException &caughtException){
        std::cout << caughtException.what() << "\n";
    }
}

void Ui::deleteFromWatchList() {
    this->seeWatchList();
    std::vector<Tutorial> arr = this->service.getWatchList();
    if(arr.empty())
    {
        std::cout<<"There are no tutorials in the watchlist\n";
        return;
    }
    int position;
    std::cout << "Enter the position of the tutorial you want to delete: \n";
    std::cin >> position;
    std::cout << "Was the tutorial helpful? (yes/no): ";
    std::string choiceOfUser;
    std::cin >> choiceOfUser;
    if (choiceOfUser == "yes")
    {
        try{
            this->service.increaseLikesTutorial(arr[position]);
            std::cout << "Number of likes increased successfully!\n";
        }
        catch(InvalidPositionException &caughtException){
            std::cout << caughtException.what() << "\n";
            return;
        }

    }
    this->service.removeWatchList(position);
    std::cout << "Tutorial deleted successfully!\n";
}




