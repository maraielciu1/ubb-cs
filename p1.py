# Solve the problem from the first set here
"""
4. For a given natural number n find the largest natural
number written with the same digits. (e.g. n=3658, m=8653).

"""
def create_digits(number: int, digits: list):
    while number!=0:
        digits.append(number%10)
        number = number // 10

def find_frequency(freq: list, digits: list):
    for i in range(0,len(digits)):
        freq[digits[i]] += 1

def new_number(new_n: int, freq: list) -> int:
    for i in range(9,-1,-1):
        while freq[i] != 0:
            new_n = new_n * 10 + i
            freq[i] -= 1
    return new_n

n = int(input("Give a natural number: "))


m = 0
digits = []
freq = [0,0,0,0,0,0,0,0,0,0]
create_digits(n,digits)
find_frequency(freq,digits)
print(new_number(m,freq))



