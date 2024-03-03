"""
Write an application that manages a group of students.
Each student has a unique id (string), a name (string) and a grade (integer).
The application will have a menu-driven user interface and will provide the
following features:

    1. Add a student
        - adds the student with the given id, name and grade to the list.
        - error if giving existing id, the name or grade fields not given or
        empty

    2. Delete a student
        - deletes the student with the given id from the list
        - error if non-existing id given

    3. Show all students
        - shows all students in descending order of their grade

    4. Show students whose grade is > than given one, ordered in descending
    order of grade

    5. exit
        - exit the program

    Observations:
        - Add 10 random students at program startup
        - Write specifications for non-UI functions
        - Each function does one thing only, and communicates via parameters and return value
        - The program reports errors to the user. It must also report errors from non-UI functions!
        - Make sure you understand the student's representation
        - Try to reuse functions across functionalities (Less code to write and test)
        - Don't use global variables!
"""

from random import choice, randint
"""
    Student representation functions are below
"""
# student as a dict
def create_student(_id: str, name: str, grade: int):
    """
    Creates a student
    :param id:
    :param name:
    :param grade:
    :return:
    """
    # What to do in case of bad input params??!!
    # V1 - return error
    # V2 - use exceptions
    if len(_id) == 0 or len(name) < 3  or grade < 1 or grade >10:
        raise ValueError("Invalid data to create student")
    # encapsulation -- keep everything having to do with the student in one place
    return {"id": _id, "name": name, "grade": grade}
    # return[_id,name,grade]

def get_id(student) -> str:
    return student["id"]
    #return student[0]

def get_name(student) -> str:
    return student["name"]
    #return student[1]

def get_grade(student) -> int:
    return student["grade"]
    #return student[2]

def to_str(student) -> str:
    return "ID: "+ get_id(student) +", name: "+get_name(student)+", grade: "+get_grade(student)


"""
    Program functionalities (non-UI) are below
"""

def find_by_id(student_list: list, _id: str) -> int:
    """
    Return the student with given id, None if no student found
    :param student_list: The list of students
    :param _id: The id of the student to find
    :return: The student with given id, None if no student found
    """
    for s in student_list:
        if get_id(s) == _id:
            return s
    # student was not found
    return None

def generate_random_student(n: int) -> list:
    """
    Generate n pseudo-random students
    :param n: number of students
    :return: The list of generated students
    """
    family_names = ["Pop","Marian","Boldar","Popescu","Vranceanu"]
    given_name = ["Anca","Emilia","John","Timotei","Vlad"]
    current_id = 100
    result = []
    while n > 0:
        name = choice(family_names)+" "+choice(given_name)
        student = create_student(str(current_id),name, randint(1,10))
        result.append(student)
        n -=1
        current_id +=1
    return result

def add_student(student_list: list, student):
    """
    Add a student to the list
    :param student_list: The list of students
    :param student: The new student
    :return: None
    Raise ValueError if the student id already exists
    """
    if find_by_id(student_list,get_id(student)) is not None:
        raise ValueError("Duplicate student id - "+get_id(student))

    # alternative for code above
    #   for s in student_list:
    #       if get_id(s) == get_id(student):
    #           raise ValueError("Duplicate student id - "+get_id(student))
    # if we get here => no error was raised
    student_list.append(student)

def delete_student(student_list: list, _id:str):
    """
    Delete a student with given id
    :param student_list: The list of students
    :param _id: The id of the student to delete
    :return: None
    Raise ValueError if the student id does not exist
    """

    student = find_by_id(student_list,_id)
    if student is None:
        raise ValueError("Student not found ")
    student_list.remove(student)



"""
    UI functions are below
"""
def print_menu():
    print("1: Display all students")
    print("2. Add students")
    print("3. Delete student")
    print("0: Exit")

def add_student_ui(students: list):
    try:
        _id = input("student id: ")
        name = input("student name: ")
        # might raise ValueError in case a str is provided that cannot be converted to an int
        grade = int(input("student grade: "))

        #try:
        # try/ecxept creates its own scope for variables
        student = create_student(_id,name,grade)
        add_student(students,student)
    except ValueError as ve:
        print("There was an error!")
        print(str(ve))

def delete_student_ui(students: list):
    _id = input("student id: ")
    try:
        delete_student(students,_id)
    except ValueError as ve:
        print(str(ve))

def display_students_ui(students: list):
    for s in students:
        print(s)
def start():
    # generate some input data so we don't have to work so much
    students = generate_random_student(10)

    # references to functions we call for each option
    functions ={"1": display_students_ui, "2": add_student_ui, "3": delete_student_ui}
    while True:
        print_menu()
        option = input(">")

        if option in functions:
            functions[option](students)
        elif option == "0":
            print("bye!")
            return
        else:
            # not a valid option
            print("Invalid menu option!")

        #if option == "1":
        #    display_students_ui(students)
        #elif option == "2":
        #    add_student_ui(students)
        #elif option == "3":
        #    delete_student_ui(students)
        #elif option == "0":
        #    print("bye!")
        #    return
        #else:
            # not a valid option
        #    print("Invalid menu option!")


if __name__ == "__main__":
    start()
