#!/usr/bin/python
#-*- coding:utf-8 -*-
"""
Python 使用logging模块记录日志涉及四个主要类，使用官方文档中的概括最为合适：
logger---提供了应用程序可以直接使用的接口;
handler---将(logger创建的)日志记录发送到合适的目的输出；
filter---提供了细度设备来决定输出哪条日志记录；
formatter---决定日志记录的最终输出格式。
"""

"""
模块级函数
logging.getLogger([name]):返回一个logger对象,如果没有指定名字将返回root logger
logging.debug()、logging.info()、logging.warning()、logging.error()、logging.critical()：设定root logger的日志级别
logging.basicConfig():用默认Formatter为日志系统建立一个StreamHandler，设置基础配置并加到root logger中
"""

"""
Logger

每个程序在输出信息之前都要获得一个Logger。Logger通常对应了程序的模块名，比如聊天工具的图形界面模块可以这样获得它的Logger：
LOG=logging.getLogger(”chat.gui”)
而核心模块可以这样：
LOG=logging.getLogger(”chat.kernel”)

Logger.setLevel(lel):指定最低的日志级别，低于lel的级别将被忽略。debug是最低的内置级别，critical为最高
Logger.addFilter(filt)、Logger.removeFilter(filt):添加或删除指定的filter（删除）
Logger.addHandler(hdlr)、Logger.removeHandler(hdlr)：增加或删除指定的handler（处理器）
Logger.debug()、Logger.info()、Logger.warning()、Logger.error()、Logger.critical()：可以设置的日志级别
NOTSET < DEBUG < INFO < WARNING < ERROR < CRITICAL
如果把looger的级别设置为INFO， 那么小于INFO级别的日志都不输出， 大于等于INFO级别的日志都输出
"""

"""
Handlers

handler对象负责发送相关的信息到指定目的地。Python的日志系统有多种Handler可以使用。有些Handler可以把信息输出到控制台，有些Logger可以把信息输出到文件，还有些 Handler可以把信息发送到网络上。如果觉得不够用，还可以编写自己的Handler。可以通过addHandler()方法添加多个多handler
Handler.setLevel(lel):指定被处理的信息级别，低于lel级别的信息将被忽略
Handler.setFormatter()：给这个handler选择一个格式
Handler.addFilter(filt)、Handler.removeFilter(filt)：新增或删除一个filter对象
"""

"""
Formatter对象设置日志信息最后的规则、结构和内容，默认的时间格式为%Y-%m-%d %H:%M:%S，下面是Formatter常用的一些信息

"""

"""
"""
# encoding:utf-8
#import logging
 
 #FORMAT = '%(asctime)-15s %(clientip)s %(user)-8s %(message)s'
 #logging.basicConfig(format=FORMAT)
 #d = {'clientip': '192.168.0.1', 'user': 'fbloggs'}
 #logger = logging.getLogger('tcpserver')
 #logger.warning('Protocol problem: %s', 'connection reset', extra=d)
  
  #FORMAT = '%(asctime)-15s %(message)s'
  #logging.basicConfig(filename = "C:\\Users\\june\\Desktop\\1.txt", level = logging.DEBUG, filemode = "a", format=FORMAT)  
  #logging.debug('this is a message')  
  #logging.debug('test')  
   
   #import logging
   #import datetime
   #
   #curDate = datetime.date.today() - datetime.timedelta(days=0)
   #logName =  'C:\\Users\\june\\Desktop\\error_%s.log' %curDate
   #
   #logging.basicConfig(level=logging.INFO,
   #               format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
   #               #datefmt='%a, %d %b %Y %H:%M:%S',
   #               filename=logName,
   #               filemode='a')
   #
   ##2013-10-21 03:25:51,509 writeLog.py[line:14] INFO This is info message
   ##2013-10-21 03:25:51,510 writeLog.py[line:15] WARNING This is warning message
   #logging.debug('This is debug message')
   #logging.info('This is info message')
   #logging.warning('This is warning message')import logging
   import logging.config
    
    logging.config.fileConfig("logging.conf")
     
     #create logger
     loggerInfo = logging.getLogger("infoLogger")
      
      #"application" code
      loggerInfo.debug("debug message")
      loggerInfo.info("info message")
      loggerInfo.warn("warn message")
      loggerInfo.error("error message")
      loggerInfo.critical("critical message")
       
        
        loggerError = logging.getLogger("errorLogger")
        loggerError.error("Error: Hello world!")





# 定义logger模块，root是父类，必需存在的，其它的是自定义。
# logging.getLogger(NAME)便相当于向logging模块注册了一种日志打印
# name 中用 . 表示 log 的继承关系
[loggers]
keys=root,infoLogger,errorLogger
 
 # 定义handler
 [handlers]
 keys=infoHandler,errorHandler
  
  # 定义格式化输出
  [formatters]
  keys=infoFmt,errorFmt
   
   #--------------------------------------------------
   # 实现上面定义的logger模块，必需是[logger_xxxx]这样的形式
   #--------------------------------------------------
   # [logger_xxxx] logger_模块名称
   # level     级别，级别有DEBUG、INFO、WARNING、ERROR、CRITICAL
   # handlers  处理类，可以有多个，用逗号分开
   # qualname  logger名称，应用程序通过 logging.getLogger获取。对于不能获取的名称，则记录到root模块。
   # propagate 是否继承父类的log信息，0:否 1:是
   [logger_root]
   level=INFO
   handlers=errorHandler
    
    [logger_errorLogger]
    level=ERROR
    handlers=errorHandler
    propagate=0
    qualname=errorLogger
     
     [logger_infoLogger]
     level=INFO
     handlers=infoHandler
     propagate=0
     qualname=infoLogger
      
      #--------------------------------------------------
      # handler
      #--------------------------------------------------
      # [handler_xxxx]
      # class handler类名
      # level 日志级别
      # formatter，上面定义的formatter
      # args handler初始化函数参数
       
       [handler_infoHandler]
       class=StreamHandler
       level=INFO
       formatter=infoFmt
       args=(sys.stdout,)
        
        [handler_errorHandler]
        class=logging.handlers.TimedRotatingFileHandler
        level=ERROR
        formatter=errorFmt
        # When computing the next rollover time for the first time (when the handler is created),
        # the last modification time of an existing log file, or else the current time,
        # is used to compute when the next rotation will occur.
        # 这个功能太鸡肋了，是从handler被创建的时间算起，不能按自然时间 rotation 切分，除非程序一直运行，否则这个功能会有问题
        # 临时解决方案参考下面的链接：Python 多进程日志记录
        # http://blogread.cn/it/article/4175?f=wb2
        args=('C:\\Users\\june\\Desktop\\error.log', 'M', 1, 5)
         
         #--------------------------------------------------
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
          
          [formatter_infoFmt]
          format=%(asctime)s %(levelname)s %(message)s
          datefmt=
          class=logging.Formatter
           
           [formatter_errorFmt]
           format=%(asctime)s %(levelname)s %(message)s
           datefmt=
           class=logging.Formatter




#coding=utf-8
import logging
import datetime

format='%(asctime)s - %(filename)s - [line:%(lineno)d] - %(levelname)s - %(message)s'
curDate = datetime.date.today() - datetime.timedelta(days=0)
infoLogName =  r'C:/Users/june/Desktop/info_%s.log' %curDate
errorLogName =  r'C:/Users/june/Desktop/error_%s.log' %curDate
formatter = logging.Formatter(format)
infoLogger = logging.getLogger("infoLog")
errorLogger = logging.getLogger("errorLog")    
infoLogger.setLevel(logging.INFO)
errorLogger.setLevel(logging.ERROR)     
infoHandler = logging.FileHandler(infoLogName, 'a')
infoHandler.setLevel(logging.INFO)
infoHandler.setFormatter(formatter)
errorHandler = logging.FileHandler(errorLogName, 'a'
errorHandler.setLevel(logging.ERROR)
errorHandler.setFormatter(formatter)       
testHandler = logging.StreamHandler()
testHandler.setFormatter(formatter)
testHandler.setLevel(logging.ERROR)
        
infoLogger.addHandler(infoHandler)
infoLogger.addHandler(testHandler)
errorLogger.addHandler(errorHandler)
#infoLogger.debug("debug message")
#infoLogger.info("info message")
#infoLogger.warn("warn message")
# # 下面这行会同时打印在文件和终端上
#infoLogger.error("error message")
#
#errorLogger.error("error message")
#errorLogger.critical("critical message"
