#/usr/bin/pytyon
#-*- coding:utf-8 -*-

from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Table,MetaData,Column,ForeignKey,Integer,String,Unicode,DateTime
from sqlalchemy.orm import mapper,sessionmaker
#sessionmaker()函数是最长使用的创建最顶层可用于整个应用Session的方法
from datetime import datetime
#创建到数据库的连接，echo=True表示logging输出调试结果
engine = create_engine("sqlite:///tutorial.db", echo=True)
#engine = create_engine('sqlite://tutorial.db,echo=True')
#跟踪表属性
metadata = MeteDate()
user_table = Table( #创建表所需的信息：
'tf_user',metadata,
Column('id',Interger,primary_key=True),
Column('user_name',Unicode(16),unique=True,nullable=False),
Column('email_address',Unicode(255),unique=True,nullable=False),
Column('password',Unicode(40),nullable=False),
Column('first_name',Unicode(255),default=''),
Column('last_name',Unicode(255),default=''),
Column('created',DataTime,default=datatime.now))
metada.create_all(engine) #在数据库中生成表
class User(object):pass #创建一个映射类
mapper(User,user_table) #把表映射到类
Session = sessionmaker() #创建一个自定义的Session类
Session.configure(bind=engine) #创建的数据库连接关联到这个session
session = Session()
u = User()
#给映射类添加一下必要的属性，因为上面创建表指定这几个字段不能为空
u.user_name = 'dongwm'
u.email_address = 'dongwm@dongwm.com'
u.password = 'testpass'
session.add(u) #在session中添加内容

session.flush() #保存数据
session.commit() #数据库事务的提交，session自动过期而不需要关闭

#select()的支持ORM的替代方法，可以接受任意组合的class/cokumn 表达式
query = session.query(User)
print list(query)#列出user
print query.get(1)#根据主健显示
#类似于SQL的where，打印其中的第一个
print query.filter_by(user_name='dongwm').first()
u = query.filter_by(user_name='dongwm').first()
u.password = 'newpass' #修改密码
session.commit()#提交事物
print query.get(1).password #打印处新密码

for instance in session.query(User).order_by(User.id):
    print instance.user_name,instance.email_address




#Base = declarative_base()
#class User(Base):
#    __tablename__ = 'user‘
#    ids = Column(String(20),primary_key=True)
#    name = Column(String(20))
#    engine = create_engine()
