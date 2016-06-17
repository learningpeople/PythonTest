#!/usr/bin/python
from multiprocessing import Process,Queue
from multiprocessing import Pool
import os,time,random

class multiprocessing_01:
    """"""
    print 'Process (%s) start ...'%os.getpid()
    pid = os.fork()
    if pid ==0:
        print 'I am child process (%s) and my parent is %s.'%(os.getpid(),os.getppid())
    else:
        print 'I (%s) just created a child process (%s).'%(os.getpid(),pid)
class multiprocessing_02:
    def run_proc(name):
        print 'Run child process %s (%s) ...'%(name,os.getpid())
    if __name__ == '__main__':
        print 'Parent process %s.'%os.getpid()
        p = Process(target=run_proc,args=('test',))
        print 'Process will start.'
        p.start()
        p.join()
        print 'Process end.'
#class processing_03_pool():
#    def __init__(self,name):
#        self.name = name
def long_time_task(name):
    print 'Run task %s (%s) ...'%(name,os.getpid())
    start = time.time()
    time.sleep(random.random()*3)
    end = time.time()
    print 'Task %s run %0.2f seconds.'%(name,(end-start))
class processing_03_pool:
    """"""
    if __name__=='__main__':
        print 'Parent process %s.'%os.getpid()
        p = Pool()
#pp=processing_03_pool
        for i in range(3):
            p.apply_async(long_time_task,args=(i,))
            time.sleep(0.1)
        print 'Waiting for all subprocesses done...'
        p.close()
        p.join()
        print 'All subprocess done.'
class processing_05_Queue:
    def write(q):
        for value in ['A','B','C']:
            print 'Put %s to queue...'%value
            q.put(value)
            time.sleep(random.random())
    def read(q):
        while True:
            value = q.get(True)
            print 'Get %s from queue.'%value
    if __name__ == '__main__':
        q = Queue()
        pw = Process(target=write,args=(q,))
        pr = Process(target=read,args=(q,))
        pw.start()
        pr.start()
        pw.join()
#        pr.join()
        pr.terminate()

