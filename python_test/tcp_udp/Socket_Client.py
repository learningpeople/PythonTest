#!/usr/bin/python
#-*- coding: utf-8 -*-
import socket
#导入soket库并创建socket。AF_INET即指定使用IPv4，IPv6就指定AF_INET6。SOCK_STREAM指定使用面向流的TCP协议
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
#创建连接，需要知道服务器的IP地址和段口号
s.connect(('www.sina.com.cn',80))

#创建TCP连接后，向新浪服务器发送请求，要求返回首页内容，windows的换行是 \r\n,linux的换行是 \n
s.send('GET / HTTP/1.1\nHost:www.sina.com.cn\nConnection:close\n\n')

#TCP创建的是双向通道，双方都可以同时给对方发送数据，但谁先发谁后发，怎么协调则需要根据具体的协议来决定，eg：HTTP协议规定客户端过必须先发送请求给服务器，服务器受到后才发数据非客户端。
#发送的文本格式必须符合HTTP标准，若标准没问题就可接收新浪服务器返回的数据了
#接收数据
#接收数据时调用recv（max）方法一次最多接收制定>的字节数，因此在while循环中反复接收直到recv返回空数据，表示接收完毕退出循环。
buffer = []
while True:
    #每次最多接收1k的字节
    d = s.recv(1024)
    if d :
        buffer.append(d)
    else:
        break
#join，将字符串通过某种符号连接起来
data = ''.join(buffer)
#当接收完数据后，调用close()方法关闭Socket，这样一次完整的网络通信就结束了。
s.close()

#接收的数据包括HTTP头和网页本身，我们只需要把HTTP头和网页分离，把头打印出来，网页内容保存到文件。
header,html = data.split('\n\n',1)
print header
#print html
#html.decode('utf-8').encode('gb2312')
with open('/home/ubuntu-user/sina.html','wb') as f:
    f.write(html)
#此时之需要在浏览器中打开这个sina.html文件就可以看到新浪的首页了。   
html.decode('utf-8').encode('utf-8')





