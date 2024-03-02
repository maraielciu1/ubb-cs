# Solve the problem from the second set here
"""
11. The numbers n1 and n2 have the property P if their writing in base 10 uses the
same digits (e.g. 2113 and 323121).
Determine whether two given natural numbers have property P.

"""
def listof_digits(number: int,digits: list):
    while number != 0:
        digits[number % 10] = 1
        number = number // 10


def compare(num1: list, num2: list) -> bool:
    for i in range(0,9):
        if num1[i] and num2[i] == False:
            return False
    return True

n = int(input("Give the first number: "))
m = int(input("Give the second number: "))

digitn = []
digitm = []

app_listn = [0,0,0,0,0,0,0,0,0,0]
app_listm = [0,0,0,0,0,0,0,0,0,0]

listof_digits(n,app_listn)
listof_digits(m,app_listm)

if compare(app_listn, app_listm) == True:
    print("the numbers have property P")
else:
    print("the numbers do not have property P")


