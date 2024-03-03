from practice.hangman.controller import Controller
from practice.hangman.repo import Sentence


class UI:
    def __init__(self, controller: Controller):
        self.__controller = controller

    def print_menu(self):
        print("1. Add sentence")
        print("2. Start game")
        print("3. Exit")

    def game(self):
        self.print_menu()
        while True:
            option = input("Enter option: ")
            if option == "1":
                sentence = input("Enter sentence: ")
                try:
                    self.__controller.add_sentence(sentence)
                except ValueError as e:
                    print(e)
            elif option == "2":
                ok = True
                print(f"Game started and the sentence is: {self.__controller.start_game()}")
                while True:
                    letter = input("Enter letter: ")
                    if letter == "restart":
                        continue
                    try:
                        print(self.__controller.place(letter))
                    except ValueError as e:
                        print(e)
                    print(f"Game status: {self.__controller.start_game()}")
                    if self.__controller.game_over:
                        print("Game over")
                        ok = False
                        break
                if not ok:
                    break

            elif option == "3":
                print("Bye!")
                break
            else:
                print("Invalid option")


ui = UI(Controller(Sentence()))
ui.game()
