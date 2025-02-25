import socket

" nr de vocale dintr-un sir de caractere"


def count_vowels(data):
    c = 0
    for i in data:
        if i in "aeiouAEIOU":
            c += 1
    return c


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("0.0.0.0", 7777))
s.listen(5)
cs, addr = s.accept()
b = cs.recv(10)
print(b)
c = count_vowels(b.decode())
cs.send(str(c).encode())
cs.close()
