from src.actions.add.discipline import AddDisciplineCommand
from src.actions.remove.discipline import RemoveDisciplineCommand
from src.actions.remove.grade import RemoveGradeCommand
from src.actions.update.discipline import UpdateDisciplineCommand
from src.domain.discipline import Discipline
from src.repository.memory_repo import MemoryRepository
from src.repository.repository import RepositoryError, Repository


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
            add_discipline_command = AddDisciplineCommand(
                self.__disc_repo, discipline)
            add_discipline_command.execute()
            self.__disc_repo.record_undo_commands([add_discipline_command])
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
            remove_discipline_command = RemoveDisciplineCommand(
                self.__disc_repo, discipline_id)
            remove_discipline_command.execute()
            undo_commands = [remove_discipline_command]

            discipline_grades = self.__disc_repo.get_grades_by_discipline(
                discipline_id)
            for grade in discipline_grades:
                remove_grade_command = RemoveGradeCommand(
                    self.__disc_repo, grade.id)
                remove_grade_command.execute()
                undo_commands.append(remove_grade_command)

            self.__disc_repo.record_undo_commands(undo_commands)
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
            update_discipline_command = UpdateDisciplineCommand(
                self.__disc_repo, discipline_id, new_name)
            update_discipline_command.execute()
            self.__disc_repo.record_undo_commands([update_discipline_command])
        except RepositoryError as re:
            response_data["success"] = False
            response_data["message"] = str(re)
            return response_data
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

    def find_discipline_by_id(self, discipline_id):
        """
        Returns the discipline with the given id
        :param discipline_id:
        :return:
        """
        response_data = {"success": True,
                         "message": "Discipline found successfully!",
                         "data": None}
        try:
            response_data["data"] = self.__disc_repo.find_discipline_by_id(
                discipline_id)
        except RepositoryError as re:
            response_data["success"] = False
            response_data["message"] = str(re)
        return response_data

    @property
    def add(self) -> callable:
        return self.add_discipline
