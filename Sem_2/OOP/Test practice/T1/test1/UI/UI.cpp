//
// Created by Mara Ielciu on 02.04.2024.
//

#include "UI.h"

UI::UI() {
    this->service=Service();
    this->service.generatePlayers();
}

UI::~UI() {

}

void UI::printMenu() {
    std::cout << "MENU\n";
    std::cout << "0. Exit\n";
    std::cout << "1. Add player\n";
    std::cout << "2. Display players\n";
    std:: cout << "3. Display players from team\n";
    std::cout << "Enter the option: \n";
}

void UI::addPlayer() {
    std::string name, nationality, team;
    int nr_goals;

    std::cout<<"Enter name: \n";
    std::cin>>name;
    std::cout<<"Enter nationality: \n";
    std::cin>>nationality;
    std::cout<<"Enter team: \n";
    std::cin>>team;
    std::cout<<"Enter number of goals: \n";
    std::cin>>nr_goals;
    Player p{name,nationality,team,nr_goals};
    try{
        p.validatePlayer();
    }
    catch (std::exception&)
    {
        std::cout << "Invalid player\n";
        return;
    }
    if(this->service.addPlayerService(p))
        std::cout<<"Player added\n";
    else
        std::cout<<"Player already exists\n";
}

void UI::run() {
    int option;
    while(1){
        this->printMenu();
        std::cin>>option;
        if(option==0)
        {
            std::cout << "Bye!";
            break;
        }
        else if(option == 1)
            this->addPlayer();
        else if(option==2)
            this->print_all();
        else if(option==3)
            this->print_sorted();
        else
            std::cout<<"Invalid option\n";
    }
}

void UI::print_all() {
    Player* p=this->service.getAll();
    for(int i=0;i<this->service.getSizeServ();i++)
        std::cout<<p[i].str()<<'\n';
}

void UI::print_sorted() {
    std::string team;
    std::cout<<"Enter the team name: \n";
    std::cin>>team;
    int nr_total=0;
    DynamicArray<Player> arr=this->service.getTeamS(team,nr_total);
    std::cout<<"Total number of goals: "<<nr_total<<'\n';
    if(arr.getSizeArray()==0)
        std::cout<<"There are no players in this team\n";
    for(int i=0;i<arr.getSizeArray();i++)
        std::cout<<arr[i].str()<<'\n';
}
