import pickle
import os
from src.repository.memory_repo import MemoryRepository
from src.repository.memory_repo import RepositoryException


class BinaryRepository(MemoryRepository):
    """
    This class stores the expenses in a binary file.
    """

    def __init__(self, path_file):
        super().__init__()
        self.__path_file = path_file
        self.load_expense(self. filename())

    def filename(self):
        return self.__path_file

    def load_expense(self, filename):
        """
        This function loads the expenses from the binary file.
        :return:
        """
        if not os.path.exists(self.__path_file):
            return []
        with open(self.__path_file, "rb") as f:
            while True:
                try:
                    expenses_new = pickle.load(f)
                    self._expenses.append(expenses_new)
                except EOFError as er:
                    raise ValueError(str(er))
        # try:
        #     with open(filename, 'rb') as file:
        #         if file.readable() and len(file.read()) == 0:
        #             raise RepositoryException("File is empty.")
        #         # file.seek(0)
        #         self._expenses = pickle.load(file)
        # except (pickle.UnpicklingError, FileNotFoundError) as err:
        #     raise RepositoryException(f"Error loading from file: {err}")

    def _save_expense(self):
        """
        This function saves the expenses in the binary file.
        :return:
        """
        with open(self.__path_file, "wb") as f:
            pickle.dump(self.get_all(), f)

    def add(self, new_expense, undo_state: bool = False):
        """
        This function adds an expense to the repository.
        :param new_expense:
        :param undo_state:
        :return:
        """
        super().add(new_expense, undo_state)
        self._save_expense()

    def filter(self, given_amount):
        """
        This function filters the expenses from the repository.
        :param given_amount:
        :return:
        """
        super().filter(given_amount)
        self._save_expense()

    def undo(self):
        """
        This function undoes the last operation that modified program data.
        :return:
        """
        super().undo()
        self._save_expense()
