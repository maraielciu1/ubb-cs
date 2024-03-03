#IELCIU MARA - 914

def check_echelon(matrix, rows, columns):
    last_pivot_column = -1

    for row in range(rows):
        pivot_column = 0

        # Find the leftmost non-zero entry in the current row
        while pivot_column < columns and matrix[row][pivot_column] == 0:
            pivot_column += 1

        if pivot_column <= last_pivot_column and pivot_column < columns:
            return False

        last_pivot_column = pivot_column

        if pivot_column < columns:
            # Check if the entries above and below the pivot are zero
            for i in range(rows):
                if i != row and matrix[i][pivot_column] != 0:
                    return False

    return True


def print_matrix(matrix, output_file):
    for row in matrix:
        output_file.write(" ".join(map(str, row)) + "\n")
    output_file.write("\n")


def generate_matrices(rows, columns):
    global counter
    matrices = [[0] * columns for _ in range(rows)]

    def create_matrix(k):
        global counter

        if k >= rows * columns:
            return

        for i in range(2):
            matrices[k // columns][k % columns] = i

            if k == rows * columns - 1 and check_echelon(matrices, rows, columns):
                counter += 1
                print_matrix(matrices, output_file)

            create_matrix(k + 1)

    create_matrix(0)


input_file = open('input.txt', 'r')
output_file = open('output.txt', 'w')

m = int(input_file.readline(10))
n = int(input_file.readline(10))
input_file.close()

counter = 0

output_file.write("2. The matrices in reduced echelon form are:\n")

generate_matrices(m, n)
output_file.write(f"1. For m = {m} and n = {n}, the number of matrices in reduced echelon form is {counter}.\n")
output_file.close()
