//
// Created by Mara Ielciu on 22.04.2024.
//

#include "UI.h"

void UI::printMenu() {
    std::cout<<"1. add new appliance\n";
    std::cout<<"2. print all\n";
    std::cout<<"3. print less\n";
    std::cout<<"4. write to file\n";
    std::cout<<"0. exit\n";
}

void UI::add() {
    std::cout<<"enter the type(dishwasher/refrigerator): ";
    std::string type,id;
    double weight;
    std::cin>>type;
    std::cout<<"enter the id: ";
    std::cin>>id;
    std::cout<<"enter the weight: ";
    std::cin>>weight;
    if(type=="refrigerator")
    {
        std::string c;
        bool freezer;
        std::cout<<"electricity usage class(A,A+,A++): ";
        std::cin>>c;
        std::cout<<"does it have a freezer?(yes/no): ";
        std::string s;
        std::cin>>s;
        if(s=="yes")
            freezer= true;
        else
            freezer=false;
        Refrigerator* r=new Refrigerator(id,weight,c, freezer);
        this->service.addAp(r);
    }
    else
    {
        double w,c;
        std::cout<<"washing length: ";
        std::cin>>w;
        std::cout<<"consumed energy: ";
        std::cin>>c;
        DishWasher* d=new DishWasher(id,weight,w,c);
        this->service.addAp(d);
    }
}

void UI::printAll() {
    for(auto a:this->service.getAll())
        std::cout<<a->toString()<<"\n";
}

void UI::printLess() {
    double el;
    std::cout<<"Consumed electricity less than: ";
    std::cin>>el;
    for(auto a: this->service.getLess(el))
        std::cout<<a->toString()<<"\n";
}

void UI::writeToFile() {
    double el;
    std::cout<<"Consumed electricity more than: ";
    std::cin>>el;
    std::string filename;
    std::cout<<"filename: ";
    std::cin>>filename;
    this->service.writeToFile(filename,el);
}

void UI::run() {
    while (1)
    {
        int option;
        this->printMenu();
        std::cout<<"option: ";
        std::cin>>option;
        if(option==0)
            break;
        switch (option) {
            case 1:
                this->add();
                break;
            case 2:
                this->printAll();
                break;
            case 3:
                this->printLess();
                break;
            case 4:
                this->writeToFile();
                break;
            default:
                std::cout<<"invalid option\n";
        }
    }
}
