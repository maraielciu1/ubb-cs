from random import randint, choice


class Sentence:
    def __init__(self):
        self._counter = 0
        self.__data = {}
        self._load_from_file()
        self._current_sentence_coded = []
        self._positions = []
        self.__current_sentence = self.select()
        self._history = []

    @property
    def current_sentence(self):
        return self.__current_sentence

    @property
    def current_sentence_coded(self):
        return self._current_sentence_coded

    def _save_to_file(self):
        """
        saves the sentences to a file
        """
        try:
            with (open("sentences.txt", "w") as f):
                for text in self.get_all():
                    f.write(f"{text}\n")
        except FileNotFoundError as e:
            return e

    def _load_from_file(self):
        """
        loads the sentences from a file
        """
        try:
            with open("sentences.txt", 'r') as f:
                lines = f.readlines()
        except FileNotFoundError:
            return
        for line in lines:
            line = line.strip()
            if line == "":
                continue
            self._counter += 1
            self.__data[self._counter] = line

    def get_all(self):
        return self.__data.values()

    def __str__(self):
        coded_sentence = ''.join(letter for letter in self._current_sentence_coded)
        return f"{coded_sentence}"

    def select(self):
        """
        selects a random sentence from the list and creates a coded version of it
        """
        letters = []
        bound = self._counter
        key = randint(1, bound)
        self._current_sentence_coded = [" " for i in range(len(self.__data[key]))]
        self._current_sentence_coded[0] = self.__data[key][0]
        self._positions.append(self.__data[key][0])
        self._current_sentence_coded[-1] = self.__data[key][-1]
        self._positions.append(self.__data[key][-1])
        for i in range(1, len(self.__data[key]) - 1):
            if self.__data[key][i] == " ":
                self._current_sentence_coded[i] = " "
            elif self.__data[key][i + 1] == " " or self.__data[key][i - 1] == " ":
                self._current_sentence_coded[i] = self.__data[key][i]
                self._positions.append(self.__data[key][i])
            else:
                letters.append(self.__data[key][i])
        for pos in range(1, len(self.__data[key]) - 1):
            if self.__data[key][pos] != " " and self.__data[key][pos - 1] != " " and self.__data[key][pos + 1] != " ":
                self._current_sentence_coded[pos] = choice(letters)
                letters.remove(self._current_sentence_coded[pos])
        if self.__data[key] == self.current_sentence_coded:
            self.select()
        return self.__data[key]

    @property
    def positions(self):
        return self._positions

    def swap(self, w1, p1, w2, p2):
        self._history.append(self._current_sentence_coded)
        words = str(self).split()
        letter1 = words[w1][p1]
        letter2 = words[w2][p2]
        words[w1] = words[w1][:p1] + letter2 + words[w1][p1 + 1:]
        words[w2] = words[w2][:p2] + letter1 + words[w2][p2 + 1:]
        self._current_sentence_coded = []
        for i in range(len(words)):
            self._current_sentence_coded += words[i]
            self._current_sentence_coded.append(" ")

    def undo(self):
        self._current_sentence_coded = self._history.pop()
        return str(self)

    def for_print(self):
        coded_sentence = ''.join(letter for letter in self.__current_sentence)
        return f"{coded_sentence}"

