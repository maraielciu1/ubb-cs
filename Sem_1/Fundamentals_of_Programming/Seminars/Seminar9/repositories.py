from work import Car
from work import generate_cars
import pickle
class RepositoryError(Exception):
    """
    Custom exception for repository errors
    """
    pass


class CarRepoMemory:
    """
    In-memory repository for the Car domain entity
    """
    def __init__(self):
        # this is where we'll store cars
        self.__data = []
    def add(self,new_car: Car):
        """
        Add a new car to the repository
        Raise RepositoryError if car with license plate already exists
        """
        pass
    def get(self,license_plate: str) -> Car:
        """
        Return a car by license plate
        return: none or the car
        Raise RepositoryError if car with license plate does not exist
        """
        pass
    def remove(self,license_plate: str):
        """
        Remove a car from the repository
        return: none or the car that was removed
        Raise RepositoryError if car with license plate does not exist
        """
    def get_all(self):
        """
        Return all cars in the repository
        """
        pass
    def __str__(self):
        pass
    def __len__(self):
        """
        Return the number of cars in the repository

        """
        pass


"""
CarRepoBinaryFile should do everything CarRepoMemory does AND:
    -> save the car list to a binary file after the list of cars is changed
    -> load the car list when starting the repo (??)
"""
class CarRepoBinaryFile(CarRepoMemory):
    """
    CarRepoBinaryFile inherits from CarRepoMemory
        1. all methods from CarRepoMemory are available in CarRepoBinaryFile
        2. we need to add loadFile() and saveFile() methods
    """
    def __load_file(self):
        # TODO Call from class constructor
        pass
    def __save_file(self):
        # TODO Call from all methods that change the car repo
        pass
    def add(self,new_car: Car):
        # 1. call add from superclass (CarRepoMemory)
        super().add(new_car)
        # 2. if super().add(new_car) raised an exception we don't run this code
        # 3. if no exception, save the cars into file
        self.__save_file()






def test_car_repo():
    # test the repo with these cars
    car_repo = CarRepoMemory()
    assert len(car_repo) == 0

    cars = generate_cars(10)
    car0 = cars[0]
    for c in cars:
        car_repo.add(c)
    assert len(car_repo) == 10

    # check that same car cannot be added twice
    try:
        car_repo.add(car0)
        assert False
    except RepositoryError:
        assert True
    # make sure all cars are in the repo
    for car in cars:
        assert car_repo.get(car.license_plate) == car

    # check car deletion
    repo_len = 10
    assert len(car_repo) == 10
    for car in cars:
        assert car_repo.remove(car.license_plate) == car
        repo_len -=1
        assert len(car_repo) == repo_len

if __name__ == "__main__":
    #test_car_repo()

    # 1. write the file -- anytime the repo is changed (add, delete)
    #list_of_cars = generate_cars(10)
    # file = open("cars.data","wb") # w-write, b-binary
    # pickle.dump(list_of_cars,file)
    # file.close()

    # 2. read from file (__init__)
    file = open("cars.data", "rb")  # r-read, b-binary
    my_cars = pickle.load(file)
    file.close()

    for c in my_cars:
        print(c)