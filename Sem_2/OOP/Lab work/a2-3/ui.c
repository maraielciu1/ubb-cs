//
// Created by Mara Ielciu on 15.03.2024.
//

#include "Repo/Repository.h"
#include "UndoRedo/UndoRedoRepo.h"
#include "Domain/Domain.h"
#include "Domain/DynamicArray.h"
#include "Service/Service.h"
#include <stdio.h>
#include <string.h>
#include "Tests/Test.h"


void start(Service* s)
{
    Date d1 = createDate(11,6,2024);
    Date d2 = createDate(12,6,2024);
    Date d3 = createDate(10,3,2024);
    Date d4 = createDate(10,3,2024);

    Material* m1 = createMaterial("flour", "abc", 10,d1);
    Material* m2 = createMaterial("sugar", "def", 20,d1);
    Material* m3 = createMaterial("salt", "ghi", 30,d2);
    Material* m4 = createMaterial("eggs", "abc", 4,d1);
    Material* m5 = createMaterial("milk", "abc", 2,d3);
    Material* m6 = createMaterial("oil", "abc", 10,d4);
    Material* m7 = createMaterial("seeds", "mmm", 2,d2);
    Material* m8 = createMaterial("flour", "lala", 10,d4);
    Material* m9 = createMaterial("sugar", "lala", 20,d4);
    Material* m10 = createMaterial("salt", "mmm", 10,d2);

    addMaterialService(s,m1);
    addMaterialService(s,m2);
    addMaterialService(s,m3);
    addMaterialService(s,m4);
    addMaterialService(s,m5);
    addMaterialService(s,m6);
    addMaterialService(s,m7);
    addMaterialService(s,m8);
    addMaterialService(s,m9);
    addMaterialService(s,m10);

}

void print_menu()
{
    printf("\n--- Bread'n Bagel Material Management System ---\n");
    printf("1. Add a new material\n");
    printf("2. Update an existing material\n");
    printf("3. Delete a material\n");
    printf("4. List materials past their expiration date\n");
    printf("5. List all materials\n");
    printf("6. List all materials for a supplier, sorted ascending by quantity\n");
    printf("7. List all materials for a supplier, sorted descending by quantity\n");
    printf("8. Undo\n");
    printf("9. Redo\n");
    printf("10. Bonus sort\n");
    printf("11. Exit\n");
    printf("Select an option: ");
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {} // Clear up to the end of the line or EOF
}

int readInt(const char* prompt) {
    int value;
    printf("%s", prompt);
    while (scanf("%d", &value) != 1) {
        clearInputBuffer(); // Clear the invalid input
        printf("Invalid input. Please enter a valid number.\n");
        printf("%s", prompt);
    }
    clearInputBuffer(); // Clear the newline left by scanf
    return value;
}

void readString(const char* prompt, char* str, int size) {
    printf("%s", prompt);
    fgets(str, size, stdin);
    if (str[0] == '\n') { // Handle empty input immediately after previous input
        fgets(str, size, stdin);
    }
    str[strcspn(str, "\n")] = 0; // Remove newline character
}


int main()
{
    //testAll();
    Repository* repo = createRepo();
    int typeRepo = 2;
    UndoRedoRepo* undoRedo = createUndoRedoRepo(typeRepo);
    Service* s = createService(repo,undoRedo,typeRepo);
    start(s);

    char searchString[20];
    int running = 1;
    do {
        print_menu();
        int option = readInt("Select an option: ");

        switch (option) {
            case 1: {
                char name[20], supplier[20];
                int quantity, day, month, year;

                readString("Enter the name of the material: ", name, sizeof(name));
                readString("Enter the supplier of the material: ", supplier, sizeof(supplier));
                quantity = readInt("Enter the quantity of the material: ");
                printf("Enter the expiration date of the material\n");
                day = readInt("Day: ");
                month = readInt("Month: ");
                year = readInt("Year: ");

                Date exp_date = createDate(day, month, year);
                Material* m = createMaterial(name, supplier, quantity, exp_date);
                int result = addMaterialService(s, m);
                if(result == 1)
                    printf("Material added successfully.\n");
                else
                    printf("Material already exists.\n");
                break;
            }
            case 2:
            {
                char name[20];
                char supplier[20];
                int quantity;
                int day, month, year;
                printf("Enter the name of the material: ");
                fgets(name, 20, stdin);
                name[strlen(name) - 1] = '\0'; // Remove newline character from input
                printf("Enter the supplier of the material: ");
                fgets(supplier, 20, stdin);
                supplier[strlen(supplier) - 1] = '\0'; // Remove newline character from input
                printf("Enter the quantity of the material: ");
                do{
                    scanf("%d", &quantity);
                    if(quantity<0)
                        printf("Invalid quantity. Please try again.\n");
                }while(quantity<0);
                printf("Enter the expiration date of the material (day month year): ");

                do{
                    scanf("%d %d %d", &day, &month, &year);
                    if(day<0||month<0||year<0||day>31||month>12||year<2024)
                        printf("Invalid date. Please try again.\n");
                }while(day<0||month<0||year<0||day>31||month>12||year<2024);
                Date exp_date = createDate(day, month, year);
                Material* old_m = createMaterial(name, supplier, quantity, exp_date);


                char nname[20];
                char nsupplier[20];
                int nquantity;
                int nday, nmonth, nyear;
                printf("Enter the new name of the material: ");
                fgets(nname, 20, stdin);
                name[strlen(nname) - 1] = '\0'; // Remove newline character from input
                printf("Enter the new supplier of the material: ");
                fgets(nsupplier, 20, stdin);
                supplier[strlen(nsupplier) - 1] = '\0'; // Remove newline character from input
                printf("Enter the new quantity of the material: ");
                do{
                    scanf("%d", &nquantity);
                    if(nquantity<0)
                        printf("Invalid quantity. Please try again.\n");
                }while(nquantity<0);
                printf("Enter the expiration date of the material (day month year): ");

                do{
                    scanf("%d %d %d", &nday, &nmonth, &nyear);
                    if(nday<0||nmonth<0||nyear<0||nday>31||nmonth>12||nyear<2024)
                        printf("Invalid date. Please try again.\n");
                }while(nday<0||nmonth<0||nyear<0||nday>31||nmonth>12||nyear<2024);
                exp_date = createDate(nday, nmonth, nyear);
                Material* new_m = createMaterial(name, supplier, quantity, exp_date);

                int result=updateMaterialService(s, old_m, new_m);
                if(result==1)
                    printf("Material updated successfully.\n");
                else
                    printf("Material does not exist.\n");
                break;
            }

            case 3:
            {
                char name[20];
                char supplier[20];
                int quantity;
                int day, month, year;
                printf("Enter the name of the material: ");
                fgets(name, 20, stdin);
                name[strlen(name) - 1] = '\0'; // Remove newline character from input
                printf("Enter the supplier of the material: ");
                fgets(supplier, 20, stdin);
                supplier[strlen(supplier) - 1] = '\0'; // Remove newline character from input
                printf("Enter the quantity of the material: ");
                do{
                    scanf("%d", &quantity);
                    if(quantity<0)
                        printf("Invalid quantity. Please try again.\n");
                }while(quantity<0);
                printf("Enter the expiration date of the material (day month year): ");

                do{
                    scanf("%d %d %d", &day, &month, &year);
                    if(day<0||month<0||year<0||day>31||month>12||year<2024)
                        printf("Invalid date. Please try again.\n");
                }while(day<0||month<0||year<0||day>31||month>12||year<2024);
                Date exp_date = createDate(day, month, year);
                Material* m = createMaterial(name, supplier, quantity, exp_date);
                int result=deleteMaterialService(s, m);
                if(result==1)
                    printf("Material deleted successfully.\n");
                else
                    printf("Material does not exist.\n");
                break;
            }

            case 4:
            {
                printf("Enter a search string: ");
                fgets(searchString, sizeof(searchString), stdin);
                // Check if the first character is newline immediately after fgets
                if (searchString[0] == '\n') {
                    searchString[0] = '\0'; // Set as empty string if only enter was pressed
                } else {
                    searchString[strcspn(searchString, "\n")] = 0; // Remove newline char
                }

                DynamicArray* arr = pastExpirationDate(s, searchString);
                if (getLengthOfDynamicArray(arr) == 0) {
                    printf("No materials found for the given supplier and quantity.\n");
                    break;
                }
                for (int i = 0; i < getLengthOfDynamicArray(arr); i++) {
                    Material* m = getElementOnPosition(arr, i);
                    printf("%s %s %d %d %d %d\n", getName(m), getSupplier(m), getQuantity(m), getExpDate(m).day, getExpDate(m).month, getExpDate(m).year);
                }
                //destroyDynamicArray(arr);
                break;
            }

            case 5:
            {
                DynamicArray* arr =get_all_service(s);
                for (int i = 0; i < getLengthOfDynamicArray(arr); i++) {
                    Material* m = getElementOnPosition(arr,i);
                    printf("%s %s %d %d %d %d\n", getName(m), getSupplier(m), getQuantity(m), getExpDate(m).day, getExpDate(m).month, getExpDate(m).year);
                }
                break;
            }
            case 6:
            {
                printf("Enter a supplier name: ");
                scanf("%s", searchString);
                printf("Enter quantity: ");
                int quantity;
                do {
                    scanf("%d", &quantity);
                    if (quantity < 0)
                        printf("Invalid quantity. Please try again.\n");
                } while (quantity < 0);

                DynamicArray* arr = returnAscending(s, searchString, quantity);
                if (getLengthOfDynamicArray(arr) == 0) {
                    printf("No materials found for the given supplier and quantity.\n");
                    break;
                }
                if (getLengthOfDynamicArray(arr) == 0) {
                    printf("No materials found for the given supplier and quantity.\n");
                    break;
                }
                for (int i = 0; i < getLengthOfDynamicArray(arr); i++) {
                    Material* m = getElementOnPosition(arr,i);
                    printf("%s %s %d %d %d %d\n", getName(m), getSupplier(m), getQuantity(m), getExpDate(m).day, getExpDate(m).month, getExpDate(m).year);
                }
                //destroyDynamicArray(arr);
                break;
            }

            case 7:
            {
                printf("Enter a supplier name: ");
                scanf("%s", searchString);
                printf("Enter quantity: ");
                int quantity;
                scanf("%d", &quantity);
                DynamicArray* arr = returnDescending(s, searchString, quantity);
                if (getLengthOfDynamicArray(arr) == 0) {
                    printf("No materials found for the given supplier and quantity.\n");
                    break;
                }
                for (int i = 0; i < getLengthOfDynamicArray(arr); i++) {
                    Material* m = getElementOnPosition(arr, i);
                    printf("%s %s %d %d %d %d\n", getName(m), getSupplier(m), getQuantity(m), getExpDate(m).day, getExpDate(m).month, getExpDate(m).year);
                }
                //destroyDynamicArray(arr);
                break;
            }
            case 8:
            {
                UndoRedoRepo *undoRedoRepository = getUndoRedo(s);
                DynamicArray *undoOperations = getUndoOperations(undoRedoRepository), *redoOperations = getRedoOperations(undoRedoRepository);
                if (undoRedoService(s, undoOperations, redoOperations))
                    printf("Undo performed successfully!\n");
                else
                    printf("No more undos available!\n");
                break;
            }

            case 9:
            {
                UndoRedoRepo *undoRedoRepository = getUndoRedo(s);
                DynamicArray *undoOperations = getUndoOperations(undoRedoRepository), *redoOperations = getRedoOperations(undoRedoRepository);
                if (undoRedoService(s, redoOperations, undoOperations))
                    printf("Redo performed successfully! \n");
                else
                    printf("No more redos available \n");
                break;
            }
            case 10:
            {
                char supplier[100];
                int quantity;
                int sortOption;

                printf("Enter supplier name: ");
                scanf("%s", supplier);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                printf("Sort ascending (1) or descending (2)?: ");
                scanf("%d", &sortOption);

                DynamicArray* sortedMaterials;
                if (sortOption == 1) {
                    sortedMaterials = sortMaterials(s, supplier, quantity, returnAscending);
                } else
                if (sortOption == 2){
                    sortedMaterials = sortMaterials(s, supplier, quantity, returnDescending);
                }
                else {
                    printf("Invalid sort option. Please try again.\n");
                    break;
                }

                for(int i=0;i<getLengthOfDynamicArray(sortedMaterials);i++)
                {
                    Material* m = getElementOnPosition(sortedMaterials,i);
                    printf("%s %s %d %d %d %d\n", getName(m), getSupplier(m), getQuantity(m), getExpDate(m).day, getExpDate(m).month, getExpDate(m).year);
                }
                break;
            }
            case 11:
                running = 0;
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }while ((getchar()) != '\n');
    }while(running);

    destroyService(s);
    printf("Exiting Bread'n Bagel Material Management System. Goodbye!\n");

    return 0;
}