"""
Input: non-zero natural number n
Output: 1. the number of associative operations on set A = {a1, a2, ..., an}
        2. the operation tables for each associative operations n<=4
"""



all_pos_table = []
nr_associative_operations = 0
nr_associative_operations1 = 0


def create_check_table(n):
    """
    This function creates the initial table for the set {1, 2, ..., n} with all the possible combinations
    of 3 elements from the given set, which will be used to check if an operation is associative
    :param n: number of elements in the set
    """
    end = n + 1
    values = [0, 0, 0]
    for i in range(1, end):
        values[0] = i
        for j in range(1, end):
            values[1] = j
            for k in range(1, end):
                values[2] = k
                all_pos_table.append(values.copy())


def is_associative(table):
    for index in range(len(all_pos_table)):
        x = all_pos_table[index][0]
        y = all_pos_table[index][1]
        z = all_pos_table[index][2]

        # we "compute" the value of the operation (x*y)*z
        xy = table[x - 1][y - 1]
        xy_z = table[xy - 1][z - 1]

        # we "compute" the value of the operation x*(y*z)
        yz = table[y - 1][z - 1]
        x_yz = table[x - 1][yz - 1]

        # if the two values are different, the operation is not associative
        if xy_z != x_yz:
            return False
    return True


def det_assoc(table, n, k):
    """
    This function uses backtracking to determine
    all the possible operation tables for the set A with n elements
    """
    global nr_associative_operations
    if k >= n * n:
        return
    for i in range(0, n):
        table[k // n][k % n] = i + 1
        if k == n * n - 1:
            if is_associative(table):
                nr_associative_operations += 1
                output.write("")
                for x in range(len(table)):
                    for y in range(len(table[0])):
                        output.write(f"a{table[x][y]}")
                        output.write(" ")
                    output.write("\n")
                output.write("\n")

        else:
            det_assoc(table, n, k + 1)
def det_nr_assoc(table, n, k):
    """
    This function uses backtracking to determine
    the number of associative operations on the set A with n elements
    """
    global nr_associative_operations1
    if k >= n * n:
        return
    for i in range(0, n):
        table[k // n][k % n] = i + 1
        if k == n * n - 1:
            if is_associative(table):
                nr_associative_operations1 += 1
        else:
            det_nr_assoc(table, n, k + 1)

input = open('input.txt', 'r')
output = open('output.txt', 'w')
n = int(input.read())
input.close()

create_check_table(n)
table_op = [[0] * n for i in range(n)]

set = "{a1"
for i in range(1, n):
    set = set + ", a" + str(i + 1)
set += "}"
if n <= 4:
    output.write(f"2. on A = {set}, these are the following associative op.: \n")
    det_assoc(table_op, n, 0)
    output.write(f"1. the number of associative operations on set A = {set} is {nr_associative_operations} \n")
    output.close()
else:
    det_nr_assoc(table_op, n, 0)
    output.write(f"1. the number of associative operations on set A = {set} is {nr_associative_operations1} \n")
    output.close()

