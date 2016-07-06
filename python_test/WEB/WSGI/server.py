#!/usr/bin/python
#-*- coding:utf-8 -*-
#导入wsgitrf
from wsgiref.simple_server import make_server
from hello import application#自建
httpd = make_server('',8000,application)
#创建一个服务器，IP地址为空，端口是8000，处理函数是application
print 'serving Http on port 8000'
httpd.serve_forever()
#开始监听HTTP请求

