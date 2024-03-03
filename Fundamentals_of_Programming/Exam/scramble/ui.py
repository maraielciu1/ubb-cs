from practice.scrable.controller import Controller


class UI:
    def __init__(self):
        self.__controller = Controller()

    def print_menu(self):
        print("1. Swap")
        print("2. Undo")
        print("3. Exit")

    def game(self):
        self.print_menu()
        print(str(self.__controller) + "\nTries left: " + str(self.__controller.tries))
        undo_count = 0
        while True:
            option = input("Enter option: ")
            if option == "1":
                w1 = int(input("Enter word 1: "))
                p1 = int(input("Enter position 1: "))
                w2 = int(input("Enter word 2: "))
                p2 = int(input("Enter position 2: "))
                try:
                    message = self.__controller.swap(w1, p1, w2, p2)
                    if message == "Game over! YOU WIN!":
                        print(message)
                        break
                    elif message == "Game over! YOU LOSE!":
                        print(message)
                        break
                    else:
                        print(message)
                    undo_count = 1
                except ValueError as e:
                    print(e)
            elif option == "2":
                if undo_count == 0:
                    print("No more undos")
                else:
                    print(self.__controller.undo())
            elif option == "3":
                print("Bye!")
                break
            else:
                print("Invalid option")

    def game1(self):
        self.print_menu()
        print(str(self.__controller) + "\nTries left: " + str(self.__controller.tries))
        undo_count = 0
        while True:
            command = input("enter command: ")
            comando = command.split()
            if comando[0] == "swap":
                w1 = int(comando[1])
                p1 = int(comando[2])
                w2 = int(comando[4])
                p2 = int(comando[5])
                try:
                    message = self.__controller.swap(w1, p1, w2, p2)
                    if "Game over! YOU WIN!" in message:
                        print(message)
                        break
                    elif "Game over! YOU LOSE!" in message:
                        print(message)
                        break
                    else:
                        print(message)
                    undo_count = 1
                except ValueError as e:
                    print(e)
            elif comando[0] == "undo":
                if undo_count == 0:
                    print("No more undos")
                else:
                    print(self.__controller.undo())
            elif comando[0] == "exit":
                print("Bye!")
                break
            else:
                print("Invalid option")

ui = UI()
ui.game1()
