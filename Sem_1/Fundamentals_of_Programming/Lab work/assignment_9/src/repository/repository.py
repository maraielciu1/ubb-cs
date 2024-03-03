from src.domain.discipline import Discipline
from src.domain.grade import generate_grades, Grade
from src.domain.student import Student


class RepositoryError(Exception):
    def __init__(self, message: str) -> None:
        self.__message = message

    @property
    def message(self) -> str:
        return self.__message

    def __str__(self) -> str:
        return self.__message


class Repository:
    def __init__(self):
        students, disciplines, grades = generate_grades(10)
        self.students = students
        self.disciplines = disciplines
        self.grades = grades
        self.undo_commands_stack = []
        self.redo_commands_stack = []

    def find_student_by_id(self, student_id):
        """
        Finds a student by ID
        :param student_id:
        :return:
        """
        for student in self.students:
            if student.id == student_id:
                return student
        raise RepositoryError(f"Student with ID {student_id} not found.")

    def find_discipline_by_id(self, discipline_id):
        """
        Finds a discipline by ID
        :param discipline_id:
        :return:
        """
        for discipline in self.disciplines:
            if discipline.id == discipline_id:
                return discipline
        raise RepositoryError(f"Discipline with ID {discipline_id} not found.")

    def find_grade_by_id(self, grade_id):
        """
        Finds a grade by ID
        :param grade_id:
        :return:
        """
        for grade in self.grades:
            if grade.id == grade_id:
                return grade
        raise RepositoryError(f"Grade with ID {grade_id} not found.")

    def add_student(self, student):
        """
        Adds a student to the repository
        :param student:
        :return:
        """
        if student in self.students:
            raise RepositoryError(
                f"Student with ID {student.id} already exists.")
        if not isinstance(student, Student):
            raise RepositoryError("Not a student.")
        self.students.append(student)

    def remove_student(self, student_id):
        """
        Removes a student from the repository
        :param student_id:
        :return:
        """
        for student in self.students:
            if student.id == student_id:
                self.students.remove(student)
                return
        raise RepositoryError(f"Student with ID {student_id} not found.")

    def update_student(self, student_id, new_name):
        """
        Updates a student from the repository
        :param student_id:
        :param new_name:
        :return:
        """
        for student in self.students:
            if student.id == student_id:
                student.name = new_name
                return
        raise RepositoryError(f"Student with ID {student_id} not found.")

    def list_students(self):
        """
        Returns the list of students
        :return:
        """
        return self.students.copy()

    def add_discipline(self, discipline):
        """
        Adds a discipline to the repository
        :param discipline:
        :return:
        """
        if discipline in self.disciplines:
            raise RepositoryError(
                f"Discipline with ID {discipline.id} already exists.")
        if not isinstance(discipline, Discipline):
            raise RepositoryError("Not a discipline.")
        self.disciplines.append(discipline)

    def remove_discipline(self, discipline_id):
        """
        Removes a discipline from the repository
        :param discipline_id:
        :return:
        """
        for discipline in self.disciplines:
            if discipline.id == discipline_id:
                self.disciplines.remove(discipline)
                return
        raise RepositoryError(f"Discipline with ID {discipline_id} not found.")

    def update_discipline(self, discipline_id, new_name):
        """
        Updates a discipline from the repository
        :param discipline_id:
        :param new_name:
        :return:
        """
        for discipline in self.disciplines:
            if discipline.id == discipline_id:
                discipline.name = new_name
                return
        raise RepositoryError(f"Discipline with ID {discipline_id} not found.")

    def list_disciplines(self):
        """
        Returns the list of disciplines
        :return:
        """
        return self.disciplines.copy()

    def add_grade(self, grade):
        if not isinstance(grade, Grade):
            raise RepositoryError("Not a grade.")
        if grade.grade < 1 or grade.grade > 10:
            raise RepositoryError("Invalid grade value.")

        self.find_discipline_by_id(grade.discipline.id)
        self.find_student_by_id(grade.student.id)

        if grade in self.grades:
            raise RepositoryError(
                f"Grade with ID {grade.id} already exists.")

        self.grades.append(grade)

    def remove_grade(self, grade_id):
        for grade in self.grades:
            if grade.id == grade_id:
                self.grades.remove(grade)
                return
        raise RepositoryError(f"Grade with ID {grade_id} not found.")

    def remove_grades_by_student(self, student_id):
        self.grades = [
            grade for grade in self.grades if grade.student.id != student_id]

    def remove_grades_by_discipline(self, discipline_id):
        self.grades = [
            grade for grade in self.grades if grade.discipline.id != discipline_id]

    def get_grades_by_student(self, student_id):
        return [grade for grade in self.grades if grade.student.id == student_id]

    def get_grades_by_discipline(self, discipline_id):
        return [grade for grade in self.grades if grade.discipline.id == discipline_id]

    def get_average_grade_for_discipline(self, discipline_id, student_id=None):
        if student_id is not None:
            discipline_grades = [grade.grade for grade in self.grades if
                                 grade.discipline.id == discipline_id and grade.student.id == student_id]
        else:
            discipline_grades = [
                grade.grade for grade in self.grades if grade.discipline.id == discipline_id]

        if not discipline_grades:
            return None

        return sum(discipline_grades) / len(discipline_grades)

    def get_student_aggregated_average(self, student_id):
        student_grades = [
            grade.grade for grade in self.grades if grade.student.id == student_id]

        if not student_grades:
            return None

        return sum(student_grades) / len(student_grades)

    def get_all_disciplines_with_grades(self):
        return list(set(grade.discipline.id for grade in self.grades))

    def find_students(self, query):
        query_lower = query.lower()
        return [student for student in self.students if query_lower in student.name.lower()]

    def find_disciplines(self, query):
        query_lower = query.lower()
        return [discipline for discipline in self.disciplines if query_lower in discipline.name.lower()]

    def list_all_grades(self):
        return self.grades.copy()

    def record_undo_commands(self, commands):
        self.undo_commands_stack.append(commands)
        self.redo_commands_stack.clear()