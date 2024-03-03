
"""
from board import move, get


def choose_move(board):

    Calculate the computer's move
    :param board:
    :return: (row,column) where the computer should move

    # TODO Decide where the computer should move
    # FIXME Remove this hard codinng
    for i in range(3):
        for j in range(3):
            if get(board,i,j) == -10:
                return i,j
    raise ValueError("The board is full!")


def move_human(board, row: int, column: int):
    move(board, row, column, 1)

def move_computer(board):
    row,column = choose_move(board)
    move(board, row, column, 0)
"""
from board import Board

class ComputerPlayerFirstSquare:
    def __init__(self,board):
       self.__board = board
    def move(self):
        """
        Calculate the computer's move
        :return: (row, column)where the computer should move
        """

        # TODO Decide where the computer should move
        # FIXME Remove this hard codinng
        for i in range(3):
            for j in range(3):
                if self.__board.get(i, j) == -10:
                    self.__board.move(i,j,0) # computer plays with  0
                    return i, j

class ComputerPlayerSmarter:
    def __init__(self,board):
       self.__board = board
    def move(self):
        # TODO implement a smarter move
        pass

class Game:
    def __init__(self,board: Board,computer_level):
        self.__board = board
        self.__ai = computer_level
    def get_board(self):
        return self.__board
    def move_human(self,row,column):
        self.__board.move(row,column,1)
    def move_computer(self):
        return self.__ai.move()

