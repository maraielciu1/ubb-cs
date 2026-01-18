import time
import random


def gcd_euclid_iterative(a, b):
    """ Compute the GCD of two natural numbers iteratively using the Euclidean Algorithm.
    Returns:
        The GCD of a and b
    """
    while b:
        a, b = b, a % b
    return a


def gcd_repeated_subtractions(a, b):
    """ Compute the GCD of two natural numbers by repeated subtractions

    Returns:        The GCD of a and b
    """
    if a == 0:
        return b

    if b == 0:
        return a

    while a != b:
        if a > b:
            a -= b
        else:
            b -= a
    return a


def extended_euclidean(a, b):
    """
    Extended Euclidean Algorithm.
    Returns d = gcd(a, b), and integers u, v such that au + bv = d.
    """
    u2, u1 = 1, 0
    v2, v1 = 0, 1

    while b > 0:
        q = a // b
        r = a - q * b
        u = u2 - q * u1
        v = v2 - q * v1

        a, b = b, r
        u2, u1 = u1, u
        v2, v1 = v1, v

    d = a
    u = u2
    v = v2
    return d, u, v


fixed_inputs = [
    (0, 0),
    (1, 1),
    (17, 31),
    (56, 98),
    (270, 192),
    (81, 27),
    (100, 25),
]

# random medium and large test cases for performance
random_inputs = [
    (random.randint(1, 10**4), random.randint(1, 10**4)) for _ in range(5)
] + [
    (random.randint(10**6, 10**9), random.randint(10**6, 10**9)) for _ in range(5)
]

inputs = fixed_inputs + random_inputs

for a, b in inputs:
    start_time = time.time()
    gcd, u, v = extended_euclidean(a, b)
    end_time = time.time()
    print(f"extended euclidean for ({a}, {b}) = {gcd} in {end_time - start_time:.30f} seconds")

    start_time = time.time()
    gcd = gcd_euclid_iterative(a, b)
    end_time = time.time()
    print(f"euclid iterative for ({a}, {b}) = {gcd} in {end_time - start_time:.30f} seconds")

    start_time = time.time()
    gcd = gcd_repeated_subtractions(a, b)
    end_time = time.time()
    print(f"repeated subtractions for ({a}, {b}) = {gcd} in {end_time - start_time:.30f} seconds")

    print("\n")

    #problema 1 
