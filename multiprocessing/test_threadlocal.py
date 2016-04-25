#!/usr/bin/python
import threading

class threadlocal_01:
    def process_student(name):
        std = Student(name)
        do_task_1(std)
        do_task_2(std)
    def do_task_1(std):
        do_subtask_1(std)
        do_subtask_2(std)
    def do_task_2(std):
        do_subtask_2(std)
        do_subtask_2(std)
class threadlocal_02:
    global_dict={}
    def std_thread(name):
        std = Student(name)
        global_dict[threading.current_thread()] = std
        do_task_1()
        do_task_2()
    def do_task_1():
        std = global_dict[threading.current_thread()]
    def do_task_2():
        std = global_dict[threading.current_thread()]
def process_student():
    print 'Hello, %s (in %s)'%(local.student,threading.current_thread().name)
local = threading.local()
class threadlocal_03:
#    local_school = threading.local()
#    def process_student():
#        print 'Hello ,%s (in %s)'%(local_school.student,threading.current_thread().name)
    def process_thread(name):
#        global local_school
        local.student = name
        process_student()
    t1 = threading.Thread(target=process_thread,args=('Alse',))
    t2 = threading.Thread(target=process_thread,args=('Bon',))
    t3 = threading.Thread(target=process_thread,args=('Linda',),name='Thread-A')
    t1.start()
    t2.start()
    t3.start()
    t1.join()
    t2.join()
    t3.join()

