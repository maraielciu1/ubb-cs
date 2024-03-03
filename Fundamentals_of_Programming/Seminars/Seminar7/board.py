def create_board():
    """
    Create an empty TTT board
    :return: the board
    """
    """
    -10 --> empty square
    1 --> X
    0 --> O
    """
    board = []
    for i in range(3):
        # these are separate lists
        board.append([-10]*3)
    return board


def move(board, row: int, column: int ,symbol: int):
    """
    Record a move on the board
    :param board: the board
    :param symbol: one of [0,1] (means O or X)
    :return:
    Raise ValueError if:
    - symbol is invalid
    - outside of the board
    - coordinates not empty
    """
    if symbol not in [0,1]:
        raise ValueError("Invalid symbol!")
    if row not in (0,1,2) or column not in (0,1,2):
        raise ValueError("Move outside the board!")
    if board[row][column] != -10:
        raise ValueError("Coordinates not empty!")
    board[row][column] = symbol



def get(board, row: int, column: int):
    """
    Get the symbol at position (row,column)
    :param row:
    :param column:
    :return: 0 for O, 1 for X, -1 for empty
    Raise ValueError if:
    - outside of the board
    """
    if row not in (0,1,2) or column not in (0,1,2):
        raise ValueError("Cell outside the board!")
    return board[row][column]

def is_board_drawn(board) -> bool:
    """
    Return True is board is full and not won
    """
    if is_board_won(board):
        return False
    for i in range(3):
        for j in range(3):
            if board[i][j] == -10:
                return False
    return True

def is_board_won(board) -> bool:
    """
    Return true if board is won, false otherwise
    :param board:
    """

    # check rows for win
    for i in range(3):
        if sum(board[i]) in (0,3):
            return True

    # check columns for win
    # TODO Not very nicely written
    as_list = board[0]+board[1]+board[2]
    for j in range(3):
        if sum(as_list[j::3]) in (0,3):
            return True

    # check diagonals for win
    b = board
    if sum(([b[0][0],b[1][1],b[2][2]])) in (0,3):
        return True
    if sum(([b[2][0],b[1][1],b[2][0]])) in (0,3):
        return True
    return False

def _display(symbol: str) -> str:
    """
    How to display a symbol on the board
    _ means the function is private (should not be used outside the module)
    """
    if symbol == -10:
        return ' '
    elif symbol == 1:
        return 'X'
    elif symbol == 0:
        return 'O'

def to_str(board) -> str:
    d = _display  # alias the function with d
    result = ""
    for i in range(3):
        b = board[i]
        result += d(b[0])+"|"+d(b[1])+"|"+d(b[2])+"\n-----\n"
    # delete the last line of dashes
    return result[:-6]

def test_board():
    board = create_board()

    # test empty board
    for i in range(3):
        for j in range(3):
            # check board is empty
            assert get(board,i,j) == -10

    # make some moves
    move(board,1,1,1)
    assert get(board,1,1) == 1

    move(board, 0, 0, 0)
    assert get(board, 0, 0) == 0

    # try moving to an occupied square
    try:
        move(board, 1,1,1)
        assert False # if we get here, the function did not raise ValueError
    except ValueError:
        # we expect to get to this point
        assert True

    # try moving outside the board
    try:
        move(board,3,3,1)
        assert False # if we get here, the function did not raise ValueError
    except ValueError:
        # we expect to get to this point
        assert True

    # try moving with an invalid symbol
    try:
        move(board,2,2,2)
        assert False
    except ValueError:
        assert True

#test_board()
#

"""
1. How do I know this represents a board?
2. You can modify the board from everywhere
3. Board variable needs to be carried everywhere :(
"""


b = create_board()
b[1][1]= "abc"
print(type(b))
# move(b,1,1,1)
# move(b,0,0,0)
# move(b,2,2,1)
# print(to_str(b))
# print(is_board_won(b))