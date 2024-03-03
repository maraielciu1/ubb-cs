"""
The console based game of hangman
"""
from random import randint


class Sentence:
    def __init__(self):
        self._counter = 0
        self.__data = {}
        self._load_from_file()
        self._current_sentence_coded = []
        self.__current_sentence = self.select()

    @property
    def current_sentence(self):
        return self.__current_sentence
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

    def add(self, text):
        """
        adds a sentence to the list and saves to file
        """
        self._counter += 1
        self.__data[self._counter] = text
        self._save_to_file()

    def select(self):
        """
        selects a random sentence from the list and creates a coded version of it
        """
        letters = []
        bound = self._counter
        key = randint(1, bound)
        self._current_sentence_coded = ["_" for i in range(len(self.__data[key]))]
        self._current_sentence_coded[0] = self.__data[key][0]
        letters.append(self.__data[key][0])
        self._current_sentence_coded[-1] = self.__data[key][-1]
        letters.append(self.__data[key][-1])
        for pos in range(1, len(self.__data[key]) - 1):
            if self.__data[key][pos] == " ":
                self._current_sentence_coded[pos] = " "
            if self.__data[key][pos-1] == " " or self.__data[key][pos+1] == " ":
                self._current_sentence_coded[pos] = self.__data[key][pos]
                letters.append(self.__data[key][pos])
        for letter in letters:
            for pos in range(len(self.__data[key])):
                if self.__data[key][pos] == letter:
                    self._current_sentence_coded[pos] = letter
        return self.__data[key]

    def get_all(self):
        """
        returns all the sentences in the list
        """
        return self.__data.values()

    def __str__(self):
        coded_sentence = ''.join(letter for letter in self._current_sentence_coded)
        return f"{coded_sentence}"

    def place(self, letter):
        """
        places the letter in the coded sentence if it is in the original sentence
        """
        if letter in self.__current_sentence:
            for pos in range(len(self.__current_sentence)):
                if self.__current_sentence[pos] == letter:
                    self._current_sentence_coded[pos] = letter
            return True
        return False



