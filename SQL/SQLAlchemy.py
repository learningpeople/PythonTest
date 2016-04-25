#/usr/bin/pythona
#-*- coding:utf-8 -*-
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import create_engine,Table,Column,Integer,String,MetaData,PrimaryKeyConstraint,ForeignKey
from sqlalchemy.orm import sessionmaker,relationship
#创建对象的基类：a
Base = declarative_base()
class Parent(Base):
    __tablename__ = 'Writer2'
    id = Column(String(20),primary_key=True)
    name = Column(String(25))
#初始化数据库连接,详情见http://docs.sqlalchemy.org/en/rel_1_0/core/engines.html
#create_engin('<数据库类型+数据库驱动名称>://<sql-user>:<sql-password>@<机器地址>：<port>/<sql-name>')
engine = create_engine('mysql+mysqldb://testuser1:test1@localhost/testdb')
##创建DBSession类型：
DBSession = sessionmaker(bind=engine)
##创建seddion对象：
session = DBSession()
##创建新User对象：
new_user = Parent(id='5',name='Bob')
##添加到session:
session.add(new_user)
##提交即保存到数据库：
session.commit()
##关闭session:
session.close()
##创建Session：
session = DBSession()
##创建Query查询，filter是where条件，最后调用one（）返回唯一行，如果调用all()则返回所有行：
user = session.query(Parent).filter(Parent.id=='5').one()
## 打印类型和对象的name属性：
print('type',type(user))
print('name',user.name)
##关闭Session：
session.close()
#
#
