from src.domain.expense import Expense
from src.repository.memory_repo import RepositoryException
from src.services.functions import Functions
from random import randint, choice


class UI:
    def __init__(self, chosen_repo):
        self._expense_service = Functions(chosen_repo)

    def __print_menu(self):
        print("1. Add expense")
        print("2. Display expenses")
        print("3. Filter expenses")
        print("4. Undo")
        print("5. Exit")

    def __add(self):
        day = int(input("Day: "))
        amount = int(input("Amount: "))
        type = input("Type: ")
        try:
            self._expense_service.add(day, amount, type)
        except RepositoryException as ve:
            print(ve)

    def __display(self):
        if not self._expense_service.get_all():
            print("No expenses!")
            return
        for expense in self._expense_service.get_all():
            print(expense)

    def __filter(self):
        given_amount = int(input("Amount: "))
        try:
            self._expense_service.filter(given_amount)
        except RepositoryException as ve:
            print(ve)

    def __undo(self):
        self._expense_service.undo()

    def run(self):
        commands = {
            "1": self.__add,
            "2": self.__display,
            "3": self.__filter,
            "4": self.__undo
        }
        while True:
            self.__print_menu()
            command = input("Enter command: ")
            if command == "5":
                return
            if command in commands:
                try:
                    commands[command]()
                except RepositoryException as ve:
                    print(ve)
            else:
                print("Invalid command!")


def add_starting():
    initial_expenses = []
    for i in range(10):
        day = randint(1, 30)
        amount = randint(100, 500)
        type = choice(("gas", "water", "electricity", "heating", "other"))
        new_expense = Expense(day, amount, type)
        if new_expense not in initial_expenses:
            initial_expenses.append(new_expense)
        else:
            i -= 1
    return initial_expenses
