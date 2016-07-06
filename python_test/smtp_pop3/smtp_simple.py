#!/usr/bin/python
#-*- coding: utf-8 -*-

#Python 对SMTP支持有smtplib和email两个模块，email负责构造邮件，smtplib负责发送邮件。

from email.mime.text import MIMEText
#在构造MIMEText对象时第一个参数是邮件的正文，第二个参数是MIME的subtyple，传入‘plain’，最终MIME就是‘text/plain’，最后一定要用‘utf-8’编码保证多语言兼容性。
msg = MIMEText('hello,send by Python ...','plain','utf-8')
#输入Email地址和口令，以及SMTP服务器的IP地址和收件人的地址。
from_addr = raw_input('From: ')
password = raw_input('Password: ')
smtp_server = raw_input('SMTP server: ')
to_addr = raw_input('To: ')
import smtplib
server = smtplib.SMTP(smtp_server,25) #SMTP协议默认端口号是25
server.set_debuglevel(1)
server.login(from_addr,password)
server.sendmail(from_addr,[to_addr],msg.as_string())
server.quit()
#用set_debuglevel(1)就可以打印出和SMTP服务器交互的所有信息。SMTP就是简单的文本命令与响应。login()方法用来登陆SMTP服务器，sendmail（）方法就是发邮件，由于可以一次发给多个人，所以传入一个list，文件正文是一个str，as_string()把MIMEText对象变成str
