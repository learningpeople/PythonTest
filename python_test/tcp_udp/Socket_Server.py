#!/usr/bin/python
#-*- coding: utf-8 -*-
import socket
import threading
import time
#首先创建一个给予IPv4和TCP协议的Socket：
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

#绑定监听的地址和端口。服务器可能有多块网卡，可以绑定到某块网卡的IP地址上，也可以用0.0.0.0绑定到所有的网络地址，或是127.0.0.1绑定本机，如果绑定了这个地址客户端必须在本机运行才能连接，即外部计算机无法连接进来。
#端口号需要预先指定，因为我们写的这个服务不是标准服务，所以用9999，注：小于1024的端口号必须要有管理员权限才能绑定：
s.bind(('127.0.0.1',9999))#监听端口

#紧接着，调用listen（）方法可是监听端口，传入的参数等待连接的最大数量。
s.listen(5)
print 'Waiting for connection..'

#服务器程序通过一个永久循环来接受来自客户端的连接，accept（）会等待并返回客户端的连接：
while True:
    sock,addr = s.accept()#接受一个新连接
    #创建新线程来处理TCP连接：
    t = threading.Thread(target=tcplink,args=(sock,addr,))
    t.start()
#每次连接都必须传见新线程（或进程）来处理，否则，单线程在处理连接的过程中，无法接受其他客户端的连接：
#连接建立后，服务器首先发一条‘welcome’的信息，并加上‘hello’在发送给客户端。如果客户端发送了‘exit’字符串，就直接关闭连接。
def tcplink(sock,addr):
    print 'Accpet new connection from %s:%s...'%addr
    sock.send('Welcome!')
    while True:
        data = sock.recv(1024)
        time.sleep(1)
        if data == 'exit' or not data:
            break
        sock.send('Hello,%s'%data)
    sock.close()
    print 'Connection from %s:%s closed.'%addr
#while True:
#    print s.accept()
#    sock,addr = s.accept()
#    print sock,addr
#    t = threading.Thread(target=tcplink,args=(sock,addr,))
#    t.start()
#要测试服务器还需一个客户端程序
