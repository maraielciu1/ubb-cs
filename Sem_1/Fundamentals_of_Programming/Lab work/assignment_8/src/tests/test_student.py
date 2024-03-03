from unittest import TestCase
from src.domain.student import Student
from src.repository.memory_repo import MemoryRepository
from src.services.student_service import StudServ


class TestStud(TestCase):
    def setUp(self):
        self.__s = Student("1", "Ana")

    def tearDown(self):
        pass

    def test_stud_1(self):
        assert self.__s.name == "Ana"
        self.assertEqual(self.__s.name, "Ana", "oopsie!")

    def test_stud_2(self):
        assert self.__s.id == "1"
        self.assertEqual(self.__s.id, "1", "oopsie!")


class TestStudAdd(TestCase):
    def setUp(self):
        self.__s = StudServ(MemoryRepository())

    def tearDown(self):
        pass

    def test_stud_add_1(self):
        self.__s.add_student("1", "Ana")
        self.assertEqual(len(self.__s.list_students()["data"]), 11, "oopsie!")

    def test_stud_add_2(self):
        self.__s.add_student("2", "Maria")
        self.assertEqual(len(self.__s.list_students()["data"]), 11, "oopsie!")


class TestStudRemove(TestCase):
    def setUp(self):
        self.__s = StudServ(MemoryRepository())

    def tearDown(self):
        pass

    def test_stud_remove_1(self):
        self.__s.remove_student("100")
        self.assertEqual(len(self.__s.list_students()["data"]), 9, "oopsie!")

    def test_stud_remove_2(self):
        self.__s.remove_student("105")
        self.assertEqual(len(self.__s.list_students()["data"]), 9, "oopsie!")


class TestStudUpdate(TestCase):
    def setUp(self):
        self.__s = StudServ(MemoryRepository())

    def tearDown(self):
        pass

    def test_stud_update_1(self):
        self.__s.add_student("1", "Ana")
        self.__s.update_student("1", "Maria")
        self.assertEqual(len(self.__s.list_students()["data"]), 11, "oopsie!")

    def test_stud_update_2(self):
        self.__s.add_student("2", "Maria")
        self.__s.update_student("2", "Ana")
        self.assertEqual(len(self.__s.list_students()["data"]), 11, "oopsie!")


class TestStudList(TestCase):
    def setUp(self):
        self.__s = StudServ(MemoryRepository())

    def tearDown(self):
        pass

    def test_stud_list_1(self):
        self.__s.add_student("1", "Ana")
        self.assertEqual(len(self.__s.list_students()["data"]), 11, "oopsie!")