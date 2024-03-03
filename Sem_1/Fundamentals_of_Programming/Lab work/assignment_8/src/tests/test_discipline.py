from unittest import TestCase
from src.domain.discipline import Discipline
from src.repository.memory_repo import MemoryRepository
from src.services.discipline_service import DiscServ


class TestDisc(TestCase):
    def setUp(self):
        self.__d = Discipline("1", "Mate")

    def tearDown(self):
        pass

    def test_disc_1(self):
        self.assertEqual(self.__d.name, "Mate", "oopsie!")

    def test_disc_2(self):
        self.assertEqual(self.__d.id, "1", "oopsie!")


class TestDiscAdd(TestCase):
    def setUp(self):
        self.__d = DiscServ(MemoryRepository())

    def tearDown(self):
        pass

    def test_disc_add_1(self):
        service_response = self.__d.add_discipline("1", "Mate")
        self.assertEqual(service_response["success"], True)
        self.assertEqual(
            len(self.__d.list_disciplines()["data"]), 11)

    def test_disc_add_2(self):
        service_response = self.__d.add_discipline("2", "Romana")
        self.assertEqual(service_response["success"], True)
        service_response = self.__d.add_discipline("2", "Romana")
        self.assertEqual(service_response["success"], False)
        self.assertEqual(len(self.__d.list_disciplines()["data"]), 11)


class TestDiscRemove(TestCase):
    def setUp(self):
        self.__d = DiscServ(MemoryRepository())

    def tearDown(self):
        pass

    def test_disc_remove_1(self):
        self.__d.add_discipline("1", "Mate")
        self.__d.remove_discipline("1")
        self.assertEqual(
            len(self.__d.list_disciplines()["data"]), 10, "oopsie!")

    def test_disc_remove_2(self):
        self.__d.remove_discipline("505")
        self.assertEqual(
            len(self.__d.list_disciplines()["data"]), 9, "oopsie!")


class TestDiscUpdate(TestCase):
    def setUp(self):
        self.__d = DiscServ(MemoryRepository())

    def tearDown(self):
        pass

    def test_disc_update_1(self):
        self.__d.add_discipline("1", "Mate")
        self.__d.update_discipline("1", "Romana")
        self.assertEqual(
            len(self.__d.list_disciplines()["data"]), 11, "oopsie!")

    def test_disc_update_2(self):
        self.__d.add_discipline("2", "Romana")
        self.__d.update_discipline("2", "Mate")
        self.assertEqual(
            len(self.__d.list_disciplines()["data"]), 11, "oopsie!")


class TestDiscList(TestCase):
    def setUp(self):
        self.__d = DiscServ(MemoryRepository())

    def tearDown(self):
        pass

    def test_disc_list_1(self):
        self.assertEqual(
            len(self.__d.list_disciplines()["data"]), 10, "oopsie!")

    def test_disc_list_2(self):
        self.assertEqual(
            len(self.__d.list_disciplines()["data"]), 10, "oopsie!")