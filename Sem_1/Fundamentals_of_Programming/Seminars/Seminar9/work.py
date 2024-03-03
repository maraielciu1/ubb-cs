"""
1. Write a Car class
    private fields:
        __licence_plate
        __make
        __model
        __color
    properties:
        only get for the __licence plate, __make, __model
        get/set for __color
    methods:
        constructor (must set all fields)
        __str__ (return a one-line str with the car's info)
        __eq__ (==, return True iff cars have the same licence plates)
2. Write a generate_cars(n: int) -> list method
    - generate a list of n random cars
    - license plates should be valid(pick a few counties - [AB, CJ, B, TL]
    - make/model/color combined from a defined list

"""
from random import randint, choice
class Car:
    def __init__(self,licence_plate, make, model, color):
        self.__licence_plate = licence_plate
        self.__make = make
        self.__model = model
        self.__color = color
    def set_color(self,new_color):
        self.__color = new_color
    @property
    def license_plate(self):
        return self.__licence_plate
    @property
    def make(self):
        return self.__make
    @property
    def model(self):
        return self.__model
    @property
    def color(self):
        return self.__color
    @color.setter
    def color(self,new_color):
        self.set_color(new_color)

    def __str__(self):
        return "the car with the license plate: "+str(my_car.license_plate)+" made by "+str(my_car.make)+" it's a "+str(my_car.color)+" "+str(my_car.model)
    def __eq__(self,other):
        return self.__licence_plate == other.__licence_plate


def generate_cars(n: int):
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


my_car = Car("CJ 67 ABL", "Toyota", "Corolla", "red")
other_car = Car("CJ 67 ABL", "Ford", "Mondeo", "blue")
print(my_car.__str__())
print(my_car.__eq__(other_car))

car_list = generate_cars(10)
for car in car_list:
    print(car.__str__())
