//
// Created by Mara Ielciu on 21.04.2024.
//

#include "UI.h"

void UI::printMenu() {
    cout<<"1. Add dwelling\n";
    cout<<"2. Remove client\n";
    cout<<"3. Print all\n";
    cout<<"4. Write to file\n";
    cout<<"0. Exit\n";
}

void UI::addDwelling() {
    double price;
    bool isProfitable;
    cout<<"Enter the price: ";
    cin>>price;
    cout<<"Is the dwelling profitable? (1/0)";
    cin>>isProfitable;
    Dwelling d=this->agency.addDwelling(price, isProfitable);
    vector<Client*> v=this->agency.getInterestedClients(d);
    for(auto c:v)
        cout<<c->toString()<<endl;
}

void UI::removeClient() {
    string name;
    cout<<"Enter the name of the client you want to remove: ";
    cin>>name;
    this->agency.removeClient(name);
}

void UI::writeToFile() {
    string filename;
    cout<<"Enter the filename: ";
    cin>>filename;
    this->agency.writeToFile(filename);
}

void UI::run() {
    int option=0;
    while(1){
        this->printMenu();
        cout<<"Enter your option: ";
        cin>>option;
        switch(option){
            case 1:
                this->addDwelling();
                break;
            case 2:
                this->removeClient();
                break;
            case 3:
                this->printAll();
                break;
            case 4:
                this->writeToFile();
                break;
            case 0:
                return;
        }

    }
}

void UI::printAll() {
    for(auto c:this->agency.getClients())
        cout<<c->toString()<<endl;
    for(auto d:this->agency.getDwellings())
        cout<<d.toString()<<endl;
}
