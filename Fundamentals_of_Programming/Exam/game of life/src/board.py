import texttable


class Board:
    def __init__(self):
        self.__board = [[" " for i in range(8)] for j in range(8)]

    def __str__(self):
        header = ["/", "0", "1", "2", "3", "4", "5", "6", "7"]
        t = texttable.Texttable()
        t.header(header)
        for i in range(8):
            t.add_row([i] + self.__board[i])
        return t.draw()

    def load_from_file(self):
        """
        loads the board to file
        :return:
        """
        try:
            with open("board.txt", "r") as f:
                for i in range(8):
                    line = f.readline().strip()
                    for j in range(8):
                        if line[j] != "-":
                            self.__board[i][j] = line[j]
                        else:
                            self.__board[i][j] = " "
        except FileNotFoundError as e:
            return e

    def save_from_file(self):
        """
        saves the board in a file
        :return:
        """
        try:
            with open("board.txt", "w") as f:
                for i in range(8):
                    for j in range(8):
                        if self.__board[i][j] == " ":
                            f.write("-")
                        else:
                            f.write(self.__board[i][j])
                    f.write("\n")
        except FileNotFoundError as e:
            return e

    def get_board(self):
        return self.__board

    def place(self, x, y):
        """
        places a live cell at coords x,y
        :param x:
        :param y:
        :return:
        """
        self.__board[x][y] = "X"

    def count_neighbours(self, i, j):
        """
        counts the living neighbours of the element at the coords i,j
        :param i:
        :param j:
        :return: nr of living neighbours
        """
        count = 0
        for x in range(i - 1, i + 2):
            for y in range(j - 1, j + 2):
                if 0 <= x < 8 and 0 <= y < 8 and (x != i or y != j):
                    if self.__board[x][y] == "X":
                        count += 1
        return count

    def set_board(self, new_board):
        """
        replaces the old values from the board with the updated ones
        :param new_board:
        :return:
        """
        for i in range(8):
            for j in range(8):
                self.__board[i][j] = new_board[i][j]

