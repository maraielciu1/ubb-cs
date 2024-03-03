from board import move, get


def choose_move(board):
    """
    Calculate the computer's move
    :param board:
    :return: (row,column) where the computer should move
    """
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