from src.domain.discipline import Discipline
from src.domain.grade import Grade
from src.domain.student import Student
from src.repository.memory_repo import RepositoryError
from src.services.main_service import Services
from src.services.discipline_service import DiscServ


class ConsoleUI:
    def __init__(self,repository):
        self.__services = Services(repository)

    ###

    def display_main_menu(self):
        print("\n1. Add Student")
        print("2. Remove Student")
        print("3. Update Student")
        print("4. List Students")
        print("5. Add Discipline")
        print("6. Remove Discipline")
        print("7. Update Discipline")
        print("8. List Disciplines")
        print("9. Add Grade")
        print("10. Remove Grades by Student")
        print("11. Remove Grades by Discipline")
        print("12. Get Failing Students")
        print("13. Get Best Students")
        print("14. Get Disciplines with Grades")
        print("15. Search Students")
        print("16. Search Disciplines")
        print("17. List All Grades")
        print("0. Exit")
    ###

    def display_service_message(self, service_response):
        if not service_response["success"]:
            print(f"Error: {service_response['message']}")
        else:
            print(service_response["message"])

    ###
    def read_student_data(self):
        student_id = self.read_student_id()
        name = self.read_student_name()
        return student_id, name
    ###

    ###
    def read_student_name(self, message: str = "Enter student name: "):
        name = input(message)
        return name

    def read_student_id(self, message: str = "Enter student ID: "):
        student_id = input(message)
        return student_id

    def read_discipline_id(self, message: str = "Enter discipline ID: "):
        discipline_id = input(message)
        return discipline_id

    def read_discipline_name(self, message: str = "Enter discipline name: "):
        name = input(message)
        return name

    def run(self):
        while True:
            try:
                self.display_main_menu()

                choice = input("Enter your choice: ")

                if choice == "1":
                    self.add_student()
                elif choice == "2":
                    self.remove_student()
                elif choice == "3":
                    self.update_student()
                elif choice == "4":
                    self.list_students()
                elif choice == "5":
                    self.add_discipline()
                elif choice == "6":
                    self.remove_discipline()
                elif choice == "7":
                    self.update_discipline()
                elif choice == "8":
                    self.list_disciplines()
                elif choice == "9":
                    self.add_grade()
                elif choice == "10":
                    self.remove_grades_by_student()
                elif choice == "11":
                    self.remove_grades_by_discipline()
                elif choice == "12":
                    self.get_failing_students()
                elif choice == "13":
                    self.get_best_students()
                elif choice == "14":
                    self.get_disciplines_with_grades()
                elif choice == "15":
                    self.search_students()
                elif choice == "16":
                    self.search_disciplines()
                elif choice == "17":
                    self.list_all_grades()
                elif choice == "0":
                    break
                else:
                    print("Invalid choice. Please try again.")

            except Exception as e:
                print(f"An error occurred: {e}")

    def add_student(self):
        student_id, name = self.read_student_data()
        service_response = self.__services.student_service.add_student(
            student_id, name)
        self.display_service_message(service_response)

    def remove_student(self):
        student_id = self.read_student_id()
        service_response = self.__services.student_service.remove_student(
            student_id)
        self.display_service_message(service_response)

    def update_student(self):
        student_id = self.read_student_id("Enter student ID to update: ")
        new_name = self.read_student_name("Enter new name: ")
        service_response = self.__services.student_service.update_student(
            student_id, new_name)
        self.display_service_message(service_response)

    def list_students(self):
        ###
        service_response = self.__services.student_service.list_students()
        students = service_response["data"]
        ###
        print("List of Students:")
        for student in students:
            print(f"{student.id}: {student.name}")

    def add_discipline(self):
        discipline_id = self.read_discipline_id()
        discipline_name = self.read_discipline_name()
        service_response = self.__services.discipline_service.add_discipline(
            discipline_id, discipline_name)
        self.display_service_message(service_response)

    def remove_discipline(self):
        discipline_id = self.read_discipline_id()
        service_response = self.__services.discipline_service.remove_discipline(
            discipline_id)
        self.display_service_message(service_response)

    def update_discipline(self):
        discipline_id = self.read_discipline_id(
            "Enter discipline ID to update: ")
        new_name = self.read_discipline_name("Enter new name: ")
        service_response = self.__services.discipline_service.update_discipline(
            discipline_id, new_name)
        self.display_service_message(service_response)

    def list_disciplines(self):
        service_response = self.__services.discipline_service.list_disciplines()
        disciplines = service_response["data"]
        print("List of Disciplines:")
        for discipline in disciplines:
            print(f"{discipline.id}: {discipline.name}")

    def add_grade(self):
        student_id = self.read_student_id()
        discipline_id = self.read_discipline_id()
        grade_value = input("Enter grade value: ")
        service_response = self.__services.grade_service.add_grade(
            student_id, discipline_id, grade_value)
        self.display_service_message(service_response)

    def remove_grades_by_student(self):
        student_id = self.read_student_id()
        service_response = self.__services.grade_service.remove_grades_by_student(
            student_id)
        self.display_service_message(service_response)

    def remove_grades_by_discipline(self):
        discipline_id = self.read_discipline_id()
        service_response = self.__services.grade_service.remove_grades_by_discipline(
            discipline_id)
        self.display_service_message(service_response)

    def get_failing_students(self):
        service_response = self.__services.grade_service.get_failing_students()
        failing_students = service_response["data"]
        if failing_students:
            print("Failing Students:")
            for student in failing_students:
                print(f"{student.id}: {student.name}")
        else:
            print("No failing students.")

    def get_best_students(self):
        service_response = self.__services.grade_service.best_students()
        best_students = service_response["data"]
        if best_students:
            print("Best Students:")
            for student in best_students:
                print(f"{student.id}: {student.name}")
        else:
            print("No students found.")

    def get_disciplines_with_grades(self):
        service_response = self.__services.grade_service.disciplines_with_grades_sorted_by_average()
        disciplines = service_response["data"]
        if disciplines:
            print("Disciplines with Grades:")
            for discipline in disciplines:
                print(
                    f"Discipline: {discipline[0].name}, Average Grade: {discipline[1]}")
        else:
            print("No disciplines found.")

    def search_students(self):
        query = input("Enter student name or ID to search: ")
        service_response = self.__services.student_service.find_students(
            query)
        found_students = service_response["data"]
        if found_students:
            print("Found Students:")
            for student in found_students:
                print(f"{student.id}: {student.name}")
        else:
            print("No students found.")

    def search_disciplines(self):
        query = input("Enter discipline name or ID to search: ")
        service_response = self.__services.discipline_service.find_disciplines(
            query)
        found_disciplines = service_response["data"]
        if found_disciplines:
            print("Found Disciplines:")
            for discipline in found_disciplines:
                print(f"{discipline.id}: {discipline.name}")
        else:
            print("No disciplines found.")

    def list_all_grades(self):
        service_response = self.__services.grade_service.list_grades()
        grades = service_response["data"]
        if grades:
            print("List of All Grades:")
            for grade in grades:
                print(
                    f"Discipline: {grade.discipline.name}, Student: {grade.student.name}, Grade: {grade.grade}")
        else:
            print("No grades found.")
