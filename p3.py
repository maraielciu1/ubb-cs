# Solve the problem from the third set here
"""

15. Generate the largest perfect number smaller than a given natural number n. If such
a number does not exist, a message should be displayed. A number is perfect if it is equal
to the sum of its divisors,
except itself. (e.g. 6 is a perfect number, as 6=1+2+3).

"""
def check_perfect(number: int) ->bool:
    sum = 0
    for i in range(1,number):
        if number % i == 0:
            sum += i
    if sum == number:
        return True
    else:
        return False

def smallest_large(number: int) ->int:
    for nr in range(n-1,0,-1):
        if check_perfect(nr) == True:
            return nr
            break
    return 0

n = int(input("Give a natural number: "))

result = smallest_large(n)

if result != 0:
    print(result)
else:
    print("such a number does not exist")

