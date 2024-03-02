"""
1. Add an expense. Expense data is read from the console.
2. Display the list of expenses.
3. Filter the list so that it contains only expenses above a certain value read from the console.
4. Undo the last operation that modified program data. This step can be repeated.
The user can undo only those operations made during the current run of the program.

"""
from src.domain.expense import Expense
from random import randint,choice

class Functions:
    """
    This class implements the functions of the program.
    """
    def __init__(self,chosen_repo):
        self.__repo = chosen_repo
        """if len(self.__repo.get_all())<10:
            self.add_starting()"""

    """def add_starting(self):
        
        for i in range(10):
            day = randint(1,30)
            amount = randint(100,500)
            type = choice(("gas","water","electricity","heating","other"))
            if Expense(day,amount,type) not in self.__repo.get_all():
                self.add(day,amount,type,False)
            else:
                i -= 1"""


    def get_all(self):
        """
        This function returns all the expenses from the repository.
        :return: the list of expenses
        """
        return self.__repo.get_all()
    def add(self,expense_day,expense_amount,expense_type,undo_state: bool = True,):
        """
        This function adds an expense to the repository.
        :param expense_day:
        :param expense_amount:
        :param expense_type:
        :param undo_state:
        :return:
        """
        expense = Expense(expense_day, expense_amount, expense_type)
        self.__repo.add(expense, undo_state)
    def filter(self,given_amount):
        """
        This function filters the expenses from the repository.
        """
        return self.__repo.filter(given_amount)
    def undo(self):
        """
        This function undoes the last operation that modified program data.
        """
        return self.__repo.undo()
