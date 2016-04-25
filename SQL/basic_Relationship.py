#!/usr/bin/python
#-*- coding:utf-8 -*-
from sqlalchemy import Table,Column,Integer,ForeignKey
from sqlalchemy.orm import relationship
from sqlalchemy.ext.declarative import declarative_base
Base = declarative_base()
#One To Many
#relationship(),指定父，
#ForeignKey(),指定子
class Parent(Base):
    __tablename__ = 'Writer'
    id = Column(Integer,primary_key=True)
    children = relationship('Child')
class Child(Base):
    __tablename__ = 'Writer2'
    id = Column(Integer,primary_key=True)
    parent = Column(Integer,ForeignKey('Writer.id'))
#one to many bidirectional（双向）
class Parent(Base):
    __tablename__ = 'parent'
    id = Column(Integer,primary_key=True)
    children = relationship('Child',back_populates='parent')
class Child(Base):
    __tablename__ = 'child'
    id = Column(Integer,primary_key=True)
    parent_id = Column(Integer,ForeignKey('parent.id'))
    parent = relationship('Parent',back_populates='child')

