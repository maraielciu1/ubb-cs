import texttable


class Board:
    def __init__(self):
        self.__board = [[" " for i in range(6)] for j in range(6)]
        self.__placed = 0

    def __str__(self):
        header = ['/', '0', '1', '2', '3', '4', '5']
        t = texttable.Texttable()
        t.header(header)
        for i in range(6):
            t.add_row([i] + self.__board[i])
        return t.draw()


    def place(self, x, y, symbol):
        self.__board[x][y] = symbol
        self.__placed += 1

    def win(self, symbol):
        for i in range(6):
            for j in range(6):
                if self.__board[i][j] == symbol:
                    if self.check_consecutive(i, j, symbol):
                        return True
        return False

    def check_consecutive(self, row, col, symbol):
        directions = [(-1, 0), (1, 0), (0, -1), (0, 1), (-1, -1), (-1, 1), (1, -1), (1, 1)]
        for drow, dcol in directions:
            count = 1
            for step in range(1, 5):
                newr, newc = row + step * drow, col + step * dcol
                if 0 <= newr < 6 and 0 <= newc < 6 and self.__board[newr][newc] == symbol:
                    count += 1
                else:
                    break
            if count == 5:
                return True
        return False

    def can_win(self,symbol):
        for i in range(6):
            for j in range(6):
                if self.__board[i][j] == " ":
                    self.__board[i][j] = symbol
                    if self.win(symbol):
                        self.__board[i][j] = " "
                        return [i, j]
                    self.__board[i][j] = " "
        return None

    def full(self):
        return self.__placed == 36

    def get_symbol(self, x, y):
        return self.__board[x][y]

    def get_available_spots(self):
        spots = []
        for i in range(6):
            for j in range(6):
                if self.__board[i][j] == " ":
                    spots.append([i, j])
        return spots

    def load_from_file(self):
        with open("text.txt", "r") as file:
            lines = file.readlines()
            for i in range(6):
                for j in range(6):
                    if lines[i][j] == "-":
                        self.__board[i][j] = " "
                    else:
                        self.__board[i][j] = lines[i][j]

    def save_to_file(self):
        with open("text.txt", "w") as file:
            for i in range(6):
                for j in range(6):
                    if self.__board[i][j] == " ":
                        file.write("-")
                    else:
                        file.write(self.__board[i][j])
                file.write("\n")



