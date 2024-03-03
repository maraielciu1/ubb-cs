from unittest import TestCase

from Seminars.Seminar10.Domain.car import Car
from Seminars.Seminar10.Domain.client import Client
from Seminars.Seminar10.Domain.rental import Rental


class TestRental(TestCase):
    def test_rental_1(self):
        car = Car("CJ 50 MMM","Toyota","Yaris","red")
        client = Client("12","Ion")
        r = Rental("100",car,client,12,15)