#!/usr/bin/python
import time
import threading
import multiprocessing
class threading_01:
    """"""
    def loop():
        print 'threading %s is running...'%threading.current_thread().name
        n = 0
        while n < 3:
            n = n + 1
            print 'threading %s >>> %s'%(threading.current_thread().name,n)
            time.sleep(1)
        print 'thread %s is ended.'%threading.current_thread().name
    print 'thread %s is running ...'%threading.current_thread().name
    t = threading.Thread(target=loop,name='LoopThread')
    t.start()
    t.join()
    print 'thread %s end'%threading.current_thread().name
#class threading_02_lock:
#    """"""
balance = 0
def change_it(n):
#        balance = 0
    global balance
    balance = balance + n
    balance = balance - n
 #   @staticmethod
class threading_02_lock:
    """"""
    def run_thread(n):
        for i in range(10000):
#            change_it(n)
            change_it(n)
    t1 = threading.Thread(target=run_thread,args=(5,))
    t2 = threading.Thread(target=run_thread,args=(8,))
    t1.start()
    t2.start()
    t1.join()
    t2.join()
    print balance
class threading_03_lock:
    balance = 0
#    lock = threading.Lock()
    def run_thread(n):
        lock = threading.Lock()        
        for i in range (10000):
            lock.acquire()
            try:
                change_it(n)
            finally:
                lock.release()
    t1 = threading.Thread(target=run_thread,args=(6,))
    t2 = threading.Thread(target=run_thread,args=(9,))
    t1.start()
    t2.start()
    t1.join()
    t2.join()

class threading_04_multiprocess:
    def loop():
        x=0
        while True:
            x = x ^ 1
    for i in range(multiprocessing.cpu_count()):
        t = threading.Thread(target=loop)
#        t.start()      
