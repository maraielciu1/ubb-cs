//
// Created by Mara Ielciu on 21.04.2024.
//

#include "UI.h"

void UI::add() {
    std::string type,name;
    int nrDoctors;
    std::cout<<"Enter the type of department (surgery/neonatal): ";
    std::cin>>type;
    std::cout<<"Enter the name of the hospital: ";
    std::cin>>name;
    std::cout<<"Enter the number of doctors: ";
    std::cin>>nrDoctors;
    if(type=="surgery")
    {
        int nrp;
        std::cout<<"Enter the number of patients: ";
        std::cin>>nrp;
        auto* s=new Surgery(name,nrDoctors,nrp);
        this->service.addDepartment(s);
    }
    else if(type=="neonatal")
    {
        int nrb,nrm;
        double avg;
        std::cout<<"Enter the number of newborns: ";
        std::cin>>nrb;
        std::cout<<"Enter the number of patients: ";
        std::cin>>nrm;
        std::cout<<"Enter the average: ";
        std::cin>>avg;
        auto* n=new NeonatalUnit(name,nrDoctors,nrm,nrb,avg);
        this->service.addDepartment(n);
    }
    else
        std::cout<<"Invalid type\n";
}

void UI::run() {
    int option=0;
    while(1)
    {
        this->printMenu();
        std::cin>>option;
        if(option==1)
            this->add();
        else if(option==2)
            this->printAll();
        else if(option==3)
            this->prntEFF();
        else if(option==4)
            this->writeToFile();
        else if(option==0)
            break;
        else
            std::cout<<"invalid noption\n";
    }

}

void UI::printMenu() {
    std::cout<<"1. Add\n";
    std::cout<<"2. Show all\n";
    std::cout<<"3. Show efficient\n";
    std::cout<<"4. Write to file\n";
    std::cout<<"0. exit\n";
    std::cout<<"choice: ";
}

void UI::printAll() {
    for(auto m:this->service.getAllDepartmets())
        std::cout<<m->toString()<<"\n";
}

void UI::prntEFF() {
    for(auto m:this->service.getEfficientDepartmets())
        std::cout<<m->toString()<<"\n";
}

void UI::writeToFile() {
    std::string filename;
    std::cout<<"Enter the filename: ";
    std::cin>>filename;
    this->service.writeToFile(filename);
}
