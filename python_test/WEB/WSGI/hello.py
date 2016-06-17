#!/usr/bin/python
#-*- coding:utf-8 -*-

#WSGI只要求web开发者实现一个函数，就可以响应HTTP请求。
def application(environ,start_response):
    start_response('200 OK',[('Content-Type','test/html')])#HTTP响应的Header另header只能发送一次，也就只能点用start_response函数一次
 #   return '<h1>Hello,Web!</h1>'
    return '<h1>hello,%s</h1>'%(environ['PATH_INFO'][1:] or 'Wed')#在地址栏中输入用户名作为URL的一部分 。
#environ：一个包含所有HTTP请求信息的dict对象
#start_response：一个发送HTTP响应的函数。start_response()函数接收两个参数（HTTP响应码，一组list表示的HTTP Header）其中header用包含两个str的tuple表示





