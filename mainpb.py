"""

Assigment 2
Menu:
1. Generate a list of n random natural numbers. Generated numbers must be between 0 and 100.
2. Sort the list using bubble sort.
3. Sort the list using comb sort.
4. Exit the program

"""
from random import randint

def bubble_sort(array: list, step: int):
    steps_made = 0
    multiply = 1
    for i in range(0,len(array)):
        for j in range(0,len(array)-i-1):
            if array[j] > array[j+1]:
                var = array[j]
                array[j] = array[j+1]
                array[j+1] = var
                steps_made += 1
                if steps_made == step:
                    print("The array at step: " + str(multiply*step))
                    multiply += 1
                    print(array)
                    steps_made = 0

def comb_sort(array: list, step: int):
    steps_made = 0
    multiply = 1
    gap = len(array)
    is_swapped = False
    while gap != 1 or is_swapped:
        is_swapped = False
        gap = max(1, int(gap / 1.3))
        for i in range(len(array) - gap):
            if array[i] > array[i + gap]:
                array[i], array[i + gap] = array[i + gap], array[i]
                steps_made += 1
                if steps_made == step:
                    print("The array at step:", multiply * step)
                    multiply += 1
                    print(array)
                    steps_made = 0
                is_swapped = True

def generate(n: int, array1: list):
    for i in range(0,n):
        array1.append(randint(0,100))

while True:
    print("Menu")
    print("1. Generate a list of n random natural numbers")
    print("2. Sort the list using bubble sort. ")
    print("3. Sort the list using comb sort. ")
    print("4. Exit the program")
    option = input(">")
    if option != "1" and option != "2" and option != "3" and option != "4":
        print("Please choose a valid option")
    elif option == "1":
        ok = True
        n = int(input("Please give the length of the random generated list: "))
        array1 = []
        generate(n,array1)
        copy_array = array1.copy()
        print(array1)
    elif option == "2":
        if len(array1) == 0:
            print("Please choose option 1 first")
        else:
            step = int(input("Please choose a number that represents how does the array look at that step of the processing and so on "))
            if step <= 0:
                print("Please choose a step that is a positive integer")
            elif step > n:
                print("The given step is bigger than the number of elements")
            else:
                bubble_sort(copy_array,step)
                print("The sorted array: ")
                print(copy_array)
                copy_array = array1.copy()
    elif option == "3":
        if len(array1) == 0:
            print("Please choose option 1 first")
        else:
            step = int(input(
                "Please choose a number that represents how does the array look at that step of the processing and so on "))
            if step <= 0:
                print("Please choose a step that is a positive integer")
            elif step > n:
                print("The given step is bigger than the number of elements")
            else:
                comb_sort(copy_array, step)
                print("The sorted array: ")
                print(copy_array)
                copy_array = array1.copy()
    elif option == "4":
        print("bye!")
        break