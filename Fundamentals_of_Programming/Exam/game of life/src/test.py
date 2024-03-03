from unittest import TestCase

from src.game import Game, GameError


class Test1(TestCase):

    # test if you get the correct pattern out of the file
    def test1(self):
        game = Game()
        game.get_pattern("block")
        pattern = game.pattern
        self.assertEqual(pattern, [["X", "X"], ["X", "X"]])

    def test2(self):
        game = Game()
        game.get_pattern("tub")
        pattern = game.pattern
        self.assertEqual(pattern, [[" ","X", " "], ["X"," ", "X"],[" ","X"," "]])

    def test3(self):
        game = Game()
        game.get_pattern("blinker")
        pattern = game.pattern
        self.assertEqual(pattern, [[" "," ", " "],["X","X", "X"],[" "," ", " "]])

    def test4(self):
        game = Game()
        game.get_pattern("beacon")
        pattern = game.pattern
        self.assertEqual(pattern, [["X", "X", " "," "], ["X", "X", " "," "], [" ", " ", "X","X"],[" ", " ", "X","X"]])

    def test5(self):
        game = Game()
        game.get_pattern("spaceship")
        pattern = game.pattern
        self.assertEqual(pattern,
                         [[' ', 'X', ' '], [' ', ' ', 'X'], ['X', 'X', 'X']])

class Test2(TestCase):
    # checks if pattern was put on the board on a good position
    def test1(self):
        game = Game()
        game.get_pattern("block")
        game.place(0,0)
        board = game.get_board()
        for i in range (2):
            for j in range(2):
                if board[i][j] != 'X':
                    assert False
        assert True

    def test2(self):
        game = Game()
        game = Game()
        game.get_pattern("block")
        game.place(0, 0)
        game.get_pattern("beacon")
        board = game.get_board()
        try:
            game.place(1,1)
            assert False
        except GameError:
            assert True

    def test3(self):
        game = Game()
        game.get_pattern("block")
        game.place(0, 0)
        game.get_pattern("beacon")
        board = game.get_board()
        try:
            game.place(6,6)
            assert False
        except GameError:
            assert True

    def test4(self):
        game = Game()
        game.get_pattern("block")
        game.place(0, 0)
        game.get_pattern("blinker")
        board = game.get_board()
        try:
            game.place(0, 2)
            assert True
        except GameError:
            assert False

    def test5(self):
        game = Game()
        game.get_pattern("block")
        game.place(0, 0)
        game.get_pattern("blinker")
        board = game.get_board()
        try:
            game.place(2, 0)
            assert True
        except GameError:
            assert False

    def test6(self):
        game = Game()
        game.get_pattern("block")
        game.place(0, 0)
        game.get_pattern("blinker")
        board = game.get_board()
        try:
            game.place(1, 0)
            assert True
        except GameError:
            assert False

    def test7(self):
        game = Game()
        game.get_pattern("spaceship")
        try:
            game.place(5, 5)
            assert True
        except GameError:
            assert False

    def test8(self):
        game = Game()
        game.get_pattern("blinker")
        game.place(0,0)
        board = game.get_board()
        for j in range(3):
            if board[0][j]!=" ":
                assert False
            if board[2][0]!=" ":
                assert False
        for i in range(3):
            if board[1][j]!="X":
                assert False
        assert True

    def test9_3(self):
        game = Game()
        game.get_pattern("blinker")
        game.place(0,0)
        board = game.get_board()
        game.tick(1)
        if board[0][1]!="X" or board[0][0] !=" " or board[0][2] !=" " or board[1][0] !=" " or board[1][1]!="X" or board[1][2]!=" " or board[2][0]!=" " or board[2][1] !="X" or board[2][2]!=" ":
            assert False
        assert True

    def test10_3(self):
        game = Game()
        game.get_pattern("block")
        game.place(0, 0)
        board = game.get_board()
        game.tick(1)
        if board[0][0]!="X" or board[0][1]!="X" or board[1][0]!="X" or board[1][1]!="X":
            assert False
        assert True






