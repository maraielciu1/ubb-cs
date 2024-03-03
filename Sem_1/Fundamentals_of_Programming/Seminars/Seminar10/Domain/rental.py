from datetime import date
from car import Car
from client import Client

class Rental:
    """
    1 rental object means 1 client rented 1 care for a given time interval

    Rental class should have:
        - id -> str
        - car -> Car
        - client -> Client
        - start, end -> date

    All fields are set in the constructor
    All fields have getter properties
    """
    def __init__(self,id:str,car:Car,client:Client,start:date,end:date):
        self.__id = id
        self.__car = car
        self.__client = client
        self.__start = start
        self.__end = end
    @property
    def id(self):
        return self.__id
    @property
    def car(self):
        return self.__car
    @property
    def client(self):
        return self.__client
    @property
    def start(self):
        return self.__start
    @property
    def end(self):
        return self.__end

if __name__ == "__main__":
    print(date.today())
    d = date(2019, 12, 1)
    print(d)
    print(d.strftime("%d/%m/%Y"))
