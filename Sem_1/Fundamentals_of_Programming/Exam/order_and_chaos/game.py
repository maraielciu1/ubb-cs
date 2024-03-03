from random import randint, choice

from practice.order_and_chaos.board import Board


class GameError(Exception):
    pass


class Game:
    def __init__(self):
        self.__repo = Board()

    def move(self, x, y, symbol):
        if 0 <= x < 6 and 0 <= y < 6 and self.__repo.get_symbol(x, y) == " ":
            self.__repo.place(x, y, symbol)
        else:
            raise GameError("Invalid move")

    def computer_place(self):
        if self.__repo.can_win("O") is not None:
            x, y = self.__repo.can_win("O")
            self.__repo.place(x, y, "X")
            return [x, y, "X"]
        elif self.__repo.can_win("X") is not None:
            x, y = self.__repo.can_win("X")
            self.__repo.place(x, y, "O")
            return [x, y, "O"]
        else:
            spots = self.__repo.get_available_spots()
            rand = spots[randint(0, len(spots) - 1)]
            return [rand[0], rand[1], choice(["O", "X"])]

    def win(self, symbol):
        return self.__repo.win(symbol)

    def full(self):
        return self.__repo.full()

    def load_from_file(self):
        self.__repo.load_from_file()

    def save_to_file(self):
        self.__repo.save_to_file()

    def __str__(self):
        return str(self.__repo)
