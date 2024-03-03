from unittest import TestCase
from Seminars.Seminar10.Domain.car import Car


class TestCar(TestCase):
    """
    class TestCar inherits from class TestCase
        -> all methods and non-private fields in TestCase can be accessed by the inheriting class

    unittest.TestCase is thebase unit testing class in PyUnit
        -> all classes that inherit from TestCase are test classes
    """
    def setUp(self):
        """
        runs before all test_ methods
        """
        self.__c = Car("CJ 01 ABC","Toyota","Yaris","red")
    def tearDown(self):
        """
        runs after all tests are complete
        """
        pass
    def test_car_1(self):
        # c = Car("CJ 01 ABC","Toyota","Yaris","red")

        # not nice bcs AssertionError stops all test from running
        assert self.__c.license_plate == "CJ 01 ABC"

        self.assertEqual(self.__c.license_plate,"CJ 01 ABC","oopsie!")
    def test_car_2(self):
        #c = Car("CJ 01 ABC","Toyota","Yaris","red")

        # not nice bcs AssertionError stops all test from running
        assert self.__c.license_plate == "CJ 01 ABC"

        self.assertEqual(self.__c.license_plate,"CJ01 ABC","not ok!")
    def test_car_3(self):
        #c = Car("CJ 01 ABC","Toyota","Yaris","red")

        # not nice bcs AssertionError stops all test from running
        assert self.__c.license_plate == "CJ 01 ABC"

        self.assertEqual(self.__c.license_plate,"CJ 01 ABC","oopsie!")
    def test_car_4(self):
        #c = Car("CJ 01 ABC","Toyota","Yaris","red")

        # not nice bcs AssertionError stops all test from running
        assert self.__c.license_plate == "CJ 01 ABC"

        self.assertEqual(self.__c.license_plate,"CJ 01 ABC","oopsie!")


