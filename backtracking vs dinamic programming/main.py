

"""
Backtracking - 6:
Generate all sequences of n parentheses that close correctly.
Example: for n=4 there are two solutions: (()) and ()()

"""

def generate_parentheses_rec(n):
    def back(array, open_count, close_count):
        if len(array) == n:  # we stop when the length of the solution reaches n
            valid_sequences.append(array)
            return
        if open_count < n/2 :
            # we can add an open parenthesis until open_count reaches n/2
            # because if the number exceeds the half of the wanted length,
            # then the solution is no longer valid
            back(array + '(', open_count + 1, close_count)
        if close_count < open_count:
            # we can add a closed parenthesis as long as close_count
            # doesn't exceed the number of already opened parenthesis
            back(array + ')', open_count, close_count + 1)

    valid_sequences = []
    back('', 0, 0)
    return valid_sequences

def generate_parentheses_iter(n):
    valid_sequences = []
    stack = []
    stack.append(("",0,0))
    while stack:
        array, open_count, close_count = stack.pop()
        if len(array) == n:
            valid_sequences.append(array)
        else:
            if open_count < n//2:
                stack.append((array+"(",open_count+1,close_count))
            if close_count < open_count:
                stack.append((array+")",open_count,close_count+1))
    return valid_sequences


n = int(input("> "))
if n % 2 == 1:
    print("There are no valid results for an odd number")
elif n == 0:
    print("None")
else:
    results = generate_parentheses_rec(n)
    for set in results:
        print(set)
    print()
    results = generate_parentheses_iter(n)
    for set in results:
        print(set)


