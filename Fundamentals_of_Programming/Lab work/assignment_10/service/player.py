from domain.board import Board, GameError


class Player:
    """
    This class represents the human player in the game
    """
    def __init__(self, name, symbol, board: Board):
        """
        Initialize the player with a name and a symbol
        :param name:
        :param symbol:
        :param board:
        """
        self.board = board
        self.__name = name
        self.__symbol = symbol
        self.remaining_pieces = 9  # Each player starts with 9 pieces
        self.placed_pieces = 0

    @property
    def name(self):
        return self.__name

    @property
    def symbol(self):
        return self.__symbol

    def move1(self, pos: int):
        """
        Place the piece at the specified position
        :param pos:
        :return:
        """
        if pos < 0 or pos > 23:
            raise GameError("Invalid position. Position must be between 0 and 23.")
        elif self.board.board[pos] != '*':
            raise GameError("Invalid position. The chosen position is already occupied.")
        self.board.move1(pos, self.symbol)
        self.remaining_pieces -= 1

    def move2(self, current_pos: int, next_pos: int):
        """
        Move the piece from current position to next position
        :param current_pos:
        :param next_pos:
        :return:
        """
        if current_pos < 0 or current_pos > 23 or next_pos < 0 or next_pos > 23:
            raise GameError("Invalid positions. Positions must be between 0 and 23.")
        elif self.board.board[current_pos] != self.symbol:
            raise GameError("Invalid position. You can only move your own piece.")
        elif next_pos not in self.board.adjacent_pos[current_pos]:
            raise GameError("Invalid move. You can only move to an adjacent position.")
        elif self.board.board[next_pos] != '*':
            raise GameError("Invalid move. The chosen position is already occupied.")
        else:
            self.board.move2(current_pos, next_pos, self.symbol)

    def move3(self, current_pos: int, next_pos: int):
        """
        Move the piece from current position to next position
        :param current_pos:
        :param next_pos:
        :return:
        """
        if current_pos < 0 or current_pos > 23 or next_pos < 0 or next_pos > 23:
            raise GameError("Invalid positions. Positions must be between 0 and 23.")
        elif self.board.board[current_pos] != self.symbol:
            raise GameError("Invalid position. You can only move your own piece.")
        elif self.board.board[next_pos] != '*':
            raise GameError("Invalid move. The chosen position is already occupied.")
        else:
            self.board.move2(current_pos, next_pos, self.symbol)

    def check_if_mill_is_formed(self, pos, symbol):
        """
        Check if a mill is formed at the specified position
        :param pos:
        :param symbol:
        :return:
        """
        board_copy = self.board.board.copy()
        for mill in self.board.possible_mills:
            if pos in mill:
                if all(board_copy[other_positions] == symbol for other_positions in mill):
                    return True
        return False

    def remove_piece_opponent(self, pos):
        """
        Remove the piece at the specified position
        :param pos:
        :return:
        """
        ok = False
        if not self.check_if_mill_is_formed(pos, 'X'):
            if self.board.board[pos] == 'X':
                self.board.board[pos] = '*'
                ok = True
        return ok

    def __str__(self):
        return f"{self.name} ({self.symbol}): {self.remaining_pieces} pieces remaining"
