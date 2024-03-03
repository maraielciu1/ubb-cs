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
        return "the car with the license plate: "+str(self.license_plate)+" made by "+str(self.make)+" it's a "+str(self.color)+" "+str(self.model)
    def __eq__(self,other):
        return self.__licence_plate == other.__licence_plate
