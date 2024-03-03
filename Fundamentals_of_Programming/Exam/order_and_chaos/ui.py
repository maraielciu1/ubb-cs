from practice.order_and_chaos.game import Game, GameError


class UI:
    def __init__(self):
        self.__game = Game()

    def print_menu(self):
        print("1. Start new game")
        print("2. Load game")

    def start(self):
        self.print_menu()
        command = input("Enter your command: ")
        if command == "1":
            print(str(self.__game))
        elif command == "2":
            self.__game.load_from_file()
        else:
            print("Invalid command")
            return
        while True:
            print(str(self.__game))
            print(
                "You can save the game by typing 'save' or exit by typing 'exit'. If you want to make a move, type 'move'.")
            if self.__game.full():
                print("The board is full, Chaos won...")
                break
            command = input("Enter your command: ")
            if command == "save":
                self.__game.save_to_file()
                break
            elif command == "exit":
                break
            elif command == "move":
                try:
                    x, y, symbol = input("Enter your move: ").split()
                    self.__game.move(int(x), int(y), symbol)
                    print(str(self.__game))
                    if self.__game.win(symbol):
                        print("Congrats! Order won!!")
                        break
                    comp = self.__game.computer_place()
                    print(f"Computer moved to ({comp[0]}, {comp[1]}) with {comp[2]}.")
                    if self.__game.win(comp[2]):
                        print("Congrats! Order won!!")
                        break
                    print(str(self.__game))
                except GameError as e:
                    print(e)
                except ValueError:
                    print("Invalid input, must be 2 integers.")


ui = UI()
ui.start()
