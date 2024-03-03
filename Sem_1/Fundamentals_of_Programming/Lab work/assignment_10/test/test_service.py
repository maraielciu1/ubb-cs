import unittest
from domain.board import Board
from service.computer_player import NotSoRandomPlayer
from service.player import Player


class TestService(unittest.TestCase):
    def setUp(self):
        self.board = Board()
        self.player = NotSoRandomPlayer(self.board)
        self.human = Player('Player1', 'O', self.board)

    """
    Tests for computer player
    """

    def test_move1(self):
        # Test the move1 method
        self.player.remaining_pieces = 9
        self.player.move1()
        self.assertEqual(self.player.remaining_pieces, 8)

    def test_check_morris(self):
        self.player.board.board[0] = 'X'
        self.player.board.board[1] = 'X'
        self.player.board.board[3] = 'O'
        self.player.move1()
        self.assertEqual(self.player.board.board[2], 'X')

    def test_check_morris_opponent(self):
        self.player.board.board[0] = 'O'
        self.player.board.board[1] = 'O'
        self.player.move1()
        self.assertEqual(self.player.board.board[2], 'X')

    def test_remove_piece(self):
        self.player.board.board[0] = 'X'
        self.player.board.board[1] = 'X'
        self.player.board.board[3] = 'O'
        self.player.board.board[4] = 'O'
        self.player.move1()
        self.assertEqual(len(self.player.board.get_symbol_positions('O')), 1)

    def test_move2(self):
        self.player.board.board[0] = 'X'
        self.player.board.board[1] = 'X'
        self.player.board.board[2] = 'O'
        self.player.move2()
        if self.player.board.board[0] == 'X':
            self.assertEqual(self.player.board.board[4], 'X')
        if self.player.board.board[1] == 'X':
            self.assertEqual(self.player.board.board[9], 'X')
        self.assertEqual(len(self.player.board.get_symbol_positions('X')), 2)
        self.assertEqual(self.player.board.board[2], 'O')

    def test_move3(self):
        self.player.board.board[0] = 'X'
        self.player.board.board[1] = 'X'
        self.player.board.board[2] = 'O'
        self.player.board.board[3] = 'O'
        self.player.board.board[4] = 'X'
        self.player.board.board[5] = 'O'
        self.player.move3()
        if self.player.board.board[0] == 'X' or self.player.board.board[4] == 'X' or self.player.board.board[1] == 'X':
            ok = False
            for i in range(24):
                if i != 0 and i != 1 and i != 2 and i != 3 and i != 4 and i != 5:
                    if self.player.board.board[i] == 'X':
                        ok = True
            self.assertTrue(ok)
        self.assertEqual(len(self.player.board.get_symbol_positions('X')), 3)
        self.assertEqual(len(self.player.board.get_symbol_positions('O')), 3)

    """
    Tests for the human player
    """

    def test_move1_human(self):
        self.human.remaining_pieces = 9
        self.human.move1(0)
        self.assertEqual(self.human.remaining_pieces, 8)

    def test_move1_human_invalid(self):
        self.human.remaining_pieces = 9
        self.human.board.board[0] = 'O'
        self.assertRaises(Exception, self.human.move1, 0)

    def test_move1_human2(self):
        self.human.remaining_pieces = 9
        self.human.move1(0)
        self.assertEqual(self.human.board.board[0], 'O')

    def test_move2_human(self):
        self.human.board.board[0] = 'O'
        self.human.board.board[1] = 'O'
        self.human.board.board[2] = 'X'
        self.human.move2(0, 9)
        self.assertEqual(self.human.board.board[9], 'O')
        self.assertEqual(self.human.board.board[0], '*')

    def test_move3_human(self):
        self.human.board.board[0] = 'O'
        self.human.board.board[1] = 'O'
        self.human.board.board[2] = 'X'
        self.human.board.board[3] = 'X'
        self.human.board.board[4] = 'O'
        self.human.board.board[5] = 'X'
        self.human.move3(0, 9)
        if self.human.board.board[0] == 'O' or self.human.board.board[4] == 'O' or self.human.board.board[1] == 'O':
            ok = False
            for i in range(24):
                if i != 0 and i != 1 and i != 2 and i != 3 and i != 4 and i != 5:
                    if self.human.board.board[i] == 'O':
                        ok = True
            self.assertTrue(ok)
        self.assertEqual(len(self.human.board.get_symbol_positions('O')), 3)
        self.assertEqual(len(self.human.board.get_symbol_positions('X')), 3)

    def test_move2_human_invalid(self):
        self.human.board.board[0] = 'O'
        self.human.board.board[1] = 'O'
        self.human.board.board[2] = 'X'
        self.assertRaises(Exception, self.human.move2, 0, 3)

    def test_move2_human_invalid2(self):
        self.human.board.board[0] = 'O'
        self.human.board.board[1] = 'O'
        self.human.board.board[2] = 'X'
        self.assertRaises(Exception, self.human.move2, 0, 1)

    """
    Tests for the board
    """

    def test_move1_board(self):
        self.board.move1(0, 'O')
        self.assertEqual(self.board.board[0], 'O')

    def test_move1_board_invalid(self):
        self.board.board[0] = 'O'
        self.assertRaises(Exception, self.board.move1, 0, 'O')

    def test_move2_board(self):
        self.board.board[0] = 'O'
        self.board.board[1] = 'O'
        self.board.board[2] = 'X'
        self.board.move2(0, 9, 'O')
        self.assertEqual(self.board.board[9], 'O')
        self.assertEqual(self.board.board[0], '*')

    def test_is_game_over(self):
        self.board.board[0] = 'O'
        self.board.board[1] = 'O'
        self.board.board[2] = 'O'
        self.board.board[9] = 'X'
        self.board.board[4] = 'X'
        self.board.board[14] = 'X'
        self.assertTrue(self.board.is_game_over('O'))


