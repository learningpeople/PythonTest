#!/usr/bin/python
#-*- conding:utf-8 -*-
"""
logging.basicConfig([**kwargs]):
filename:日志文件的保存路径。如果配置了些参数，将自动创建一个FileHandler作为Handler。
filemode:日志文件的打开模式。默认值为‘a’，表示日志消息以追加的形式添加到日志文件中。如果设为'w',那么每次程序启动的时候都会创建一个新的日志文件。
format:设置日志输出格式。
datefmt:定义日期格式。
level:设置日志的级别，对地狱该级别的日志消息将被忽略。
stream: 设置特定的流用于初始化StreamHandler

# 日志格式
#--------------------------------------------------
# %(asctime)s       年-月-日 时-分-秒,毫秒 2013-04-26 20:10:43,745
# %(filename)s      文件名，不含目录
# %(pathname)s      目录名，完整路径
# %(funcName)s      函数名
# %(levelname)s     级别名
# %(lineno)d        行号
# %(module)s        模块名
# %(message)s       消息体
# %(name)s          日志模块名
# %(process)d       进程id
# %(processName)s   进程名
# %(thread)d        线程id
# %(threadName)s    线程名

"""
import logging
#os.path.join(path,filename/path) 连接目录与文件名或目录
#os.getcwd() 当前脚本工作目录的路径。
logging.basicConfig(filename = os.path.join(os.getcwd(),'log.txt'),level = logging.WARN,filemode = 'w',format = '%(levelname)s - %(levelname)s : %(message)s')
#logging.basicConfig(format=log_format,datefmt='%Y-%m-%d %H:%M:%S %p',level=logging.DEBUG)
logging.debug('debug')
logging.info('info')
logging.warning('warn')
logging.error('error')


"""
logging.getLogger([name])
创建Logger对象.日志记录的工作主要由Logger对象来完成。在调用getLogger时要提供Logger的名称（注：多次使用相同名称来调用getLogger，返回的是同一个对象的引用。），Logger实例之间有层次关系，这些关系通过Logger名称来体现，如：
"""
p = logging.getLogger("root")
c1 = logging.getLogger("root.c1")
c2 = logging.getLogger("root.c2")
#p是父logger, c1,c2分别是p的子logger。c1, c2将继承p的设置。如果省略了name参数, getLogger将返回日志对象层次关系中的根Logger。


"""
logging.setLoggerClass(klass)
logging.getLoggerClass()
获取/设置日志类型。用户可以自定义日志类来代替系统提供的logging.Logger类。

logging.getLevelName(lvl)
获取日志级别对应的名称。例如
print logging.getLevelName(logging.NOTSET)
print logging.getLevelName(10) #logging.DEBUG
print logging.getLevelName(logging.DEBUG)
print logging.getLevelName(30) #logging.WARN
print logging.getLevelName(logging.ERROR)
print logging.getLevelName(50)  #logging.CRITICAL
    #这里来设置日志的级别
    #CRITICAl    50
    #ERROR    40
    #WARNING    30
    #INFO    20
    #DEBUG    10
    #NOSET    0

logging.shutdown()

　　当不再使用日志系统的时候，调用该方法，它会将日志flush到对应的目标域上。一般在系统退出的时候调用。

Logger对象 通过调用logging.getLogger(name)来创建，它有如下常用的方法和属性：

Logger.setLevel(lvl):

设置日志的级别。对于低于该级别的日志消息将被忽略。下面一个例子演示setLevel方法：
#coding=gbk
import logging
logging.basicConfig(filename = os.path.join(os.getcwd(), 'log.txt'), level = logging.DEBUG)
log = logging.getLogger('root.test')
log.setLevel(logging.WARN)  #日志记录级别为WARNNING
log.info('info')    #不会被记录
log.debug('debug')  #不会被记录
log.warning('warnning')
log.error('error')

Logger.debug(msg [ ,*args [, **kwargs]])
记录DEBUG级别的日志信息。参数msg是信息的格式，args与kwargs分别是格式参数。
import logging
logging.basicConfig(filename = os.path.join(os.getcwd(), 'log.txt'), level = logging.DEBUG)
log = logging.getLogger('root')
log.debug('%s, %s, %s', *('error', 'debug', 'info'))
log.debug('%(module)s, %(info)s', {'module': 'log', 'info': 'error'})

Logger.info(msg[ , *args[ , **kwargs] ] )

Logger.warnning(msg[ , *args[ , **kwargs] ] )

Logger.error(msg[ , *args[ , **kwargs] ] )

Logger.critical(msg[ , *args[ , **kwargs] ] )

记录相应级别的日志信息。参数的含义与Logger.debug一样。

Logger.log(lvl, msg[ , *args[ , **kwargs] ] )

记录日志，参数lvl用户设置日志信息的级别。参数msg, *args, **kwargs的含义与Logger.debug一样。

Logger.exception(msg[, *args])

以ERROR级别记录日志消息，异常跟踪信息将被自动添加到日志消息里。Logger.exception通过用在异常处理块中，如：

import logging
logging.basicConfig(filename = os.path.join(os.getcwd(), 'log.txt'), level = logging.DEBUG)
log = logging.getLogger('root')
try:
    raise Exception, 'this is a exception'
except:
    log.exception('exception') #异常信息被自动添加到日志消息中

Logger.addFilter(filt)

Logger.removeFilter(filt)

添加/移除日志消息过滤器。在讲述Filter时具体介绍。

Logger.addHandler(hdlr)

Logger.removeHandler(hdlr)

添加/移除日志消息处理器。在讲述Handler时具体介绍。

Logger.makeRecord(name, lvl, fn, lno, msg, args, exc_info[, func, extra])

创建LogRecord对象。日志消息被实例为一个LogRecord对象，并在日志类内处理。

"""


"""

"""



