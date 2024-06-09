//
// Created by Mara Ielciu on 20.04.2024.
//

#include "UI.h"

void UI::print_menu() {
    cout<<"Welcome\n";
    cout<<"0. Exit\n";
    cout<<"1. Add an analysis\n";
    cout<<"2. Show all analyses\n";
    cout<<"3. Check if you are ill\n";
    cout<<"4. Save to a file your monthly report\n";
    cout<<"Choose an option: ";
}

UI::UI() {
    this->p=Person("Ana");
    this->p.startingAnalysis();
}

UI::~UI() {}

void UI::add() {
    cout<<"What analysis would you like to add? (BMI/BP)\n";
    cout<<"Option: ";
    string s;
    cin>>s;
    if(s=="BMI")
    {
        string date; int value;
        cout<<"Enter the date (yyyy.mm.dd): ";
        cin>>date;
        cout<<"Enter the value: ";
        cin>>value;
        BMI* b=new BMI(date,value);
        this->p.addAnalysis(b);
        cout<<"Analyses added!\n";
    }
    else
        if(s=="BP")
        {
            string date; int value1,value2;
            cout<<"Enter the date (yyyy.mm.dd): ";
            cin>>date;
            cout<<"Enter the systolic value: ";
            cin>>value1;
            cout<<"Enter the diastolic value: ";
            cin>>value2;
            BP* b=new BP(date,value1,value2);
            this->p.addAnalysis(b);
            cout<<"Analyses added!\n";
        }
        else
            cout<<"Invalid type of analysis";

}

void UI::printAll() {
    for(auto a:this->p.getAllAnalyses())
        cout<<a->toString()<<"\n";
}

void UI::isIll() {
    int month;
    cout<<"Please provide the month (1-12): ";
    cin>>month;
    bool ok=this->p.isIll(month);
    if(ok)
        cout<<"You are healthy!\n";
    else
        cout<<"You are not healthy\n";
}

void UI::writeToFileM() {
    string filename,date1,date2;
    cout<<"Please enter the filename: ";
    cin>>filename;
    cout<<"Enter the first date: ";
    cin>>date1;
    cout<<"Enter the second date: ";
    cin>>date2;
    try{
        this->p.writeToFile(filename,date1,date2);
    }
    catch (exception&)
    {cout<<"Not ok";}
}

void UI::run() {
    int option;
    while(true)
    {
        this->print_menu();
        cin>>option;
        if(option==0)
            break;
        else if(option==1)
            this->add();
        else if(option==2)
            this->printAll();
        else if(option==3)
            this->isIll();
        else if(option==4)
            this->writeToFileM();
        else
            cout<<"Invalid option!\n";
    }
}


