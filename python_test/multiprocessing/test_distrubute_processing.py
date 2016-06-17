import random,time,Queue
from multiprocessing.managers import BaseManager

task_queue = Queue.Queue()
result_queue = Queue.Queue()
class QueueManager(BaseManager):
    pass
#class QueueManager1:
#    QueueManager.register('get_task_queue',callable=lambda:task_queue)
#    QueueManager.register('get_result_queue',callable=lambda:result_queue)
#    BaseManager.register('get_task_queue',callable=lambda:task_queue)
#    BaseManager.register('get_result_queue',callable=lambda:result_queue)
#    manager = QueueManager(address=('',5000),authkey='abc')
#    manager = BaseManager(address=('',5000),authkey='abc')
#    manager.start()
#    task = manager.get_task_queue()
#    result = manager.get_result_queue()
#    for i in range(10):
#        n = random.randint(0,10000)
#        print ('Put task %d ...'%n)
#        task.put(n)
#    print ('Try get results ...')
#    for i in range(10):
#        r = result.get(timeout=10)
#        print('Result:%s'%r)
#    manager.shutdown()
class QueueManager2:
    QueueManager.register('get_task_queue')
    QueueManager.register('get_result_queue')
#    BaseManager.register('get_task_queue')
#    BaseManager.register('get_result_queue')
    server_addr = '127.0.0.1'
    print ('Connect to server %s ...'%server_addr)
    worker = QueueManager(address=(server_addr,5000),authkey='abc')
#    worker = BaseManager(address=(server_addr,5000),authkey='abc')
    worker.connect()
    task = worker.get_task_queue
    result = worker.get_result_queue
    for i in range(10):
        try:
            n = task.get(timeout=1)
            print('run task %d * %d'%(n,n,n*n))
            r = '%d * %d = %d'%(n,n,n*n) 
            time.sleep(1)
            result.put(r)
        except Queue.Empty:
            print ('task queue is empty.')
    print ('worker exit') 
