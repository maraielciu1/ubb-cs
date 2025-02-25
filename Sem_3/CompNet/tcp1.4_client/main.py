import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("192.168.114.163", 7777))
word = input("Give a word: ")
s.send(word.encode())
data = s.recv(10)
counter = data.decode()
print("The number of vowels is: " + counter)
s.close()
