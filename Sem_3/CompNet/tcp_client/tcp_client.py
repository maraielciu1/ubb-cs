import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 7777))
s.send(b"Salut")
print(s.recv(10))
s.close()
