#!/usr/bin/python
import socket
s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
for data in ['Michael','Trecy','Saech']:
    s.sendto(data,('127.0.0.1',9998))
    print s.recv(1024)
s.close()
