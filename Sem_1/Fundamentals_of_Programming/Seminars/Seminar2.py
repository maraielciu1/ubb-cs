print("hello world")

"""
We want to get some practice with
    1. Python 3
    2. Writing a menu-driven console application
    3. Generating some numbers
    4. Representing program entities

Let's implement something - Manage a list of rational numbers 
    Functionalities:
        1. Add a new rational number from console
        2. Adding some random numbers programmatically
        3. Sort the list of numbers
        4. Exit the program
"""

# Functions for rational numbers
# create a number, compare 2 numbers

# Data type recap: int, float, boolean, str, tuple, list, dict
# we keep the numbers in a python list
# we represent each number as a dict

from random import randint
from math import gcd


def create_q(num: int, den: int) -> dict:
    """
    Create a new rational number
    :param num: numerator
    :param den: denominator value
    :return: the newly created value
    """
    g = gcd(num, den)

    return {"num": num // g, "den": den // g}  # // - integer division


def to_str(q: dict) -> str:
    """
    Return the number's String representation
    :param q: the rational number
    :return: str representation
    """
    if q["den"] == 1:
        return str(q["num"])
    else:
        return str(q["num"]) + "/" + str(q["den"])


# q = 1/3 => {"num":1, "den":3}

print(create_q(1, 4))
print(to_str(create_q(1, 4)))

# Program functionalities


"""
data = [create_q(1,2),create_q(2,3), create_q(4,5)]
print(data)
for i in range(0,len(data)):
    print(to_str(data[i]))

for q in data:
    print(to_str(q))
"""

data = [create_q(1, 2), create_q(2, 3), create_q(4, 5)]
while True:
    print()
    print("1. Add a number from console")
    print("2. Add a random number")
    print("3. Display numbers")
    print("0. Exit")

    option = input(">")

    if option == "1":
        number_str = input("number: ")
        tokens = number_str.split("/")
        numerator = int(tokens[0])  # python str
        denominator = int(tokens[1])

        if denominator == 0:
            print("denominator cannot be 0")
        else:
            q = create_q(numerator, denominator)
            data.append(q)
    elif option == "2":
        numerator = randint(-10, 10)
        denominator = randint(1, 10)  # avoid generating 0 for denominator
        q = create_q(numerator, denominator)
        data.append(q)

    elif option == "3":
        print("the list is:")
        for q in data:
            print(to_str(q), end=' ')  # end is a named python parameter
    elif option == "0":
        print("bye!")
        break
    else:
        print("Bad command or file name!")
