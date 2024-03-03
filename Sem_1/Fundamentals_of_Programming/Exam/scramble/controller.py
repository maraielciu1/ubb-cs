from practice.scrable.repo import Sentence


class Controller:
    def __init__(self):
        self.__repo = Sentence()
        self.__tries = len(self.__repo.current_sentence) - self.__repo.current_sentence.count(" ")

    def swap(self, w1, p1, w2, p2):
        words = str(self.__repo).split()
        if w1 < 0 or w1 >= len(words) or w2 < 0 or w2 >= len(words):
            raise ValueError("Invalid word index")
        if p1 < 0 or p1 >= len(words[w1]) or p2 < 0 or p2 >= len(words[w2]):
            raise ValueError("Invalid position index")
        if p1 in self.__repo.positions or p2 in self.__repo.positions:
            raise ValueError("Invalid position")
        self.__repo.swap(w1, p1, w2, p2)
        if self.game_overh():
            return "Game over! YOU WIN! the sentence was: " + self.__repo.for_print()
        else:
            self.__tries -= 1
            if self.game_overc():
                return "Game over! YOU LOSE! the sentence was: " + self.__repo.for_print()
        return str(self.__repo) + "\nTries left: " + str(self.__tries)

    @property
    def tries(self):
        return self.__tries

    def game_overh(self):
        for i in range(len(self.__repo.current_sentence_coded)):
            if self.__repo.current_sentence_coded[i] != " " and self.__repo.current_sentence_coded[i] != \
                    self.__repo.current_sentence[i]:
                return False
        return True

    def undo(self):
        return self.__repo.undo()

    def game_overc(self):
        if self.__tries == 0:
            return True
        return False

    def __str__(self):
        return str(self.__repo)
