from domain.board import Board, GameError, GameOverException
from service.player import Player
from service.computer_player import NotSoRandomPlayer


class GameUI:
    def __init__(self, player1, player2):
        self.player1 = player1
        self.player2 = player2
        self.current_player = player1
        self.stage = 1  # Initialize the game in stage 1
        self.stage31 = False
        self.stage32 = False

    def switch_player(self):
        if self.current_player == self.player1:
            self.current_player = self.player2
        else:
            self.current_player = self.player1

    def print_board(self):
        print(str(self.player2.board))
        print("\n")

    def play(self):
        while True:
            self.print_board()
            try:

                if self.stage == 1:
                    print(f"{self.current_player.name}, it's your turn (Stage 1).")
                    if self.current_player == self.player1:
                        pos = int(input("Enter the position to place your piece: "))
                        self.current_player.move1(pos)
                        if player1.check_if_mill_is_formed(pos, 'O'):
                            print("True")
                            ok = False
                            while not ok:
                                removing_pos = int(input("Choose a piece to remove"))
                                ok = player1.remove_piece_opponent(removing_pos)
                    else:
                        self.current_player.move1()
                elif self.stage == 2:
                    if self.current_player == self.player1:
                        if not self.stage31:
                            print(f"{self.current_player.name}, it's your turn (Stage 2).")
                            if self.current_player == self.player1:
                                current_pos = int(input("Enter the current position to move from: "))
                                next_pos = int(input("Enter the next position to move to: "))
                                self.current_player.move2(current_pos, next_pos)
                                if player1.check_if_mill_is_formed(next_pos, 'O'):
                                    print("True")
                                    ok = False
                                    while not ok:
                                        removing_pos = int(input("Choose a piece to remove"))
                                        ok = player1.remove_piece_opponent(removing_pos)
                        else:
                            print(f"{self.current_player.name}, it's your turn (Stage 3).")
                            if self.current_player == self.player1:
                                current_pos = int(input("Enter the current position to move from: "))
                                next_pos = int(input("Enter the next position to move to: "))
                                self.current_player.move3(current_pos, next_pos)
                                if player1.check_if_mill_is_formed(next_pos, 'O'):
                                    print("True")
                                    ok = False
                                    while not ok:
                                        removing_pos = int(input("Choose a piece to remove"))
                                        ok = player1.remove_piece_opponent(removing_pos)
                    else:
                        if not self.stage32:
                            self.current_player.move2()
                        else:
                            self.current_player.move3()
                if self.current_player.board.is_game_over(self.current_player.symbol):
                    print("Game is over. It's a draw.")
                    break
                if self.current_player.remaining_pieces == 0:
                    self.stage = 2  # Move to stage 2 when all pieces are placed
                if len(self.current_player.board.get_symbol_positions(
                        self.current_player.symbol)) == 3 and self.stage == 2:
                    if self.current_player == self.player1:
                        self.stage31 = True
                    else:
                        self.stage32 = True
                elif len(self.current_player.board.get_symbol_positions(
                        self.current_player.symbol)) == 2 and self.stage == 2:
                    if self.current_player == self.player1:
                        print("You lost")
                        break
                    else:
                        print("You won")
                        break
                if self.current_player.board.is_game_over(self.current_player.symbol):
                    print("Game is over. It's a draw.")
                    break  # End the game
                self.switch_player()
            except GameError as e:
                print(f"Error: {str(e)}. Please try again.")
            except GameOverException as e:
                print(str(e))
                break
            except ValueError:
                print("Invalid input. Please try again.")


if __name__ == "__main__":
    b = Board()
    player1 = Player('Player1', 'O', b)
    ai = NotSoRandomPlayer(b)
    ui = GameUI(player1, ai)
    ui.play()
