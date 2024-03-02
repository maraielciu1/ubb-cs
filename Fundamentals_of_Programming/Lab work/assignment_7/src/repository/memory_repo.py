from src.domain.expense import Expense
class RepositoryException(Exception):
    """
    This class implements the exceptions for the repository.
    """
    pass

class MemoryRepository:
    """
    This class stores expenses in memory.
    """
    def __init__(self):
        self.__expenses = []
        self.__history = []
    def _save_history(self):
        """
        This function saves the current state of the repository.
        :return:
        """
        self.__history.append(self.__expenses.copy())
    def add(self,new_expense: Expense,undo_state: bool = False):
        """
        This function adds an expense to the repository.
        :param new_expense:
        :param undo_state:
        :return:
        """
        if new_expense.day < 1 or new_expense.day > 30:
            raise RepositoryException("Invalid day!")
        elif new_expense.amount < 0:
            raise RepositoryException("Invalid amount!")
        elif new_expense.type == "":
            raise RepositoryException("Invalid type!")
        elif new_expense in self.__expenses:
            raise RepositoryException("Expense already exists!")
        elif undo_state:
            self._save_history()
        self.__expenses.append(new_expense)
    def get_all(self):
        """
        This function returns all the expenses from the repository.
        :return:
        """
        return self.__expenses.copy()
    def filter(self,given_amount:int):
        """
        This function filters the expenses from the repository.
        :param given_amount:
        :return:
        """
        if given_amount < 0:
            raise RepositoryException("Invalid amount!")
        self._save_history()
        self.__expenses = [expense for expense in self.__expenses if expense.amount > given_amount]
    def undo(self):
        """
        This function undoes the last operation that modified program data.
        :return:
        """
        if len(self.__history) == 0:
            raise RepositoryException("No more undo!")
        self.__expenses = self.__history.pop()