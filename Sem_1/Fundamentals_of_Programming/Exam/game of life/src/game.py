import copy

from src.board import Board


class GameError(Exception):
    pass


class Game:
    def __init__(self):
        self.__repo = Board()
        self.__pattern = None
        self.__dimension = 0

    def get_pattern(self, pattern):
        """
        sets self.__pattern according to the command received from the ui
        sets self.__dimension with the number of rows in the pattern
        :param pattern: the type of pattern that we have to find in the file
        :return:
        """
        dimension, symbols = self.find_pattern(pattern)
        self.__pattern = [[" " for i in range(dimension)] for j in range(dimension)]
        for symbol in symbols:
            self.__pattern[symbol[0]][symbol[1]] = "X"
        self.__dimension = dimension

    @property
    def pattern(self):
        return self.__pattern

    @property
    def dimension(self):
        return self.__dimension

    def find_pattern(self, pattern):
        """
        gets the dimension of the square matrix that will keep the pattern and the configuration.
        the configuration is transmited through the symbols list that keeps all the coordinates that are occupied
        :param pattern: the type of pattern
        :return: an int that represents the nr of rows, symbols list
        """
        with open("pattern.txt", "r") as f:
            found = False
            symbols = []
            rows = 0
            for line in f:
                if pattern in line.strip():
                    found = True
                    continue
                if found:
                    if line.strip() == "END":
                        break
                    row = list(line.strip())
                    for col, letter in enumerate(row):
                        if letter == "X":
                            symbols.append((rows, col))
                    rows += 1
            return rows, symbols

    def place(self, x, y):
        """
        function that places the pattern on the board, if possible
        raise GameError() if the pattern exceeds the board bounds or if you try to place it on a non empty position
        :param x: x coord
        :param y: y coord
        :return:
        """
        if 0 <= x + self.__dimension <= 8 and 0 <= y + self.__dimension <= 8:
            for i in range(len(self.__pattern)):
                for j in range(len(self.__pattern)):
                    if self.__pattern[i][j] == "X" and self.__repo.get_board()[x + i][y + j] == "X":
                        raise GameError("Invalid move. You cannot place a living cell over another living cell")
            for i in range(len(self.__pattern)):
                for j in range(len(self.__pattern)):
                    if self.__pattern[i][j] == "X" and self.__repo.get_board()[x + i][y + j] == " ":
                        self.__repo.place(x + i, y + j)
        else:
            raise GameError("Invalid move. The whole pattern must be placed inside the board")

    def tick(self, number):
        """
        this function checks every space in the current board and applies the rules to create/kill cells
        rules:
        - a live cell with less that 2 living neighbours, dies
        - a live cell with 2 or 3 living neighbours, lives
        - a live cell with more than 3 living neighbours, dies
        - a dead cell with 3 live neighbours, lives
        :param number: the number of 'tick' operations that need to be applied
        :return:
        """
        for k in range(number):
            # creating a copy of our current board bcs the checkigs are done on the original board
            current_board = copy.deepcopy(self.__repo.get_board())

            #initializing a new board on which we put the transformations
            new_board = [[" " for i in range(8)] for j in range(8)]

            for i in range(8):
                for j in range(8):
                    neighbors = self.__repo.count_neighbours(i, j)
                    if current_board[i][j] == "X" and neighbors in [2, 3]:
                        new_board[i][j] = "X"
                    elif current_board[i][j] == " " and neighbors == 3:
                        new_board[i][j] = "X"
                    else:
                        new_board[i][j] = " "

            self.__repo.set_board(new_board)

    def load_board(self):
        """
        loads the board from file
        :return:
        """
        return self.__repo.load_from_file()

    def save_board(self):
        """
        saves the board to the file
        :return:
        """
        return self.__repo.save_from_file()

    def get_board(self):
        """
        gets the board
        :return:
        """
        return self.__repo.get_board()

    def __str__(self):
        return str(self.__repo)






