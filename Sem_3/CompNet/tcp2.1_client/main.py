import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("192.168.114.163", 7777))
sentance = input("Give a sentance: ")
s.send(sentance.encode())
data = s.recv(1024)
concatenated = data.decode()
print("The concatenated words are: " + concatenated)
s.close()
