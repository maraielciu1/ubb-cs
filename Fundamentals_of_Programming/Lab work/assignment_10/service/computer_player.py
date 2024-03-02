from random import choice

from domain.board import Board, GameError, GameOverException


class ComputerPlayer:
    """
    This class represents the computer player in the game
    """

    def __init__(self, game_board: Board):
        """
        Initialize the computer player with a name and a symbol
        :param game_board:
        """
        self.__name = "AI"
        self.__symbol = 'X'
        self.remaining_pieces = 9  # Each player starts with 9 pieces
        self.placed_pieces = 0
        self.board = game_board

    @property
    def symbol(self):
        return self.__symbol

    @property
    def name(self):
        return self.__name

    def __str__(self):
        return f"{self.name} ({self.symbol}): {self.remaining_pieces} pieces remaining"

    def move1(self):
        return self.board.get_available_spaces1()


class RandomMovePlayer(ComputerPlayer):
    def move1(self):
        if len(self.board.get_available_spaces1()) == 0 or self.remaining_pieces == 0:
            raise GameError("No more moves available for stage 1")
        pos = choice(self.board.get_available_spaces1())
        self.board.move1(pos, self.symbol)  # Make the move on the board
        self.remaining_pieces -= 1


class NotSoRandomPlayer(ComputerPlayer):
    """
    points system for stage 1:
    1. if you can get morris -> get morris 500 p
    2. if you can block opponent morris -> block 400 p
    3. if you can get 2 in a row -> get 2 in a row 300 p
    4. if you can block opponent 2 in a row -> block 200 p
    5. if you can get 1 in a row -> get 1 in a row (random) 100 p

    points system for removing opponent piece:
    1. if opponent has morris -> remove piece 500 p (but only if all pieces are in morris)
    2. if opponent has 2 in a row -> remove piece 400 p
    3. if opponent blocked your morris -> remove piece 300 p
    4. if opponent has 1 in a row -> remove piece 200 p

    """

    def check_morris(self, pos):
        """
        Check if a mill is formed at the specified position
        :param pos:
        :return:
        """
        board_copy = self.board.board.copy()
        board_copy[pos] = self.symbol
        for mill in self.board.possible_mills:
            if pos in mill:
                ok = True
                for pl in mill:
                    if board_copy[pl] != self.symbol:
                        ok = False
                if ok:
                    return True
        return False

    def check_morris_opponent(self, pos):
        """
        Check if a mill is formed at the specified position
        :param pos:
        :return:
        """
        board_copy = self.board.board.copy()
        opponent_symbol = 'O' if self.symbol == 'X' else 'X'
        board_copy[pos] = opponent_symbol

        for mill in self.board.possible_mills:
            count_opponent_pieces = sum(1 for pl in mill if board_copy[pl] == opponent_symbol)
            if count_opponent_pieces == 3:
                return True

        return False

    def can_get_morris1(self):
        """
        Check if the player can get a mill
        :return:
        """
        pos = -1
        nr_points = 0
        available_pos = self.board.get_available_spaces1()
        for av in available_pos:
            yes = self.check_morris(av)
            if yes:
                pos = av
                nr_points = 500
        return pos, nr_points

    def can_block_morris1(self):
        """
        Check if the player can block the opponent's mill
        :return:
        """
        pos = -1
        nr_points = 0
        available_pos = self.board.get_available_spaces1()
        for av in available_pos:
            yes = self.check_morris_opponent(av)
            if yes:
                pos = av
                nr_points = 400
        return pos, nr_points

    def can_get_2_in_a_row1(self):
        """
        Check if the player can get 2 in a row
        :return:
        """
        pos = -1
        nr_points = 0
        available_pos = self.board.get_available_spaces1()
        for av in available_pos:
            adj = self.board.adjacent_pos[av]
            for a in adj:
                if self.board.board[a] == self.symbol:
                    for another_adj in adj:
                        if another_adj != a and another_adj != av and another_adj != self.symbol:
                            pos = av
                            nr_points = 50
                        else:
                            pos = av
                            nr_points = 300
        return pos, nr_points

    def can_block_2_in_a_row1(self):
        """
        Check if the player can block the opponent's 2 in a row
        :return:
        """
        pos = -1
        nr_points = 0
        available_pos = self.board.get_available_spaces1()
        for av in available_pos:
            adj = self.board.adjacent_pos[av]
            for a in adj:
                if self.board.board[a] == 'O' if self.symbol == 'X' else 'X':
                    pos = av
                    nr_points = 200
        return pos, nr_points

    def can_get_1_in_a_row1(self):
        """
        Check if the player can get 1 in a row
        :return:
        """
        pos = -1
        nr_points = 0
        available_pos = self.board.get_available_spaces1()
        pos = choice(available_pos)
        nr_points = 100
        return pos, nr_points

    def points_system_move_stage1(self):
        """
        Points system for stage 1
        :return:
        """
        pos = -1
        nr_points = 0
        pos1, nr_points1 = self.can_get_morris1()
        pos2, nr_points2 = self.can_block_morris1()
        pos3, nr_points3 = self.can_get_2_in_a_row1()
        pos4, nr_points4 = self.can_block_2_in_a_row1()
        pos5, nr_points5 = self.can_get_1_in_a_row1()
        if nr_points1 > nr_points:
            pos = pos1
            nr_points = nr_points1
        if nr_points2 > nr_points:
            pos = pos2
            nr_points = nr_points2
        if nr_points3 > nr_points:
            pos = pos3
            nr_points = nr_points3
        if nr_points4 > nr_points:
            pos = pos4
            nr_points = nr_points4
        if nr_points5 > nr_points:
            pos = pos5
            nr_points = nr_points5
        return pos

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

    def check_all_pieces_in_mill(self, symbol):
        """
        Check if all pieces are in a mill
        :param symbol:
        :return:
        """
        board_copy = self.board.board.copy()
        position_symbol = self.board.get_symbol_positions(symbol)
        if len(position_symbol) == 0:
            return False
        else:
            for pos in position_symbol:
                for mill in self.board.possible_mills:
                    if pos in mill:
                        for other_positions in mill:
                            if board_copy[
                                other_positions] == symbol and other_positions != pos and other_positions in position_symbol:
                                position_symbol.remove(other_positions)
            if len(position_symbol) == 0:
                return True
            else:
                return False

    def remove_piece_from_mill(self, symbol):
        """
        Remove a piece from a mill
        :param symbol:
        :return:
        """
        check = self.check_all_pieces_in_mill(symbol)
        pos = -1
        points = 0
        if check:
            position_symbol = self.board.get_symbol_positions(symbol)
            pos = choice(position_symbol)
            self.board.board[pos] = '*'
            points = 500
        return pos, points

    def remove_piece_2_in_a_row(self, symbol):
        """
        Remove a piece from a 2 in a row
        :param symbol:
        :return:
        """
        pos = -1
        points = 0
        position_symbol = self.board.get_symbol_positions(symbol)
        for pos in position_symbol:
            adj = self.board.adjacent_pos[pos]
            for a in adj:
                if self.board.board[a] == symbol:
                    for another_adj in adj:
                        if another_adj != a and another_adj != pos and another_adj != symbol:
                            pos = a
                            points = 400
        return pos, points

    def remove_piece_blocked_mill(self, symbol):
        """
        Remove a piece from a blocked mill
        :param symbol:
        :return:
        """
        pos = -1
        points = 0
        position_symbol = self.board.get_symbol_positions(symbol)
        for pos in position_symbol:
            adj = self.board.adjacent_pos[pos]
            for a in adj:
                if self.board.board[a] == 'O' if symbol == 'X' else 'X':
                    pos = a
                    points = 300
        return pos, points

    def remove_piece_1_in_a_row(self, symbol):
        """
        Remove a piece from a 1 in a row
        :param symbol:
        :return:
        """
        pos = -1
        points = 0
        position_symbol = self.board.get_symbol_positions(symbol)
        pos = choice(position_symbol)
        points = 200
        return pos, points

    def points_system_remove(self):
        """
        Points system for removing opponent piece
        :return:
        """
        points = 0
        pos = -1
        pos1, points1 = self.remove_piece_from_mill('O' if self.symbol == 'X' else 'X')
        pos2, points2 = self.remove_piece_2_in_a_row('O' if self.symbol == 'X' else 'X')
        pos3, points3 = self.remove_piece_blocked_mill('O' if self.symbol == 'X' else 'X')
        pos4, points4 = self.remove_piece_1_in_a_row('O' if self.symbol == 'X' else 'X')

        if points1 > points:
            pos = pos1
            points = points1
        if points2 > points:
            pos = pos2
            points = points2
        if points3 > points:
            pos = pos3
            points = points3
        if points4 > points:
            pos = pos4
            points = points4
        return pos

    def remove_piece_opponent(self):
        """
        Remove a piece from the opponent
        :return:
        """
        if len(self.board.get_symbol_positions('O' if self.symbol == 'X' else 'X')) == 0:
            raise GameError("No more pieces to remove")
        pos = self.points_system_remove()
        self.board.board[pos] = '*'

    def move1(self):
        """
        Make a move in stage 1
        :return:
        """
        if len(self.board.get_available_spaces1()) == 0 or self.remaining_pieces == 0:
            raise GameError("No more moves available for stage 1")
        pos = self.points_system_move_stage1()
        self.board.move1(pos, self.symbol)
        self.remaining_pieces -= 1
        if self.check_if_mill_is_formed(pos, self.symbol):
            self.remove_piece_opponent()

    """
    points system for stage 2:
    1. if you can get morris -> get morris 500 p
    2. if you can block opponent morris -> block 400 p
    3. if you can get 2 in a row -> get 2 in a row 300 p
    4. if you can block opponent 2 in a row -> block 200 p
    5. if you can get 1 in a row -> get 1 in a row (random) 100 p
    6. if you can't, move from morris 50 p
    """

    def can_get_morris2(self, current_position):
        """
        Check if the player can get a mill
        :param current_position:
        :return:
        """
        pos = -1
        nr_points = 0
        adjacent_positions = self.board.adjacent_pos[current_position]
        for adj in adjacent_positions:
            if self.board.board[adj] == '*':
                yes = self.check_morris(adj)
                if yes:
                    pos = adj
                    nr_points = 500
        return pos, nr_points

    def can_block_morris2(self, current_position):
        """
        Check if the player can block the opponent's mill
        :param current_position:
        :return:
        """
        pos = -1
        nr_points = 0
        adjacent_positions = self.board.adjacent_pos[current_position]
        for adj in adjacent_positions:
            if self.board.board[adj] == '*':
                yes = self.check_morris_opponent(adj)
                if yes:
                    pos = adj
                    nr_points = 400
        return pos, nr_points

    def can_get_2_in_a_row2(self, current_position):
        """
        Check if the player can get 2 in a row
        :param current_position:
        :return:
        """
        pos = -1
        nr_points = 0
        adjacent_positions = self.board.adjacent_pos[current_position]
        for adj in adjacent_positions:
            if self.board.board[adj] == '*':
                adj2 = self.board.adjacent_pos[adj]
                for a in adj2:
                    if self.board.board[a] == self.symbol:
                        pos = adj
                        nr_points = 300
        return pos, nr_points

    def can_block_2_in_a_row2(self, current_position):
        """
        Check if the player can block the opponent's 2 in a row
        :param current_position:
        :return:
        """
        pos = -1
        nr_points = 0
        adjacent_positions = self.board.adjacent_pos[current_position]
        for adj in adjacent_positions:
            if self.board.board[adj] == '*':
                adj2 = self.board.adjacent_pos[adj]
                for a in adj2:
                    if self.board.board[a] == 'O' if self.symbol == 'X' else 'X':
                        pos = adj
                        nr_points = 200
        return pos, nr_points

    def can_get_1_in_a_row2(self, current_position):
        """
        Check if the player can get 1 in a row
        :param current_position:
        :return:
        """
        pos = -1
        nr_points = 0
        adjacent_positions = self.board.adjacent_pos[current_position]
        for adj in adjacent_positions:
            if self.board.board[adj] == '*':
                pos = adj
                nr_points = 100
        return pos, nr_points

    def points_system_move_stage2(self):
        """
        Points system for stage 2
        :return:
        """
        pos = -1
        nr_points = 0
        start_pos = -1
        for i_pos in self.board.get_symbol_positions(self.symbol):
            current_position = i_pos
            pos1, nr_points1 = self.can_get_morris2(current_position)
            pos2, nr_points2 = self.can_block_morris2(current_position)
            pos3, nr_points3 = self.can_get_2_in_a_row2(current_position)
            pos4, nr_points4 = self.can_block_2_in_a_row2(current_position)
            pos5, nr_points5 = self.can_get_1_in_a_row2(current_position)
            if nr_points1 > nr_points:
                pos = pos1
                nr_points = nr_points1
                start_pos = current_position
            if nr_points2 > nr_points:
                pos = pos2
                nr_points = nr_points2
                start_pos = current_position
            if nr_points3 > nr_points:
                pos = pos3
                nr_points = nr_points3
                start_pos = current_position
            if nr_points4 > nr_points:
                pos = pos4
                nr_points = nr_points4
                start_pos = current_position
            if nr_points5 > nr_points:
                pos = pos5
                nr_points = nr_points5
                start_pos = current_position
        return start_pos, pos

    def move2(self):
        """
        Make a move in stage 2
        :return:
        """
        if len(self.board.get_symbol_positions('O' if self.symbol == 'X' else 'X')) == 2:
            raise GameError("No more moves available for stage 2")

        current_position, next_position = self.points_system_move_stage2()

        self.board.move2(current_position, next_position, self.symbol)

        if self.check_if_mill_is_formed(next_position, self.symbol):
            self.remove_piece_opponent()

    """
    points system for stage 3:
    1. if you can get morris -> get morris 500 p
    2. if you can block opponent morris -> block 400 p
    3. if you can get 2 in a row -> get 2 in a row 300 p
    4. if you can block opponent 2 in a row -> block 200 p
    5. if you can get 1 in a row -> get 1 in a row (random) 100 p
    """

    def points_system_move_stage3(self):
        """
        Points system for stage 3
        :return:
        """
        positions_symbol = self.board.get_symbol_positions(self.symbol)
        pos1 = positions_symbol[0]
        pos2 = positions_symbol[1]
        pos3 = positions_symbol[2]
        for mill in self.board.possible_mills:
            if pos1 in mill:
                if pos2 in mill:
                    if self.board.board[pos3] == '*':
                        for positions in mill:
                            if positions != pos2 and positions != pos1:
                                return positions, pos3, 500
                if pos3 in mill:
                    if self.board.board[pos2] == '*':
                        for positions in mill:
                            if positions != pos1 and positions != pos3:
                                return positions, pos2, 500
            if pos2 in mill:
                if pos3 in mill:
                    if self.board.board[pos1] == '*':
                        for positions in mill:
                            if positions != pos2 and positions != pos3:
                                return positions, pos1, 500
        for position in self.board.get_available_spaces1():
            if self.board.board[position] == '*' and self.check_morris_opponent(position):
                return pos1, position, 400
        for mill in self.board.possible_mills:
            if pos1 in mill and pos2 not in mill and pos3 not in mill:
                counter = 0
                new_pos = -1
                for pos in mill:
                    if self.board.board[pos] == '*':
                        counter += 1
                        new_pos = pos
                if counter == 2:
                    return pos3, new_pos, 300
            if pos2 in mill and pos1 not in mill and pos3 not in mill:
                counter = 0
                new_pos = -1
                for pos in mill:
                    if self.board.board[pos] == '*':
                        counter += 1
                        new_pos = pos
                if counter == 2:
                    return pos1, new_pos, 300
            if pos3 in mill and pos1 not in mill and pos2 not in mill:
                counter = 0
                new_pos = -1
                for pos in mill:
                    if self.board.board[pos] == '*':
                        counter += 1
                        new_pos = pos
                if counter == 2:
                    return pos2, new_pos, 300
        return pos1, choice(self.board.get_available_spaces1()), 100

    def move3(self):
        """
        Make a move in stage 3
        :return:
        """
        if len(self.board.get_symbol_positions('O' if self.symbol == 'X' else 'X')) == 0:
            raise GameError("Game over! Player 1 has won!")
        positions = self.board.get_symbol_positions(self.symbol)
        if len(positions) != 3:
            raise GameOverException("No more moves available for stage 3! You win!")
        current_position, next_position, points = self.points_system_move_stage3()
        self.board.move2(current_position, next_position, self.symbol)
        if self.check_if_mill_is_formed(next_position, self.symbol):
            self.remove_piece_opponent()
