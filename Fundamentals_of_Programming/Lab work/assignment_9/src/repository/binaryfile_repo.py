import os
import pickle

from src.repository.configuration import Config
from src.repository.repository import Repository


class BinaryRepository(Repository):
    def __init__(self):
        super().__init__()

        self.data_folder = Config.DATA_FOLDER
        self.__create_folder()

        self.students_file = os.getenv(Config.STUDENT_OS_VARIABLE)
        self.disciplines_file = os.getenv(Config.DISCIPLINE_OS_VARIABLE)
        self.grades_file = os.getenv(Config.GRADE_OS_VARIABLE)

        self.__create_file(self.students_file)
        self.__create_file(self.grades_file)
        self.__create_file(self.disciplines_file)

        self.__load_students()
        self.__load_disciplines()
        self.__load_grades()

        self.__save_students()
        self.__save_disciplines()
        self.__save_grades()

    def __check_if_folder_exists(self):
        return os.path.exists(self.data_folder)

    def __check_if_file_exists(self, file_name):
        try:
            with open(self.data_folder + file_name, "rb") as file:
                return True
        except FileNotFoundError:
            return False

    def __create_folder(self):
        if not self.__check_if_folder_exists():
            os.mkdir(self.data_folder)

    def __create_file(self, file_name):
        if not self.__check_if_file_exists(file_name):
            with open(self.data_folder + file_name, "wb") as file:
                pass

    def __load_students(self):
        students = []
        with open(self.data_folder + self.students_file, "rb") as file:
            try:
                students = pickle.load(file)
            except EOFError:
                return
        if len(students) > 0:
            self.students = students
            self.grades = []

    def __load_disciplines(self):
        disciplines = []
        with open(self.data_folder + self.disciplines_file, "rb") as file:
            try:
                disciplines = pickle.load(file)
            except EOFError:
                return
        if len(disciplines) > 0:
            self.disciplines = disciplines
            self.grades = []

    def __load_grades(self):
        grades = []
        with open(self.data_folder + self.grades_file, "rb") as file:
            try:
                grades = pickle.load(file)
            except EOFError:
                return
        if len(grades) > 0:
            self.grades = grades

    def __save_students(self):
        with open(self.data_folder + self.students_file, "wb") as file:
            pickle.dump(self.students, file)

    def __save_disciplines(self):
        with open(self.data_folder + self.disciplines_file, "wb") as file:
            pickle.dump(self.disciplines, file)

    def __save_grades(self):
        with open(self.data_folder + self.grades_file, "wb") as file:
            pickle.dump(self.grades, file)

    def add_student(self, student):
        super().add_student(student)
        self.__save_students()

    def remove_student(self, student):
        super().remove_student(student)
        self.__save_students()
        self.__save_grades()

    def update_student(self, student_id,new_name):
        super().update_student(student_id,new_name)
        self.__save_students()

    def add_discipline(self, discipline):
        super().add_discipline(discipline)
        self.__save_disciplines()

    def remove_discipline(self, discipline):
        super().remove_discipline(discipline)
        self.__save_disciplines()
        self.__save_grades()

    def update_discipline(self, discipline_id,new_name):
        super().update_discipline(discipline_id,new_name)
        self.__save_disciplines()

    def add_grade(self, grade):
        super().add_grade(grade)
        self.__save_grades()

    def remove_grade(self, grade):
        super().remove_grade(grade)
        self.__save_grades()
