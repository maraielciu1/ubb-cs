from random import choice, randint

from src.domain.discipline import Discipline, generate_disciplines
from src.domain.id_object import IdObject
from src.domain.student import Student, generate_students


class Grade():
    def __init__(self, student: Student, discipline: Discipline, grade: float):
        if not isinstance(discipline, Discipline):
            raise TypeError("discipline must be a Discipline type object!")
        if not isinstance(student, Student):
            raise TypeError("student must be a Student type object!")
        if not isinstance(grade, float) and not isinstance(grade, int):
            raise TypeError("grade is not a float!")
        if grade < 1 or grade > 10:
            raise TypeError("grade is not between 1 and 10!")

        self._discipline = discipline
        self._student = student
        self._grade = grade

    def __str__(self):
        return " discipline: " + str(self.discipline) + " student: " + str(
            self.student) + " grade: " + str(self.grade)

    @property
    def discipline(self):
        return self._discipline

    @property
    def student(self):
        return self._student

    @property
    def grade(self):
        return self._grade


def generate_grades(n):
    grades = []
    students = generate_students(n)
    disciplines = generate_disciplines(n)
    for i in range(n):
        grades.append(
            Grade(choice(students), choice(disciplines), randint(1, 10)))
    return students, disciplines, grades
