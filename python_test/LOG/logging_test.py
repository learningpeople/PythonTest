import glob
import logging
import logging.handlers
"""
该实例可实现循环打日志 ，第一个文件达到maxBytes大小后，就写入第二个文件。
"""
LOG_FILENAME="logging_rotatingfile_example.out"
my_logger = logging.getLogger('MyLogger')
my_logger.setLevel(logging.DEBUG)
handler = logging.handlers.RotatingFileHandler(LOG_FILENAME,maxBytes=20,backupCoount=5,)
my_logger.addHandler(handler)
for i in range(20):
    my_logger.debug("i = %d" %i)
    logfiles = glob.glob("%s*"%LOG_FILENAME)
    for filename in logfiles:
        print filename
