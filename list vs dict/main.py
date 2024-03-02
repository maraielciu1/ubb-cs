# PROBLEM 4 AND 9
# Write the implementation for A5 in this file
#
from random import randint
"""
1.Read a list of complex numbers (in z = a + bi form) from the console.
2.Display the entire list of numbers on the console.
3.A Length and elements of a longest subarray of numbers that contain at most 3 distinct values.
  B The length and elements of a longest increasing subsequence, when considering each number's modulus
4.Exit the application.

"""
# 
# Write below this comment 
# Functions to deal with complex numbers -- list representation
# -> There should be no print or input statements in this section 
# -> Each function should do one thing only
# -> Functions communicate using input parameters and their return values
#

# def create_complex_number(a:str, b:str):
#    return [a,b]
# def get_real(number:list) ->str:
#    return number[0]
# def get_imaginary(number: list) ->str:
#     if float(number[1])>=0:
#         return "+"+str(number[1])
#     else:
#         return str(number[1])

#
# Write below this comment 
# Functions to deal with complex numbers -- dict representation
# -> There should be no print or input statements in this section 
# -> Each function should do one thing only
# -> Functions communicate using input parameters and their return values
#


def create_complex_number(a: str, b:str):
    return {"real":a,"imaginary":b}
def get_real(number:dict):
    return number["real"]
def get_imaginary(number: dict):
    if int(number["imaginary"])>=0:
         return "+"+str(number["imaginary"])
    else:
        return number["imaginary"]

def to_string(number) ->str:
    return "z="+str(get_real(number))+str(get_imaginary(number))+"i"
#
# Write below this comment 
# Functions that deal with subarray/subsequence properties
# -> There should be no print or input statements in this section 
# -> Each function should do one thing only
# -> Functions communicate using input parameters and their return values
#
def generate_random_numbers(n: int) -> list:
    result = []
    while n > 0:
        real = randint(-100,100)
        imaginary = randint(-100,100)
        number = create_complex_number(str(real),str(imaginary))
        result.append(number)
        n -=1
    return result

def separate_number(numbers_list_unsep: list):
    numbers_list = []
    for nr in numbers_list_unsep:
        if '+' in nr:
            parts = nr.split('+')
            real_part = parts[0].strip()
            imaginary_part = parts[1].replace('i', '').strip()
        elif '-' in nr:
            parts = nr.split('-')
            real_part = parts[0].strip()
            imaginary_part = str(-1 * int(parts[1].replace('i', '').strip()))
        numbers_list.append(create_complex_number(real_part,imaginary_part))
    return numbers_list
def add_new_numbers(numbers_list: list,separated_number: list):
    for nr in separated_number:
        numbers_list.append(nr)

def read_numbers_and_add(numbers_list: list,read_numbers: list):
    unsep_numbers_list = []
    sep_nb_list = []
    for number in read_numbers:
        unsep_numbers_list.append(number)
    sep_nb_list = separate_number(unsep_numbers_list)
    add_new_numbers(numbers_list,sep_nb_list)

def find_longest_subarray_complex(numbers_list):
    number_strings =[]
    max_length = 0
    result_subarray = []
    for nr in numbers_list:
        number_strings.append(to_string(nr))
    for length in range(3,len(number_strings)):
        for i in range(len(number_strings)-length+1):
            subarray = number_strings[i:i+length]
            different_counter = 0
            already_seen = []
            for nr in subarray:
                if nr not in already_seen:
                    different_counter +=1
                    already_seen.append(nr)
            if different_counter <= 3:
                if len(subarray)>max_length:
                    max_length = len(subarray)
                    result_subarray = subarray.copy()
    return max_length,result_subarray

def compute_modulus(number) -> float:
    return (float(get_real(number))**2+float(get_imaginary(number))**2)**0.5

def find_longest_subseq_modulus_dp(numbers_list):
    modulus_list = []
    max_length = 0
    result_subarray = [1]*len(numbers_list)
    result_subarray_str = ""
    for nr in numbers_list:
        modulus_list.append(compute_modulus(nr))
    for i in range(1,len(modulus_list)):
        for j in range(i):
            if modulus_list[i]>modulus_list[j] and result_subarray[i]<result_subarray[j]+1:
                result_subarray[i] = result_subarray[j]+1
    index = 0
    for i in range(len(result_subarray)):
        if max_length<result_subarray[i]:
            max_length = result_subarray[i]
            index = i
    current_modulus = modulus_list[index]
    copy_max_length = max_length
    for i in range(index,-1,-1):
        if result_subarray[i] == max_length and current_modulus >= modulus_list[i]:
            result_subarray_str = to_string(numbers_list[i])+" "+result_subarray_str+" "
            current_modulus = modulus_list[i]
            max_length -=1
    return copy_max_length,result_subarray_str



#
# Write below this comment 
# UI section
# Write all functions that have input or print statements here
# Ideally, this section should not contain any calculations relevant to program functionalities
def display_numbers_ui(numbers):
    for nr in numbers:
        print(to_string(nr))

def read_numbers_ui(numbers_list):
    read_numbers = [str(item) for item in input("Enter the list items : ").split()]
    read_numbers_and_add(numbers_list,read_numbers)

def display_longest_subarray_naive_ui(numbers_list):
    length, subarray = find_longest_subarray_complex(numbers_list)
    print("Length of the longest subarray:", length)
    print("Elements of the longest subarray:", subarray)

def display_longest_subseq_modulus_ui(numbers_list):
    length, subarray = find_longest_subseq_modulus_dp(numbers_list)
    print("Length of the longest subsequence:", length)
    print("Elements of the longest subsequence:", subarray)

def print_menu():
    print("1.Write a list of complex numbers (in z = a + bi form)")
    print("2.Display the entire list of numbers")
    print("3.A. Length and elements of a longest subarray of numbers that contain at most 3 distinct values")
    print("3.B. The length and elements of a longest increasing subsequence, when considering each number's modulus")
    print("4.Exit")
def start():
    numbers = generate_random_numbers(10)
    while True:
        print_menu()
        option = input(">")
        if option == "1":
            read_numbers_ui(numbers)
        elif option == "2":
            display_numbers_ui(numbers)
        elif option == "3":
            option_3 = input("Please choose one option (A or B): ")
            if option_3 == "A":
                display_longest_subarray_naive_ui(numbers)
            elif option_3 == "B":
                display_longest_subseq_modulus_ui(numbers)
            else:
                print("Please choose a valid option! (A or B)")
        elif option == "4":
            print("bye!")
            return
        else:
            print("Please choose a valid option! (1,2,3 or 4)")

if __name__ == "__main__":
    start()




