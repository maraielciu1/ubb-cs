from Seminars.Seminar10.Domain.car import Car
from random import randint,choice

class CarRepoIterator:
    def __init__(self,all_cars):
        self.__cars = all_cars
        self.__pos = -1
    def __next__(self):
        self.__pos +=1
        if self.__pos == len(self.__cars):
            # no more cars
            # signal the calling loop to stop
            raise StopIteration()
        return self.__cars[self.__pos]

class RepositoryError(Exception):
    pass

class CarRepoMemory:
    def __init__(self):
        self.__data = {}

    def add(self,new_car:Car):
        if new_car.license_plate in self.__data:
            raise RepositoryError()
        self.__data[new_car.license_plate] = new_car

    def get(self, licence_plate:str)->Car:
        # V1 - slower bcs 2 times dict access
        # if licence_plate not in self.__data:
        #     return None
        # return self.__data[licence_plate]


        # V2 - slower bcs the exception might have to be built
        try:
            return self.__data[licence_plate]
        except KeyError:
            return None
    def remove(self,licence_plate:str)->Car:
        if licence_plate not in self.__data:
            return None
        del self.__data[licence_plate] # ????

    def get_all(self):
        return list(self.__data.values())

    def __len__(self):
        return len(self.__data)
    def __iter__(self):
        return CarRepoIterator(list(self.__data.values()))

def generate(n: int):
    cars = []
    county = ["AB", "CJ", "TL", "B"]
    numb = ["MRR", "BAD", "LLL", "PMO", "CCD", "VAR"]
    color = ["red", "blue", "grey", "black", "white"]
    make = ["BMW"," Audi"," Honda"," Toyota"," Ford"]
    model = ["A4", "M5", "C-Class", "A3", "Focus","Mondeo"]
    for i in range(n):
        licence_plate = choice(county) + str(randint(10, 99)) + choice(numb)
        car = Car(licence_plate, choice(make), choice(model), choice(color))
        cars.append(car)
    return cars

if __name__ == "__main__":
    repo = CarRepoMemory()
    for c in generate(10):
        #print(c)
        repo.add(c)

    for car in repo:
        print(car)