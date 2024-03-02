from random import randint


def bubble_sort(array: list, step: int):
    """
    In bubble sort we compare the 2 elements in a pair and if their order is incorrect, we swap then.
    This operation is made for every 2 adjacent elements until the list is sorted.
    :param array: the unsorted initial list
    :param step: After how many operations do we print the list
    :return: the sorted list
    """
    steps_made = 0
    multiply = 1
    for i in range(0, len(array)):
        for j in range(0, len(array) - i - 1):
            if array[j] > array[j + 1]:
                var = array[j]
                array[j] = array[j + 1]
                array[j + 1] = var
                steps_made += 1
                if steps_made == step:
                    print("The array at step: " + str(multiply * step))
                    multiply += 1
                    print(array)
                    steps_made = 0

def comb_sort(array: list, step: int):
    """
    Here's how comb sort works:

    1.Choose a gap value, which is initially set to the length of the list (or array).
    2.Compare elements that are "gap" positions apart. Swap them if they are out of order.
    3.Reduce the gap value by a shrink factor (usually around 1.3, but it can vary), which effectively reduces
    the gap between elements being compared. The gap value is updated in each iteration.
    4.Repeat steps 2 and 3 until the gap becomes 1.
    5.Continue to perform steps 2 and 3 with a gap of 1 until no more swaps are
    needed in a pass. This indicates that the list is sorted.
    :param array: the unsorted initial list
    :param step: After how many operations do we print the list
    :return: the sorted list
    """
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

def generate(n: int):
    array1 = []
    for i in range(0, n):
        array1.append(randint(0, 100))
    return array1


def main():
    array1 = []

    while True:
        print("Menu")
        print("1. Generate a list of n random natural numbers")
        print("2. Sort the list using bubble sort.")
        print("3. Sort the list using comb sort.")
        print("4. Exit the program")
        option = input(">")

        if option == "1":
            n = int(input("Please give the length of the random generated list: "))
            array1 = generate(n)
            copy_array = array1.copy()
            print(array1)
        elif option == "2":
            if len(array1) == 0:
                print("Please choose option 1 first")
            else:
                step = int(input(
                    "Please choose a number that represents how does the array look at that step of the processing and so on "))
                if step <= 0:
                    print("Please choose a step that is a positive integer")
                elif step > len(array1):
                    print("The given step is bigger than the number of elements")
                else:
                    copy_array = array1.copy()
                    bubble_sort(copy_array, step)
                    print("The sorted array: ")
                    print(copy_array)
        elif option == "3":
            if len(array1) == 0:
                print("Please choose option 1 first")
            else:
                step = int(input(
                    "Please choose a number that represents how does the array look at that step of the processing and so on "))
                if step <= 0:
                    print("Please choose a step that is a positive integer")
                elif step > len(array1):
                    print("The given step is bigger than the number of elements")
                else:
                    copy_array = array1.copy()
                    comb_sort(copy_array, step)
                    print("The sorted array: ")
                    print(copy_array)
        elif option == "4":
            print("bye!")
            break
        else:
            print("Please choose a valid option (1, 2, 3, or 4).")


if __name__ == "__main__":
    main()
