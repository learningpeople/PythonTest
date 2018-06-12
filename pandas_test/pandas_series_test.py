# /usr/bin/env python
# -*-coding:utf-8-*-

import numpy as np
import pandas as pd
from pandas import Series

"""
Series：一维数组，与Numpy中的一维array类似。二者与Python基本的数据结构List也很相近，其区别是：List中的元素可以是不同的数据类型，而Array和Series中则只允许存储相同的数据类型，这样可以更有效的使用内存，提高运算效率。
Time- Series：以时间为索引的Series。

DataFrame：二维的表格型数据结构。很多功能与R中的data.frame类似。可以将DataFrame理解为Series的容器。以下的内容主要以DataFrame为主。

Panel ：三维的数组，可以理解为DataFrame的容器。
"""
"""
1. 没有给数据指定索引，一个包含整数0到 N-1 （这里N是数据的长度）的默认索引被创建。 你可以分别的通过它的 values 和 index 属性来获取Series的数组表示和索引对象,
2. Series是一个定长的，有序的字典，因为它把索引和值映射起来了。它可以适用于许多期望一个字典的函数,
3. 没有发现对应于的值，就出现了 NaN （不是一个数），这在pandas中被用来标记数据缺失或 NA 值。在pandas中用函数 isnull 和 notnull 来检测数据丢失
4. Series对象本身和它的索引都有一个 name 属性，它和pandas的其它一些关键功能整合在一起
5. Series的索引可以通过赋值就地更改
[(.values),(.index),(.isnull),(.notnull),(.name),(.index.name)]
"""

obj = Series([4, 7, -5, 3])
print 'obj:', obj
print 'obj.values:', obj.values
print 'obj.index', obj.index
print '---------------------->'
print
obj2 = Series([4, 5, -5, 3], index=['b', 'd', 'a', 'c'])
print 'obj2:', obj2
print 'obj2.index:', obj2.index
print "obj2['a']", obj2['a']
obj2['d'] = 6
print "obj2['d'] = 6,obj2[['c', 'a', 'd']]: ", obj2[['c', 'a', 'd']]
print '---------------------->'
print
print 'obj2:', obj2
print 'obj2[obj2 >0]', obj2[obj2 >0]
print 'obj2 * 2', obj2 * 2
print 'np.exp(obj2):', np.exp(obj2) #exp() 方法返回x的指数,ex
print "'b' in obj2:", 'b' in obj2
print "'e' in obj2:", 'e' in obj2
print '---------------------->'
print
sdata = {'Ohio': 35000, 'Texas': 71000, 'Oregon': 16000, 'Utah': 5000}
obj3 = Series(sdata)
print 'obj3:', obj3
states = ['California', 'Ohio', 'Oregon', 'Texas']
obj4 = Series(sdata, index=states)
print 'obj4:', obj4
# 因为没有发现对应于 ‘California’ 的值，就出现了 NaN （不是一个数），这在pandas中被用来标记数据缺失或 NA 值
# 在pandas中用函数 isnull 和 notnull 来检测数据丢失：
print "pd.isnull(obj4):", pd.isnull(obj4)
print "pd.notnull(obj4):", pd.notnull(obj4)
print "obj4.isnull():", obj4.isnull()
print "obj3+obj4:", obj3+obj4
obj4.name = "population"
obj4.index.name = "state"
print "obj4:", obj4

obj.index = ['Bob', 'Steve', 'Jeff', 'Ryan']
print 'obj:', obj


