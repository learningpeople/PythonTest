#!/usr/bin/python
#-*- coding: utf-8 -*-
import socket
#创建socket，UDP连接（SCOVK_DGRAM）
s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
#绑定端口，不需要调用listen()方法，直接接收来自人和客户端的数据
s.bind(('127.0.0.1',9998))
print 'Bind UDP on 9998 ...'
while True:
    print s.recvfrom(1024),s.recvfrom()
    print s.recv()
    data,addr = s.recvfrom(1024)
    print 'Received from %s:%s..'%addr
    s.sendto('Hello,%s'%date,addr)
#recvfrom()方法返回数据和客户端的地址与端口，这样服务器收到数据后直接调用sendto（）方法就可以把数据用UDP发给客户端


