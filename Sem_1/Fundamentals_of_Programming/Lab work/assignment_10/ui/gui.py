import tkinter as tk
from tkinter import messagebox
from PIL import Image, ImageTk

from domain.board import Board, GameError, GameOverException
from service.computer_player import NotSoRandomPlayer
from service.player import Player


class NineMensMorrisGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Nine Men's Morris")

        self.board = Board()
        self.player1 = Player('Player1', 'O', self.board)
        self.ai = NotSoRandomPlayer(self.board)
        self.current_player = self.player1
        self.selected_piece = None
        self.waiting_for_piece_to_remove = False

        # Game stage
        self.stage = 1

        # Set the size of the canvas
        self.canvas_size = 600
        self.canvas = tk.Canvas(root, width=self.canvas_size, height=self.canvas_size)

        # Load and place the background image
        self.bg_image = Image.open('/Users/maraielciu/Desktop/projects-fp/a10-maraielciu1/static/img.png')
        self.bg_photo = ImageTk.PhotoImage(self.bg_image.resize((self.canvas_size, self.canvas_size)))
        self.canvas.create_image(self.canvas_size / 2, self.canvas_size / 2, image=self.bg_photo)
        self.canvas.pack()

        # Place buttons on the canvas
        self.buttons = {}
        self.create_board_buttons()

        # Start the game
        self.start_game()

    def create_board_buttons(self):
        # Define the button positions on the canvas
        positions = [
            (0, 0), (3, 0), (6, 0), (1, 1), (3, 1), (5, 1), (2, 2), (3, 2), (4, 2),
            (0, 3), (1, 3), (2, 3), (4, 3), (5, 3), (6, 3), (2, 4), (3, 4), (4, 4),
            (1, 5), (3, 5), (5, 5), (0, 6), (3, 6), (6, 6)
        ]
        grid_size = self.canvas_size // 7

        for i, (x, y) in enumerate(positions):
            btn = tk.Button(self.root, command=lambda i=i: self.on_button_click(i))
            btn.place(x=x * grid_size + grid_size // 2, y=y * grid_size + grid_size // 2, width=20, height=20)
            self.buttons[i] = btn

    def next_turn(self):
        # Check if we need to change the stage
        self.stage_check()

        # Change the current player
        self.current_player = self.ai if self.current_player == self.player1 else self.player1

        # If it's AI's turn, make the AI move
        if self.current_player == self.ai:
            self.ai_move()

    def on_button_click(self, button_id):
        # Check if we are in piece removal mode
        if self.waiting_for_piece_to_remove:
            self.remove_piece(button_id)
        else:
            try:
                if self.current_player == self.player1:
                    if self.stage == 1:
                        self.player1.move1(button_id)
                        self.update_board()
                        if self.player1.check_if_mill_is_formed(button_id, self.player1.symbol):
                            # Allow the player to remove a piece
                            # (You will need to implement this logic)
                            # self.wait_for_piece_removal()
                            pass
                        else:
                            # If no mill was formed, it's the next player's turn
                            self.next_turn()
                    elif self.stage == 2 or self.stage == 3:
                        self.select_piece(button_id)
                else:
                    # If AI's turn, do nothing as AI moves are automated
                    pass
            except GameError as e:
                messagebox.showerror("Game Error", str(e))
            except GameOverException as e:
                messagebox.showinfo("Game Over", str(e))
                self.root.quit()

    def wait_for_piece_removal(self):
        # Inform the player that they need to remove an opponent's piece
        messagebox.showinfo("Remove Piece", "Click an opponent's piece to remove it.")
        # The next click will call the `remove_piece` method

    def ai_move(self):
        # AI makes a move based on the game stage
        if self.stage == 1:
            # AI places a piece on the board
            pos = self.ai.move1()
            if self.ai.check_if_mill_is_formed(pos, self.ai.symbol):
                self.remove_piece_ai()
        elif self.stage == 2:
            # AI moves a piece to an adjacent spot
            current_pos, next_pos = self.ai.points_system_move_stage2()
            self.ai.move2()
            if self.ai.check_if_mill_is_formed(next_pos, self.ai.symbol):
                self.remove_piece_ai()
        elif self.stage == 3:
            # AI moves a piece to any empty spot
            current_pos, next_pos = self.ai.points_system_move_stage3()
            self.ai.move3()
            if self.ai.check_if_mill_is_formed(next_pos, self.ai.symbol):
                self.remove_piece_ai()

        self.update_board()
        self.next_turn()
        self.stage_check()

    def remove_piece_ai(self):
        # AI removes an opponent's piece
        self.ai.remove_piece_opponent()
        self.update_board()

    def stage_check(self):
        if self.current_player.remaining_pieces == 0 and self.stage == 1:
            self.stage = 2
        if len(self.current_player.board.get_symbol_positions(self.current_player.symbol)) == 3 and self.stage == 2:
            self.stage = 3

    def select_piece(self, button_id):
        # If a piece is already selected, move it
        if self.selected_piece is not None:
            try:
                self.current_player.move2(self.selected_piece, button_id)
                self.selected_piece = None  # Deselect piece after moving
                self.switch_player()  # Switch turns
            except GameError as e:
                messagebox.showerror("Invalid Move", str(e))
            finally:
                self.update_board()

        # If no piece is selected, select the piece if it belongs to the current player
        elif self.board.board[button_id] == self.current_player.symbol:
            self.selected_piece = button_id
            self.highlight_selected_piece(button_id)

    def highlight_selected_piece(self, button_id):
        # Highlight the selected piece
        for btn_id, btn in self.buttons.items():
            if btn_id == button_id:
                btn.config(bg='green')  # Use any color of choice to highlight
            else:
                btn.config(bg='SystemButtonFace')  # Reset other buttons to default color

    def remove_piece(self, button_id):
        # Check if the selected button is an opponent's piece and not in a mill
        if self.board.board[button_id] == 'X':  # Assuming 'X' is the opponent's piece
            # Add logic to check if the piece is not in a mill
            if not self.current_player.check_if_mill_is_formed(button_id, 'X'):
                # Remove the piece
                self.current_player.remove_piece_opponent(button_id)
                self.update_board()
                # Reset the flag and proceed with the game
                self.waiting_for_piece_to_remove = False
                self.next_turn()
            else:
                messagebox.showinfo("Invalid Move",
                                    "You cannot remove a piece from a mill unless all pieces are in mills.")
        else:
            messagebox.showinfo("Invalid Move", "You must select an opponent's piece.")

    def check_mill_and_remove(self, button_id):
        # Call this method when a player has made a move
        if self.current_player.check_if_mill_is_formed(button_id, self.current_player.symbol):
            self.waiting_for_piece_to_remove = True  # Set the flag
            messagebox.showinfo("Mill Formed", "Select an opponent's piece to remove.")
        else:
            self.next_turn()

    def switch_player(self):
        # Switch the current player to the other player
        self.current_player = self.ai if self.current_player == self.player1 else self.player1
        # If it's AI's turn, trigger the AI move
        if self.current_player == self.ai:
            self.after(500, self.ai_move)  # Use after to give time for the UI to update

    def update_board(self):
        # Update the button text to reflect the current board state
        for i, symbol in enumerate(self.board.board):
            text = 'O' if symbol == 'O' else ('X' if symbol == 'X' else '')
            self.buttons[i].config(text=text, bg='SystemButtonFace')  # Reset the color to default

    def start_game(self):
        self.update_board()
        if self.current_player == self.ai:
            self.ai_move()


if __name__ == "__main__":
    root = tk.Tk()
    game_gui = NineMensMorrisGUI(root)
    root.mainloop()
