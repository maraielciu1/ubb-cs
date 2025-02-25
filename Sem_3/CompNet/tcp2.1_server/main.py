import socket

" intoarce cuvintele concatenate"


def concatenate(data):
    return "".join(data.split())


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("0.0.0.0", 7777))
s.listen(5)
cs, addr = s.accept()
b = cs.recv(40)
data = b.decode()
print(data)
data1 = concatenate(data)
cs.send(data1.encode())
cs.close()
