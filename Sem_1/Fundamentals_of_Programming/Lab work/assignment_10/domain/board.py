class GameError(Exception):
    pass


class GameOverException(Exception):
    pass


class Board:
    """
    The game board is represented as a list of 24 elements,
    each element being either '*' (empty space), 'X' (computer_player piece) or 'O' (player piece).
    """
    def __init__(self):
        """
        Initialize the game board with 24 empty spaces
        """
        self.board = ['*' for _ in range(24)]
        self._open_spaces = [i for i in range(24)]
        self.adjacent_pos = {0: (1, 9),
                             1: (0, 2, 4),
                             2: (1, 14),
                             3: (4, 10),
                             4: (1, 3, 5, 7),
                             5: (4, 13),
                             6: (7, 11),
                             7: (4, 6, 8),
                             8: (7, 12),
                             9: (0, 21, 10),
                             10: (3, 9, 11, 18),
                             11: (6, 10, 15),
                             12: (8, 13, 17),
                             13: (5, 12, 14, 20),
                             14: (2, 13, 23),
                             15: (11, 16),
                             16: (15, 17, 19),
                             17: (12, 16),
                             18: (10, 19),
                             19: (16, 18, 20, 22),
                             20: (13, 19),
                             21: (9, 22),
                             22: (19, 21, 23),
                             23: (14, 22)}

        self.possible_mills = (
            (0, 1, 2), (3, 4, 5), (6, 7, 8), (9, 10, 11), (12, 13, 14), (15, 16, 17), (18, 19, 20), (21, 22, 23),
            (0, 9, 21), (3, 10, 18), (6, 11, 15), (1, 4, 7), (16, 19, 22), (8, 12, 17), (5, 13, 20), (2, 14, 23))

    def get_available_spaces1(self):
        """
        Get the list of available spaces on the board
        :return:
        """
        places = []
        for i in range(24):
            if self.board[i] == '*':
                places.append(i)
        return places

    def get_symbol_positions(self, symbol):
        """
        Get the list of positions occupied by the specified symbol
        :param symbol:
        :return:
        """
        pos = []
        for i in range(24):
            if self.board[i] == symbol:
                pos.append(i)
        return pos

    def __str__(self):
        """
        Return a string representation of the game board
        :return:
        """
        board_str = (
            f"{self.board[0]}(0)----------------------{self.board[1]}(1)----------------------{self.board[2]}(2)\n"
            "|                           |                           |\n"
            f"|       {self.board[3]}(3)--------------{self.board[4]}(4)-----------------{self.board[5]}(5)     |\n"
            "|       |                   |                    |      |\n"
            "|       |                   |                    |      |\n"
            f"|       |        {self.board[6]}(6)-----{self.board[7]}(7)-----{self.board[8]}(8)          |      |\n"
            "|       |         |                   |          |      |\n"
            "|       |         |                   |          |      |\n"
            f"{self.board[9]}(9)---{self.board[10]}(10)----{self.board[11]}(11)               {self.board[12]}(12)----{self.board[13]}(13)---{self.board[14]}(14)\n"
            "|       |         |                   |          |      |\n"
            "|       |         |                   |          |      |\n"
            f"|       |        {self.board[15]}(15)-----{self.board[16]}(16)-----{self.board[17]}(17)       |      |\n"
            "|       |                   |                    |      |\n"
            "|       |                   |                    |      |\n"
            f"|       {self.board[18]}(18)--------------{self.board[19]}(19)--------------{self.board[20]}(20)     |\n"
            "|                           |                           |\n"
            "|                           |                           |\n"
            f"{self.board[21]}(21)----------------------{self.board[22]}(22)----------------------{self.board[23]}(23)"
        )
        return board_str

    def move1(self, pos: int, symbol):
        """
        Place the piece at the specified position
        :param pos:
        :param symbol:
        :return:
        """
        # Place the piece at the specified position
        if pos in self.get_available_spaces1():
            self.board[pos] = symbol
        else:
            raise GameError("Invalid move")

    def move2(self, current_pos: int, next_pos: int, symbol):
        """
        Move the piece from current position to next position
        :param current_pos:
        :param next_pos:
        :param symbol:
        :return:
        """
        # Move the piece from current position to next position
        if next_pos in self.get_available_spaces1():
            self.board[current_pos] = '*'
            self.board[next_pos] = symbol
        else:
            raise GameError("Invalid move")

    def placement_of_current_pieces(self, symbol):
        """
        Get the list of positions occupied by the specified symbol
        :param symbol:
        :return:
        """
        pos = []
        for i in range(24):
            if self.board[i] == symbol:
                pos.append(i)
        return pos

    def is_game_over(self, symbol):
        """
        Check if the game is over
        :param symbol:
        :return:
        """
        for pos in range(len(self.board)):
            if self.board[pos] == symbol:
                adjacent_positions = self.adjacent_pos[pos]
                for adj_pos in adjacent_positions:
                    if self.board[adj_pos] == '*':
                        return False  # There's at least one valid move
        return True  # No valid moves, game is over
