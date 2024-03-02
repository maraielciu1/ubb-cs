"""
Dynamic Programming - 6
Given an array of integers A, maximize the value of the expression
A[m] - A[n] + A[p] - A[q], where m, n, p, q are array indices with m > n > p > q.
For A = [30, 5, 15, 18, 30, 40], the maximum value is 32, obtained as 40 - 18 + 15 - 5.
Display both the maximum value as well as the expression used to calculate it.

"""
import random
def create_random(n):
    array = []
    for i in range(n):
        array.append(random.randint(1,100))
    return array
def max_expression_naive(a: list) -> int:
    max_value = a[3] - a[2] + a[1] - a[0]
    max_value_expression = str(a[3]) + " - " + str(a[2]) + " + " + str(a[1]) + " - " + str(a[0])
    for q in range(len(a)-3):
        for p in range(q+1,len(a)-2):
            for n in range(p+1,len(a)-1):
                for m in range(n+1,len(a)):
                    if a[m] - a[n] + a[p] - a[q] > max_value:
                        max_value = a[m] - a[n] + a[p] - a[q]
                        max_value_expression = str(a[m]) + " - " + str(a[n]) + " + " + str(a[p]) + " - " + str(a[q])
    return max_value, max_value_expression

def max_expression_dynamic(a: list):
    if len(a) == 4:
        print(str(a[3] - a[2] + a[1] - a[0])+ ": "+str(a[3]) + " - " + str(a[2]) + " + " + str(a[1]) + " - " + str(a[0]))
    else:
        length = len(a)
        min_values_index = [0] * length
        max_values_index = [0] * length

        min_values_index[0] = 0
        for i in range(1,length):
            min_ind = 0
            for j in range(i):
                if a[j] < a[min_ind]:
                    min_ind = j
            min_values_index[i] = min_ind

        max_values_index[length-1] = length - 1
        for i in range(length-1):
            max_ind = length - 1
            for j in range(i+1,length):
                if a[j] > a[max_ind]:
                    max_ind = j
            max_values_index[i] = max_ind

        maximum_expresion_value = float("-inf")
        maximum_expresion_string = ""

        for i in range(length-1):
           for j in range(i+1,length):
                if min_values_index[i]!=i and max_values_index[j]!=j and min_values_index[i] < max_values_index[j]:
                    current_expr = a[i] - a[min_values_index[i]] +  a[max_values_index[j]]- a[j]
                    if current_expr > maximum_expresion_value:
                        maximum_expresion_value = current_expr
                        maximum_expresion_string = str(a[max_values_index[j]]) + " - " + str(a[j]) + " + " + str(a[i]) + " - " + str(a[min_values_index[i]])
        print(str(maximum_expresion_value)+": "+maximum_expresion_string)
        print("The data structures used for solving this problem in a dynamic way are 2 lists of length "+str(length))
        print("->the first list contains the index of the minimum value from the left to the right: ")
        print(min_values_index)
        print("->the second list contains the index of the maximum value from the right to the left: ")
        print(max_values_index)





n = int(input("Give the length of the array: "))
if n < 4:
    print("The length of the array must be greater than 4")
    exit(0)
array = create_random(n)
array1 = [88, 33, 2, 41, 77]
print(array)
print(max_expression_naive(array))
# print(max_expression_naive(array1))
max_expression_dynamic(array)
