from random import choice

from src.domain.id_object import IdObject


class Student(IdObject):
    def __init__(self, id, name: str):
        super().__init__(id)
        if len(name) < 3:
            raise TypeError("Name too short!")
        else:
            self.__name = name

    @property
    def name(self):
        return self.__name

    @name.setter
    def name(self, new_name):
        self.__name = new_name

    def __str__(self):
        return "Student id: " + str(self.id) + " name: " + self.name

    def __eq__(self, other):
        return isinstance(other, Student) and self.id == other.id

    def __copy__(self):
        return Student(self.id, self.name)

    def __hash__(self):
        # Use the hash of the student's ID for uniqueness in the set
        return hash(self.id)


def generate_students(n):
    students = []
    start_id = 100
    names = ["John", "Mary", "Peter", "Alice", "Bob", "Alex", "Anna", "George", "Mia", "Michael", "David", "Daniel",
             "Diana", "Dennis", "Dylan", "Dorothy", "Derek"]
    used_names = set()

    for i in range(n):
        student_name = choice(names)
        while student_name in used_names:
            student_name = choice(names)

        used_names.add(student_name)
        students.append(Student(str(i + start_id), student_name))

    return students
