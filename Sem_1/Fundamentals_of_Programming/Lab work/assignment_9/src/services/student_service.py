from src.actions.add.student import AddStudentCommand
from src.actions.remove.grade import RemoveGradeCommand
from src.actions.remove.student import RemoveStudentCommand
from src.actions.update.student import UpdateStudentCommand
from src.domain.student import Student
from src.repository.memory_repo import MemoryRepository
from src.repository.repository import RepositoryError, Repository


class StudServ:
    def __init__(self, stud_repo: MemoryRepository) -> None:
        self.__stud_repo = stud_repo

    def add_student(self, student_id, name):
        """
        Adds a student to the repository
        :return:
        """
        response_data = {"success": True,
                         "message": "Student added successfully!"}
        try:
            student = Student(student_id, name)
        except TypeError as te:
            response_data["success"] = False
            response_data["message"] = str(te)
            return response_data
        try:
            add_student_command = AddStudentCommand(self.__stud_repo, student)
            add_student_command.execute()
            self.__stud_repo.record_undo_commands([add_student_command])
        except RepositoryError as re:
            response_data["success"] = False
            response_data["message"] = str(re)
        return response_data

    def find_student_by_id(self, student_id):
        """
        Returns the student with the given ID
        :param student_id:
        :return:
        """
        response_data = {"success": True,
                         "message": "Student found successfully!",
                         "data": None}
        try:
            response_data["data"] = self.__stud_repo.find_student_by_id(
                student_id)
        except RepositoryError as e:
            response_data["success"] = False
            response_data["message"] = str(e)
        return response_data

    def remove_student(self, student_id):
        """
        Removes a student from the repository
        :param student_id:
        :return:
        """
        response_data = {"success": True,
                         "message": f"Student with ID {student_id} removed successfully!"}
        try:
            self.__stud_repo.find_student_by_id(student_id)
        except RepositoryError as e:
            response_data["success"] = False
            response_data["message"] = str(e)
            return response_data

        undo_commands = []
        remove_student_command = RemoveStudentCommand(
            self.__stud_repo, student_id)
        remove_student_command.execute()
        undo_commands.append(remove_student_command)

        student_grades = self.__stud_repo.get_grades_by_student(student_id)

        for grade in student_grades:
            remove_grade_command = RemoveGradeCommand(
                self.__stud_repo, grade.id)
            remove_grade_command.execute()
            undo_commands.append(remove_grade_command)

        self.__stud_repo.record_undo_commands(undo_commands)

        return response_data

    def update_student(self, student_id, new_name):
        """
        Updates a student from the repository
        :param student_id:
        :param new_name:
        :return:
        """
        response_data = {"success": True,
                         "message": f"Student with ID {student_id} updated successfully!"}
        try:
            update_student_command = UpdateStudentCommand(
                self.__stud_repo, student_id, new_name)
            update_student_command.execute()
            self.__stud_repo.record_undo_commands([update_student_command])
        except RepositoryError as e:
            response_data["success"] = False
            response_data["message"] = str(e)
            return response_data
        return response_data

    def list_students(self):
        """
        Returns the list of students
        :return:
        """
        response_data = {"success": True,
                         "message": "Students listed successfully!",
                         "data": self.__stud_repo.list_students()}
        return response_data

    def find_students(self, query):
        """
        Returns the list of students that match the query
        :param query:
        :return:
        """
        response_data = {"success": True,
                         "message": "Students listed successfully!",
                         "data": self.__stud_repo.find_students(query)}
        return response_data

    @property
    def add(self) -> callable:
        return self.add_student
