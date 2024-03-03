from practice.hangman.repo import Sentence


class Controller:
    def __init__(self, repo: Sentence):
        self.__repo = repo
        self.__computer_sentence = "hangman"
        self._counter = 0
        self.__progress_sentence = ""
        self.__game_over = False

    def add_sentence(self, sentence):
        words = sentence.split()
        if len(words) < 1:
            raise ValueError("Empty sentence")
        for word in words:
            if len(word) < 3 and word != "" and word != " ":
                raise ValueError("Word too short")
        if sentence not in self.__repo.get_all():
            self.__repo.add(sentence)
        else:
            raise ValueError("Sentence already exists")

    def select_sentence(self):
        return self.__repo.select()

    def get_all(self):
        return self.__repo.get_all()

    def start_game(self):
        coded_sentence = str(self.__repo)
        if coded_sentence == self.__repo.current_sentence:
            self.__game_over = True
            return f"YOU WIN!"
        else:
            return f"output: {coded_sentence} - {self.__progress_sentence}"

    def place(self, letter):
        if self.__repo.place(letter) is False:

            self.__progress_sentence = ""
            if self._counter == 7:
                self.__game_over = True
                return f"Game over! The sentence was {self.__repo.current_sentence}"
            for i in range(len(self.__computer_sentence)):
                if i <= self._counter:
                    self.__progress_sentence += self.__computer_sentence[i]
            self._counter += 1
            return f"Letter {letter} not in sentence"
        else:
            return f"Letter {letter} in sentence"

    @property
    def game_over(self):
        return self.__game_over

