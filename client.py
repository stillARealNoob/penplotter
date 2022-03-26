from socket import socket, AF_INET, SOCK_STREAM

def recvall(sock, n):
    data = bytearray()
    while len(data)<n:
        data += sock.recv( n-len(data) )
    return data

sock = socket(AF_INET, SOCK_STREAM)

sock.bind( ("127.0.0.1", 1234) )
sock.listen()

print("Attendo la connessione")
con, addr = sock.accept()
#print("Si è connesso, muoio in pace")
n  = recvall(con, 4)
nb = int.from_bytes(n, "little")
data = recvall(con, nb)
msg = data.decode("utf-8")
print(msg)
msg = msg.upper()
data = msg.encode("utf-8")
nb = len(data)
con.sendall( nb.to_bytes(4, "little") )
con.sendall(data)
