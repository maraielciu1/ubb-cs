import os

from src.domain.discipline import Discipline
from src.domain.grade import Grade
from src.domain.student import Student
from src.repository.configuration import Config
from src.repository.repository import Repository


class TextRepository(Repository):
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
            with open(self.data_folder + file_name, "r") as file:
                return True
        except FileNotFoundError:
            return False

    def __create_folder(self):
        if not self.__check_if_folder_exists():
            os.mkdir(self.data_folder)

    def __create_file(self, file_name):
        if not self.__check_if_file_exists(file_name):
            with open(self.data_folder + file_name, "w") as file:
                pass

    def __load_students(self):
        students = []
        with open(self.data_folder + self.students_file, "r") as file:
            for line in file:
                line = line.strip()
                if line == "":
                    continue
                line = line.split(",")
                students.append(Student(
                    line[Config.STUDENT_ID_FILE_INDEX], line[Config.STUDENT_NAME_FILE_INDEX]))
        if len(students) > 0:
            self.students = students
            self.grades = []

    def __load_disciplines(self):
        disciplines = []
        with open(self.data_folder + self.disciplines_file, "r") as file:
            for line in file:
                line = line.strip()
                if line == "":
                    continue
                line = line.split(",")
                disciplines.append(Discipline(
                    line[Config.DISCIPLINE_ID_FILE_INDEX], line[Config.DISCIPLINE_NAME_FILE_INDEX]))
        if len(disciplines) > 0:
            self.disciplines = disciplines
            self.grades = []

    def __load_grades(self):
        grades = []
        with open(self.data_folder + self.grades_file, "r") as file:
            for line in file:
                line = line.strip()
                if line == "":
                    continue
                line = line.split(",")
                student = self.find_student_by_id(
                    line[Config.GRADE_STUDENT_ID_FILE_INDEX])
                discipline = self.find_discipline_by_id(
                    line[Config.GRADE_DISCIPLINE_ID_FILE_INDEX])
                grade = Grade(line[Config.GRADE_ID_FILE_INDEX],
                              student, discipline, float(line[Config.GRADE_VALUE_FILE_INDEX]))
                grades.append(grade)
        if len(grades) > 0:
            self.grades = grades

    def __save_students(self):
        with open(self.data_folder + self.students_file, "w") as file:
            for student in self.students:
                file.write(f"{student.id},{student.name}\n")

    def __save_disciplines(self):
        with open(self.data_folder + self.disciplines_file, "w") as file:
            for discipline in self.disciplines:
                file.write(f"{discipline.id},{discipline.name}\n")

    def __save_grades(self):
        with open(self.data_folder + self.grades_file, "w") as file:
            for grade in self.grades:
                file.write(
                    f"{grade.id},{grade.student.id},{grade.discipline.id},{grade.grade}\n")

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
