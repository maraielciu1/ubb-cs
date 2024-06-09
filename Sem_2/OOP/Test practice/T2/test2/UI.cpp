//
// Created by Mara Ielciu on 23.04.2024.
//

#include "UI.h"

void UI::printMenu() {
    cout<<"0. Exit\n";
    cout<<"1. Add a new building\n";
    cout<<"2. Show all buildings\n";
    cout<<"3. Show all buildings that can be demolished older than a given year\n";
    cout<<"4. Save to file\n";
    cout<<"option: ";
}

void UI::add() {
    int year;
    string type;
    cout<<"Enter the type of building you want to add (block/house): ";
    cin>>type;
    cout<<"Enter the construction year: ";
    cin>>year;
    if(type=="block")
    {
        int nrap, nroc;
        cout<<"Enter the total number of apartments: ";
        cin>>nrap;
        cout<<"Enter the number of occupied apartments: ";
        cin>>nroc;
        Block* b=new Block(year,nrap,nroc);
        this->service.addBuilding(b);
    }
    else if(type=="house")
    {
        bool historical= false;
        string ok;
        cout<<"Is the house historical? (y/n): ";
        cin>>ok;
        if(ok=="y")
            historical=true;
        House* h=new House(year,historical);
        this->service.addBuilding(h);
    }
    else
        cout<<"Invalid type\n";
}

void UI::printAll() {
    for(auto b: this->service.getAllBuildings())
        cout<<b->toString()<<"\n";
}

void UI::printDemolished() {
    int year;
    cout<<"Enter the year: ";
    cin>>year;
    for(auto b: this->service.getAllToBeDemolished())
        if(b->getYear()<year)
            cout<<b->toString()<<"\n";
}

void UI::writeToFile() {
    string f1,f2;
    cout<<"Enter the file in which you want to see the buildings that need to be restored: ";
    cin>>f1;
    cout<<"Enter the file in which you want to see the buildings that need to be demolished: ";
    cin>>f2;
    this->service.writeToFile(f1,this->service.getAllToBeRestored());
    this->service.writeToFile(f2,this->service.getAllToBeDemolished());
}

void UI::run() {
    int option;
    while(true)
    {
        this->printMenu();
        cin>>option;
        if(option==1)
            this->add();
        else if(option==2)
            this->printAll();
        else if(option==3)
            this->printDemolished();
        else if(option==4)
            this->writeToFile();
        else if(option==0)
            break;
        else
            cout<<"Invalid option\n";
    }
}

