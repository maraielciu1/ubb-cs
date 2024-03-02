from src.services.main_service import Services
from src.ui.configuration import Config


class ConsoleUI:
    def __init__(self):
        self.__services = Services()

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
        print("18. Undo")
        print("19. Redo")
        print("0. Exit")

    def display_service_message(self, service_response):
        if not service_response["success"]:
            print(f"Error: {service_response['message']}")
        else:
            print(service_response["message"])

    def read_student_data(self):
        student_id = self.read_student_id()
        name = self.read_student_name()
        return student_id, name

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

    def read_grade_id(self, message: str = "Enter grade ID: "):
        grade_id = input(message)
        return grade_id

    def run(self):
        while True:
            try:
                self.display_main_menu()

                choice = input("Enter your choice: ")

                if choice == Config.ADD_STUDENT_OPTION:
                    self.add_student()
                elif choice == Config.REMOVE_STUDENT_OPTION:
                    self.remove_student()
                elif choice == Config.UPDATE_STUDENT_OPTION:
                    self.update_student()
                elif choice == Config.LIST_STUDENTS_OPTION:
                    self.list_students()
                elif choice == Config.ADD_DISCIPLINE_OPTION:
                    self.add_discipline()
                elif choice == Config.REMOVE_DISCIPLINE_OPTION:
                    self.remove_discipline()
                elif choice == Config.UPDATE_DISCIPLINE_OPTION:
                    self.update_discipline()
                elif choice == Config.LIST_DISCIPLINES_OPTION:
                    self.list_disciplines()
                elif choice == Config.ADD_GRADE_OPTION:
                    self.add_grade()
                elif choice == Config.REMOVE_GRADES_BY_STUDENT_OPTION:
                    self.remove_grades_by_student()
                elif choice == Config.REMOVE_GRADES_BY_DISCIPLINE_OPTION:
                    self.remove_grades_by_discipline()
                elif choice == Config.GET_FAILING_STUDENTS_OPTION:
                    self.get_failing_students()
                elif choice == Config.GET_BEST_STUDENTS_OPTION:
                    self.get_best_students()
                elif choice == Config.GET_DISCIPLINES_WITH_GRADES_OPTION:
                    self.get_disciplines_with_grades()
                elif choice == Config.SEARCH_STUDENTS_OPTION:
                    self.search_students()
                elif choice == Config.SEARCH_DISCIPLINES_OPTION:
                    self.search_disciplines()
                elif choice == Config.LIST_ALL_GRADES_OPTION:
                    self.list_all_grades()
                elif choice == Config.UNDO_OPTION:
                    self.undo()
                elif choice == Config.REDO_OPTION:
                    self.redo()
                elif choice == Config.EXIT_OPTION:
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
        service_response = self.__services.student_service.list_students()
        students = service_response["data"]
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
        grade_id = self.read_grade_id()
        student_id = self.read_student_id()
        discipline_id = self.read_discipline_id()
        grade_value = input("Enter grade value: ")
        service_response = self.__services.grade_service.add_grade(
            grade_id,
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
                    f"ID: {grade.id}, Discipline: {grade.discipline.name}, Student: {grade.student.name}, Grade: {grade.grade}")
        else:
            print("No grades found.")

    def undo(self):
        service_response = self.__services.undo()
        self.display_service_message(service_response)

    def redo(self):
        service_response = self.__services.redo()
        self.display_service_message(service_response)
