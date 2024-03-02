from random import randint
import timeit

def bubble_sort(array: list):
    """
    In bubble sort we compare the 2 elements in a pair and if their order is incorrect, we swap then.
    This operation is made for every 2 adjacent elements until the list is sorted.
    :param array: the unsorted initial list
    :param step: After how many operations do we print the list
    :return: the sorted list
    """
    for i in range(0, len(array)):
        for j in range(0, len(array) - i - 1):
            if array[j] > array[j + 1]:
                var = array[j]
                array[j] = array[j + 1]
                array[j + 1] = var

def bubble_sort_dec(array: list):
    """
    In bubble sort we compare the 2 elements in a pair and if their order is incorrect, we swap then.
    This operation is made for every 2 adjacent elements until the list is sorted.
    :param array: the unsorted initial list
    :param step: After how many operations do we print the list
    :return: the sorted list
    """
    for i in range(0, len(array)):
        for j in range(0, len(array) - i - 1):
            if array[j] < array[j + 1]:
                var = array[j]
                array[j] = array[j + 1]
                array[j + 1] = var

def comb_sort(array: list):
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
    gap = len(array)
    is_swapped = False
    while gap != 1 or is_swapped:
        is_swapped = False
        gap = max(1, int(gap / 1.3))
        for i in range(len(array) - gap):
            if array[i] > array[i + gap]:
                array[i], array[i + gap] = array[i + gap], array[i]
                is_swapped = True

def comb_sort_dec(array: list):
    gap = len(array)
    is_swapped = False
    while gap != 1 or is_swapped:
        is_swapped = False
        gap = max(1, int(gap / 1.3))
        for i in range(len(array) - gap):
            if array[i] < array[i + gap]:
                array[i], array[i + gap] = array[i + gap], array[i]
                is_swapped = True
def generate(n: int):
    array1 = []
    for i in range(0, n):
        array1.append(randint(0, 100))
    return array1

def prep_bc_bubble(array1: list,array2: list,array3: list,array4: list,array5: list):
    """
    In Bubble Sort the best case scenario is when the list is already sorted,
    so the amount of steps we have to make are just n, where n is the size of the list
    BC(n) = n => O(n)
    :param array1: the initial list
    :return: the sorted list in an increasing order
    """
    bubble_sort(array1)
    bubble_sort(array2)
    bubble_sort(array3)
    bubble_sort(array4)
    bubble_sort(array5)

def prep_wc_bubble(array1: list,array2: list,array3: list,array4: list,array5: list):
    """
    In Bubble Sort the worst case scenario is when the list is sorted but decreasing,
    so the amount of steps we have to make are not just n, where n is the size of the list,
    but n*n for each swap of the elements
    WC(n) = n*n => O(n^2)
    :param array1: the initial list
    :return: the list sorted decreasing
    """
    bubble_sort_dec(array1)
    bubble_sort_dec(array2)
    bubble_sort_dec(array3)
    bubble_sort_dec(array4)
    bubble_sort_dec(array5)

def prep_bc_comb(array1: list,array2: list,array3: list,array4: list,array5: list):
    """
    In Comb Sort the best case scenario is when the list is already sorted,
    so the loops of any gap size are computed once
    for gap = n -> steps = 1
        gap = n/(1.3) -> steps = n-n/(1.3)
        gap = n/(1.3)^2 -> steps = n-n/(1.3)^2
        ...
        gap = n/(1.3)^k -> steps = n-n/(1.3)^k, where k is how many times can we divide
        the gap by 1.3 till we get to 1
    n = (1.3)^k
    => (k+1)*n - n*sum of the powers of 1.3 from 1 to k
    => (logn+1)*n - n* ...
    => n*logn
    O(n*logn)
    :param array1: the initial list
    :return: the sorted list in an increasing order
    """
    comb_sort(array1)
    comb_sort(array2)
    comb_sort(array3)
    comb_sort(array4)
    comb_sort(array5)

def prep_wc_comb(array1: list,array2: list,array3: list,array4: list,array5: list):
    """
    In Comb Sort the worst case scenario is when the list is already sorted
    but in a decreasing order
    O(n*n) = O(n^2)
    :param array1: the initial list
    :return: the list sorted descending
    """
    comb_sort_dec(array1)
    comb_sort_dec(array2)
    comb_sort_dec(array3)
    comb_sort_dec(array4)
    comb_sort_dec(array5)

def main():
    array1 = []
    array2 = []
    array3 = []
    array4 = []
    array5 = []
    while True:
        print("Menu")
        print("1. Generate 5 lists of random natural numbers")
        print("2. Sort the list using bubble sort.")
        print("3. Sort the list using comb sort.")
        print("4. Exit the program")
        print("5. Best Case")
        print("6. Average Case")
        print("7. Worst Case")
        option = input(">")

        if option == "1":
            n1 = 25
            n2 = 125
            n3 = 625
            n4 = 3125
            n5 = 15625
            n6 = 78125

            array1 = generate(n1)
            copy_array1=array1.copy()
            array2 = generate(n2)
            copy_array2 = array2.copy()
            array3 = generate(n3)
            copy_array3 = array3.copy()
            array4 = generate(n4)
            copy_array4 = array4.copy()
            array5 = generate(n5)
            copy_array5 = array5.copy()

            copy_array = array1.copy()
            # print(array1)
        elif option == "2":
            if len(array1) == 0:
                print("Please choose option 1 first")
            else:
                    copy_array = array1.copy()
                    bubble_sort(copy_array)
                    print("The sorted array: ")
                    print(copy_array)
        elif option == "3":
            if len(array1) == 0:
                print("Please choose option 1 first")
            else:
                copy_array = array1.copy()
                comb_sort(copy_array)
                print("The sorted array: ")
                print(copy_array)
        elif option == "4":
            print("bye!")
            break
        elif option == "5":
            if len(array1) == 0:
                print("Please choose option 1 first")
            else:
                n = n1
                prep_bc_bubble(array1,array2,array3,array4,array5)
                for i in range(0,5):
                    if i == 0:
                        array = array1.copy()
                    elif i == 1:
                        array = array2.copy()
                    elif i == 2:
                        array = array3.copy()
                    elif i == 3:
                        array = array4.copy()
                    else:
                        array = array5.copy()
                    print("for "+str(n)+" elements, the time for bubble sort best case is  ->    ")
                    start_time = timeit.default_timer()
                    bubble_sort(array)
                    stop_time = timeit.default_timer()
                    print(stop_time-start_time)
                    n = n*5
                print()
                n = n1
                prep_bc_comb(array1,array2,array3,array4,array5)
                for i in range(0,5):
                    if i == 0:
                        array = array1.copy()
                    elif i == 1:
                        array = array2.copy()
                    elif i == 2:
                        array = array3.copy()
                    elif i == 3:
                        array = array4.copy()
                    else:
                        array = array5.copy()
                    print("and for comb sort with "+str(n)+" elements ->")
                    start_time = timeit.default_timer()
                    comb_sort(array)
                    stop_time = timeit.default_timer()
                    print(stop_time - start_time)
                    n = n*5


        elif option == "6":
            if len(array1) == 0:
                print("Please choose option 1 first")
            else:
                n = n1
                for i in range(0, 5):
                    if i == 0:
                        array = copy_array1.copy()
                    elif i == 1:
                        array = copy_array2.copy()
                    elif i == 2:
                        array = copy_array3.copy()
                    elif i == 3:
                        array = copy_array4.copy()
                    else:
                        array = copy_array5.copy()
                    print("for " + str(n) + " elements, the time for bubble sort worst case is  ->    ")
                    start_time = timeit.default_timer()
                    bubble_sort(array)
                    stop_time = timeit.default_timer()
                    print(stop_time - start_time)
                    n = n * 5
                print()

                n = n1
                for i in range(0, 5):
                    if i == 0:
                        array = array1.copy()
                    elif i == 1:
                        array = array2.copy()
                    elif i == 2:
                        array = array3.copy()
                    elif i == 3:
                        array = array4.copy()
                    else:
                        array = array5.copy()
                    print("and for comb sort with " + str(n) + " elements ->")
                    start_time = timeit.default_timer()
                    comb_sort(array)
                    stop_time = timeit.default_timer()
                    print(stop_time - start_time)
                    n = n * 5
        elif option == "7":
            if len(array1) == 0:
                print("Please choose option 1 first")
            else:
                n = n1
                prep_wc_bubble(array1, array2, array3, array4, array5)
                for i in range(0, 5):
                    if i == 0:
                        array = array1.copy()
                    elif i == 1:
                        array = array2.copy()
                    elif i == 2:
                        array = array3.copy()
                    elif i == 3:
                        array = array4.copy()
                    else:
                        array = array5.copy()
                    print("for " + str(n) + " elements, the time for bubble sort worst case is  ->    ")
                    start_time = timeit.default_timer()
                    bubble_sort(array)
                    stop_time = timeit.default_timer()
                    print(stop_time - start_time)
                    n = n * 5
                print()
                n = n1
                prep_wc_comb(array1, array2, array3, array4, array5)
                for i in range(0, 5):
                    if i == 0:
                        array = array1.copy()
                    elif i == 1:
                        array = array2.copy()
                    elif i == 2:
                        array = array3.copy()
                    elif i == 3:
                        array = array4.copy()
                    else:
                        array = array5.copy()
                    print("and for comb sort with " + str(n) + " elements ->")
                    start_time = timeit.default_timer()
                    comb_sort(array)
                    stop_time = timeit.default_timer()
                    print(stop_time - start_time)
                    n = n * 5
        else:
            print("Please choose a valid option (1, 2, 3, 4, 5, 6 or 7).")


if __name__ == "__main__":
    main()
