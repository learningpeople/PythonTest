#!/usr/bin/python
#-*- coding:utf-8 -*-
#一个子程序是指A调用B，B调用C；C执行完毕返回B，B执行完毕返回A，执行。
#一个线程是指执行一个子程序。
#协程也是子程序，但子程序可中断（类似于CPU中断，而不是函数）转而执行别的子程序，在适当的返回接着执行。
from gevent import monkey;monkey.patch_socket()
import gevent

##到此3个greenlet是依次运行而不是交替运行，及线程。
#def f(n):
#    for i in range(n):
#        print gevent.getcurrent(),i
#greenlet交替运行，通过gevent.sleep()交出控制权：
def f(n):
    for i in range(n):
        print gevent.getcurrent(),i
        gevent.sleep(0)
g1 = gevent.spawn(f,5)
g2 = gevent.spawn(f,6)
g3 = gevent.spawn(f,7)

g1.join()
g2.join()
g3.join()

