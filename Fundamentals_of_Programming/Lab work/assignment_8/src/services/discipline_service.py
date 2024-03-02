from src.repository.memory_repo import MemoryRepository, RepositoryError
from src.domain.discipline import Discipline


class DiscServ:
    def __init__(self, disc_repo: MemoryRepository):
        self.__disc_repo = disc_repo

    def add_discipline(self, discipline_id, discipline_name):
        """
        Adds a discipline to the repository
        :return:
        """
        response_data = {"success": True,
                         "message": "Discipline added successfully!"}
        try:
            discipline = Discipline(discipline_id, discipline_name)
        except TypeError as te:
            response_data["success"] = False
            response_data["message"] = str(te)
            return response_data
        try:
            self.__disc_repo.add_discipline(discipline)
        except RepositoryError as re:
            response_data["success"] = False
            response_data["message"] = str(re)
            return response_data
        return response_data

    def remove_discipline(self, discipline_id):
        """
        Removes a discipline from the repository
        :param discipline_id:
        :return:
        """
        response_data = {"success": True,
                         "message": f"Discipline with ID {discipline_id} removed successfully!"}
        try:
            self.__disc_repo.remove_discipline(discipline_id)
        except RepositoryError as re:
            response_data["success"] = False
            response_data["message"] = str(re)
            return response_data

        return response_data

    def update_discipline(self, discipline_id, new_name):
        """
        Updates a discipline from the repository
        :param discipline_id:
        :param new_name:
        :return:
        """
        response_data = {"success": True,
                         "message": f"Discipline with ID {discipline_id} updated successfully!"}
        try:
            self.__disc_repo.update_discipline(discipline_id, new_name)
        except RepositoryError as re:
            response_data["success"] = False
            response_data["message"] = str(re)
            return response_data
        self.__disc_repo.update_discipline(discipline_id, new_name)
        return response_data

    def list_disciplines(self):
        """
        Returns the list of disciplines
        :return:
        """
        response_data = {"success": True,
                         "message": "Disciplines listed successfully!",
                         "data": self.__disc_repo.list_disciplines()}
        return response_data

    def find_disciplines(self, query):
        """
        Returns the list of disciplines
        :return:
        """
        response_data = {"success": True,
                         "message": "Disciplines listed successfully!",
                         "data": self.__disc_repo.find_disciplines(query)}
        return response_data
