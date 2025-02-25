import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("0.0.0.0", 7777))
s.listen(5)
cs, addr = s.accept()
b = cs.recv(10)
print(b)
cs.send(b"Hello")
cs.close()

# ipconfig getifaddr en0
