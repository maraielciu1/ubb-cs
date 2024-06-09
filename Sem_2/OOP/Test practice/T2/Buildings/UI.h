//
// Created by Mara Ielciu on 22.04.2024.
//

#ifndef BUILDINGS_UI_H
#define BUILDINGS_UI_H


#include "Service.h"

class UI {
private:
    Service service;
public:
    void run(){
        while(true){
            cout << "1. Add building" << endl;
            cout << "2. Show all buildings" << endl;
            cout << "3. Show buildings to be restored" << endl;
            cout << "4. Write buildings to file" << endl;
            cout << "0. Exit" << endl;
            int command;
            cin >> command;
            cin.ignore();
            if(command == 0)
                break;
            if(command == 1){
                cout << "Address: ";
                string address;
                getline(cin, address);
                cout << "Construction year: ";
                int constructionYear;
                cin >> constructionYear;
                cin.ignore();
                cout << "Type (1-House, 2-Block): ";
                int type;
                cin >> type;
                cin.ignore();
                if(type == 1){
                    cout << "Historical (1-yes, 0-no): ";
                    bool historical;
                    cin >> historical;
                    cin.ignore();
                    service.addBuilding(new House(address, constructionYear, historical));
                }
                else{
                    cout << "Total apartments: ";
                    int totalApartments;
                    cin >> totalApartments;
                    cin.ignore();
                    cout << "Occupied apartments: ";
                    int occupiedApartments;
                    cin >> occupiedApartments;
                    cin.ignore();
                    service.addBuilding(new Block(address, constructionYear, totalApartments, occupiedApartments));
                }
            }
            if(command == 2){
                for(auto &building: service.getBuildings())
                    cout << building->toString() << endl;
            }
            if(command == 3){
                int year;
                cout << "Year: ";
                cin >> year;
                for(auto &building: service.getBuildingsToBeDemolished())
                    if(2024-building->getConstructionYear() < year)
                    cout << building->toString() << endl;
            }
            if(command == 4){
                cout << "Filename for restored: ";
                string filename1,filename2;
                cin>>filename1;
                cout << "Filename for demolished: ";
                cin>>filename2;
                service.writeToFile(filename1, service.getBuildingsToBeRestored());
                service.writeToFile(filename2, service.getBuildingsToBeDemolished());

            }
        }
    };
};


#endif //BUILDINGS_UI_H
