from random import randint
import timeit
from tabulate import tabulate

def bubble_sort(array):
    """
    Sorts the input list using the Bubble Sort algorithm.
    """
    for i in range(len(array)):
        is_swapped = False
        for j in range(len(array) - i - 1) :
            if array[j] > array[j + 1]:
                array[j], array[j + 1] = array[j + 1], array[j]
                is_swapped = True
        if is_swapped == False:
            break

def bubble_sort_descending(array):
    """
    Sorts the input list in descending order using Bubble Sort.
    """
    for i in range(len(array)):
        is_swapped = False
        for j in range(len(array) - i - 1):
            if array[j] < array[j + 1]:
                array[j], array[j + 1] = array[j + 1], array[j]
                is_swapped = True
        if is_swapped == False:
            break
def prep_bc(array):
    """
    Both in bubble sort and comb sort, the best case is achieved when the list is already sorted.
    the time complexity for bubble sort is O(n)
    the time complexity for comb sort is O(n*logn)

    Proof:
    bubble sort:
    the amount of steps we have to make are just n, where n is the size of the list
    => O(n)

    comb sort:
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
    => O(n*logn)
    """
    bubble_sort(array)

def prep_wc(array):
    """
    Both in bubble sort and comb sort, the worst case is achieved when the list is sorted but descending.
    the time complexity for bubble sort is O(n^2)
    the time complexity for comb sort is O(n^2)

    Proof:
    bubble sort:
    the amount of steps we have to make are not just n, where n is the size of the list,
    but n*n for each swap of the elements
    => O(n^2)

    comb sort:
    It occurs when the array elements are required to be sorted in reverse order.
    That means suppose you have to sort the array elements in ascending order, but its elements are in descending order.
    The worst-case time complexity
    => O(n^2)
    """
    bubble_sort_descending(array)

def comb_sort(array):
    """
    Sorts the input list using the Comb Sort algorithm.
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

def comb_sort_descending(array):
    """
    Sorts the input list in descending order using Comb Sort.
    """
    gap = len(array)
    is_swapped = False
    while gap != 1 or is_swapped:
        is_swapped = False
        gap = max(1, int(gap / 1.3))
        for i in range(len(array) - gap):
            if array[i] < array[i + gap]:
                array[i], array[i + gap] = array[i + gap], array[i]

def generate_random_list(size):
    """
    Generates a list of random natural numbers.
    """
    return [randint(0, 100) for _ in range(size)]

def main():
    random_lists = []
    list_sizes = [25, 125, 625, 3125, 15625]

    while True:
        print("Menu")
        print("1. Generate 5 lists of random natural numbers")
        print("2. Sort the list using bubble sort.")
        print("3. Sort the list using comb sort.")
        print("4. Exit the program")
        print("5. Best Case")
        print("6. Average Case")
        print("7. Worst Case")
        option = input("> ")

        if option == "1":
            random_lists = [generate_random_list(size) for size in list_sizes]
            # print("Generated lists:", random_lists)
        elif option == "2":
            if not random_lists:
                print("Please choose option 1 first")
            else:
                list_index = int(input("Enter the list index (0-4): "))
                if 0 <= list_index < len(random_lists):
                    list_copy = random_lists[list_index][:]
                    bubble_sort(list_copy)
                    print("Sorted array using bubble sort:", list_copy)
                else:
                    print("Invalid list index.")
        elif option == "3":
            if not random_lists:
                print("Please choose option 1 first")
            else:
                list_index = int(input("Enter the list index (0-4): "))
                if 0 <= list_index < len(random_lists):
                    list_copy = random_lists[list_index][:]
                    comb_sort(list_copy)
                    print("Sorted array using comb sort:", list_copy)
                else:
                    print("Invalid list index.")
        elif option == "4":
            print("bye!")
            break
        elif option == "5":
            if not random_lists:
                print("Please choose option 1 first")
            else:
                n = list_sizes[0]
                table = []
                print()
                for i in range(5):
                    array = random_lists[i][:]
                    prep_bc(array)
                    start_timebs = timeit.default_timer()
                    bubble_sort(array)
                    stop_timebs = timeit.default_timer()
                    elapsed_timebs = stop_timebs - start_timebs

                    prep_bc(array)
                    start_timecs = timeit.default_timer()
                    comb_sort(array)
                    stop_timecs = timeit.default_timer()
                    elapsed_timecs = stop_timecs - start_timecs
                    n = n*5
                    table.append([n//5, elapsed_timebs, elapsed_timecs])
                print("BEST CASE:")
                print(tabulate(table, headers=["Number of Elements", "Time (s) - Bubble sort", "Time (s) - Comb sort"], tablefmt="fancy_grid"))
                print()
        elif option == "6":
            if not random_lists:
                print("Please choose option 1 first")
            else:
                n = list_sizes[0]
                table = []
                print()
                for i in range(5):
                    array = random_lists[i][:]
                    start_timebs = timeit.default_timer()
                    bubble_sort(array)
                    stop_timebs = timeit.default_timer()
                    elapsed_timebs = stop_timebs - start_timebs

                    start_timecs = timeit.default_timer()
                    comb_sort(array)
                    stop_timecs = timeit.default_timer()
                    elapsed_timecs = stop_timecs - start_timecs
                    n = n * 5
                    table.append([n // 5, elapsed_timebs, elapsed_timecs])
                print("AVERAGE CASE:")
                print(tabulate(table, headers=["Number of Elements", "Time (s) - Bubble sort", "Time (s) - Comb sort"],tablefmt="fancy_grid"))
                print()
        elif option == "7":
            if not random_lists:
                print("Please choose option 1 first")
            else:
                n = list_sizes[0]
                table = []
                print()
                for i in range(5):
                    array = random_lists[i][:]
                    prep_wc(array)
                    start_timebs = timeit.default_timer()
                    bubble_sort(array)
                    stop_timebs = timeit.default_timer()
                    elapsed_timebs = stop_timebs - start_timebs

                    prep_wc(array)
                    start_timecs = timeit.default_timer()
                    comb_sort(array)
                    stop_timecs = timeit.default_timer()
                    elapsed_timecs = stop_timecs - start_timecs
                    n = n * 5
                    table.append([n // 5, elapsed_timebs, elapsed_timecs])
                print("WORST CASE:")
                print(tabulate(table, headers=["Number of Elements", "Time (s) - Bubble sort", "Time (s) - Comb sort"],tablefmt="fancy_grid"))
                print()
        else:
            print("Please choose a valid option (1, 2, 3, 4, 5, 6, or 7).")

if __name__ == "__main__":
    main()
