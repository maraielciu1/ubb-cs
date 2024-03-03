from src.game import Game, GameError


class UI:
    def __init__(self):
        self.__game = Game()

    def print_menu(self):
        print("You can load a started game with the command 'load filename'")
        print(
            "You can place a pattern with the command 'place <pattern> <x,y>', places the pattern with upper left corner at position x,y")
        print("The available patterns are: block, tub, blinker, beacon and spaceship")
        print("You can exit with the command 'exit'")
        print("You can create a new generation with the command 'tick [n]'")
        print("You can save the current game with the command 'save filename'")

    def start(self):
        print(self.__game)
        self.print_menu()
        while True:
            command = input("Enter command: ")
            command = command.split()
            if len(command) == 0:
                print("Invalid command")
            elif command[0] == "exit":
                print("Bye!")
                break
            elif command[0] == "load" and len(command) == 1:
                print("invalid load command")
            elif command[0] == "load" and len(command) > 1:
                if command[1] == "filename" and len(command) == 2:
                    try:
                        self.__game.load_board()
                    except FileNotFoundError as e:
                        print(e)
                else:
                    print("Invalid load command")
            elif command[0] == "save" and len(command) == 1:
                print("invalid save command")
            elif command[0] == "save" and len(command) > 1:
                if command[1] == "filename" and len(command) == 2:
                    try:
                        self.__game.save_board()
                    except FileNotFoundError as e:
                        print(e)
                else:
                    print("Invalid save command")
            elif command[0] == "tick":
                if len(command) == 1:
                    self.__game.tick(1)
                elif len(command) == 2:
                    try:
                        number = int(command[1])
                        self.__game.tick(number)
                    except ValueError as e:
                        print(e)
                else:
                    print("Invalid tick command")

            elif command[0] == "place":
                if len(command) == 3:
                    if command[1] not in ["block", "tub", "blinker", "beacon", "spaceship"] or command[1] == "":
                        print("Invalid pattern")
                    else:
                        if command[2] != "":
                            coords = command[2].split(",")
                            if len(coords) == 2:
                                try:
                                    x = int(coords[0])
                                    y = int(coords[1])
                                    self.__game.get_pattern(command[1])
                                    self.__game.place(x,y)
                                except ValueError as e:
                                    print(e)
                                except GameError as e:
                                    print(e)
                            else:
                                print("Invalid coordinates")
                else:
                    print("Invalid place command")
            else:
                print("invalid command")
            print(self.__game)

