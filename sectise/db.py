#!/usr/bin/python
#-*- coding:utf-8 -*-
import threading
#数据库引擎对象：
class _Engine(object):
    def __init_(self,connect):
        self._connect = connect
    def connect(self):
        return self._connect()
engine = None
#持有数据库连接的上下问对象：
#_DbCtx永远检测当前是否已存在Connection，如果存在，直接使用，如果不存在，则打开一个新的Connection。
class _DbCtx(threading.local):
    def __init__(self):
        self.connection = None
        self.transaction = 0

    def is_init(self):
        return not self.connection is None

    def init(self):
        self.connection = _LasyConnection()#self.connection是_LasyConnection()中的参数
        self.transactions = 0

    def cleanup(self):
        self.connection.cleanup()
        self.connection = None

    def cursor(self):
        return self.connection.cursor()

_db_ctx = _DbCtx()
#由于_db_ctx是threadlocal对象，所以，它持有的数据库连接对于每个线程看到的都是不一样的。任何一个线程都无法访问到其他线程持有的数据库连接。
#实现数据库连接的上下文，目的是自动获取和释放连接
class _ConnectionCtx(object):
    def __enter__(self):
        global _db_ctx
        self.should_cleanup = False
        if not _db_ctx.is_init():
            _db_ctx.init()
            self.should_cleanup = True
        return self

    def __exit__(self,exctype,excvalue,traceback):
        global _db_ctx
        if self.should_cleanup:
            _db_ctx.cleanup()

def connection():
    return _ConnectionCtx()
#return not ... 
#If the returncode is 0 then return True
#If the returncode is not 0 then return False
#把_ConnectionCtx的作用域作用到一个函数调用上，可以这么写：
#with connection():
#    do_some_db_operation()
#@with_connection
#def do_some_db_operation():
#    pass
#==> with connection():
#        do_some_db_operation():
#@with_connection
#def select(sql,*args):
#    pass
#@with_connection
#def update(sql,*args):
#    pass
class _TranctionCtx(object):
    def __enter__(self):
        global _db_ctx
        self.should_close_conn = False
        if not _db_c

