#!/usr/bin/python
#-*- coding:utf-8 -*-
#导入SQList驱动：
import sqlite3
#连接到SQLite数据库
#数据库是test.ab
#如果文件不存在，会自动在当前目录下创建。
connect = sqlite3.connect("/home/ubuntu-user/test.ab")
#创建Cursor
cursor = connect.cursor()
#执行一条SQL语句，创建user表：
#继续执行SQL语句，插入一条记录
#执行查询语句，
#获得查询结果集。
#cursor.execute('create table user (id varchar(20) primary key,name vaarcjar(20))')
cursor.execute('insert into user (id,name) values (\'1\',\'Michael\')')
cursor.execute('select * from user where id=?','1')
values = cursor.fetchall()
print values
#通过rowcount获得插入的行数
cursor.rowcount
#关闭Cursor
cursor.close()
#提交数据
connect.commit()
#关闭connection
connect.close()
#Python的BD-API时，只要高清楚Connection和Cursor对象，打开后一定记得关闭。
#Cursor对象执行insert，update，delete语句时，执行结果由roecount返回影响的行数
#Cursor对象执行select语句时，通过featchall()可以拿来结果集。结果是一个list每个元素都是tuple，对应一行记录。
#如果sql语句带有参数，那么需要把参数按照位置传递给execute（）有几个？占位符就必须对用几个参数
#try。。。except。。。finally。。用法关闭Connection对象。

