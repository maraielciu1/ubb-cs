# store the first 2 values of the sequence in a dictionary
cache = {0: 0, 1: 1}


def fib(n: int) -> int:
    """
    Principle of optimality - fib(n-k) is the solution for finding the n-k'th term of the sequence
    Overlapping subproblems - to calculate the n-th term we need to calculate all the previous terms
    Memoization - building the dictionary of the previous values
    """

    # The basic case
    # T(n) = 2 * T(n-1) + 1 => O(2^n)
    # if n < 2:
    #    return n

    # the brave new basic case :)
    if n in cache:
        return cache[n]
    # T(n) = n , as long as cache operations are O(1)
    cache[n] = fib(n - 2) + fib(n - 1)
    return cache[n]


print(fib(10))

"""
1. Calculate the maximum subarray sum (subarray = elements having 
continuous indices)

    e.g.
    for data = [-2, -5, 6, -2, -3, 1, 5, -6], maximum subarray sum is 7.
"""


def maximum_sum(data: list) -> int:
    """
    Principle of optimality
    Overlapping subproblems - we have the array's elements
    Memoization - current_sum
    """
    current_sum = data[0]
    max_sum = data[0]

    for i in range(1, len(data)):
        if current_sum + data[i] > data[i]:
            current_sum += data[i]
        else:
            current_sum = data[i]

    # equivalent code to the one below

    for i in range(1, len(data)):
        if current_sum > 0:
            current_sum += data[i]
        else:
            current_sum = data[i]
        max_sum = max(max_sum, current_sum)


"""
2. Knapsack problem. Given the weights and values of N items, put them in a knapsack having capacity W so that you
   maximize the value of the stored items. Items can be broken up
"""
# for greedy we need to sort the items by value/weight ratio
"""
For example, consider the coin denominations [1, 5, 10, 25] and the target amount of 30. 
The greedy algorithm would select a 25-cent coin, followed by a 5-cent coin, resulting in a total of 
two coins. 
However, the optimal solution is to use three 10-cent coins, which yields a total of three coins.
"""

def knapsack_naive_impl(W: int, weights: list, values: list, index: int) -> int:
    # T(n) = 2 * T(n-1) + 1 => O(2^n), n - number of objects
    if index < 0:
        # out of objects
        return 0

    # value if we add the object index
    include_value = 0
    # we can add an object only if it fits in the knapsack
    if W - weights[index] >= 0:
        include_value = values[index] + knapsack_naive_impl(W - weights[index], weights, values, index - 1)
    # value if we don't add the object index
    exclude_value = knapsack_naive_impl(W, weights, values, index - 1)
    return max(include_value, exclude_value)


def knapsack_naive(W: int, weights: list, values: list) -> int:
    return knapsack_naive_impl(W, weights, values, len(weights) - 1)


"""
    Principle of optimality - 
    Overlapping subproblems - 
    Memoization

    with dynamic programming:
    time complexity
         T(n) = W * n => O(W * n) (size of knapsack * number of objects)
    space complexity
         T(n,W) = W (size of knapsack) (because we never look back more than 1 row in the table, and that can be overwritten)

"""

W = 11
weights = [1, 2, 3, 3, 5, 6]
values = [2, 3, 1, 5, 3, 2]
print(knapsack_naive(W, weights, values))

# counter-example for using greedy in the 0-1 scenario (items cannot be broken up)
# W = 6
# values = [5,3,3]
# weights = [4,3,3]