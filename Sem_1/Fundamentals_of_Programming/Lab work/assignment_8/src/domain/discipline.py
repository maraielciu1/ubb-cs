from random import choice, random

from src.domain.id_object import IdObject


class Discipline(IdObject):
    def __init__(self, id, name: str):
        super().__init__(id)
        if len(name) < 3:
            raise TypeError("Name too short!")
        else:
            self.__name = name

    def __str__(self):
        return "Discipline id: " + str(self.id) + " name: " + self.name

    def __eq__(self, other):
        return isinstance(other, Discipline) and self.id == other.id

    def __copy__(self):
        return Discipline(self.id, self.name)

    @property
    def name(self):
        return self.__name

    @name.setter
    def name(self, new_name):
        self.__name = new_name


def generate_disciplines(n):
    disciplines = []
    start_id = 500
    names = ["Math", "English", "History", "Geography", "Physics", "Chemistry", "Biology", "Informatics", "French",
             "German"]
    used_names = set()

    for i in range(n):
        discipline_name = choice(names)
        while discipline_name in used_names:
            discipline_name = choice(names)

        used_names.add(discipline_name)
        disciplines.append(Discipline(str(i + start_id), discipline_name))

    return disciplines
