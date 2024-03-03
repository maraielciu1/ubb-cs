from board import Board
from game import Game, ComputerPlayerFirstSquare, ComputerPlayerSmarter

class UI:
    def start(self):
        board = Board()
        """
        Change the ai for different lvls of difficulty
        Instance of strategy design pattern
        """
        ai = ComputerPlayerFirstSquare(board)
        # ai = ComputerPlaysSmarter(board)
        game = Game(board,ai)
        turn = 1 # X plays
        while True:
            print(game.get_board())
            if turn == 1:
                try:
                    row = int(input("X="))
                    column = int(input("Y="))
                    game.move_human(row,column)
                except ValueError as ve:
                    print(str(ve))
                    continue
                    # turn = 1-turn
            else:
                x,y = game.move_computer()
                print(f"Computer moves at {x}, {y}")
            if game.get_board().is_won():
                if turn == 1:
                    print("Congrats!")
                else:
                    print("Commiserations!")
                break
            if game.get_board().is_full():
                print("draw")
                break
            turn = 1-turn

ui = UI()
ui.start()