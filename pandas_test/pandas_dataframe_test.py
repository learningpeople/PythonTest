# /usr/bin/evn python
# -*-coding:utf-8-*-
"""
    __author__:sa
"""
from pandas import DataFrame, Series
import pandas as pd
import numpy as np

"""
Series：一维数组，与Numpy中的一维array类似。二者与Python基本的数据结构List也很相近，其区别是：List中的元素可以是不同的数据类型，而Array和Series中则只允许存储相同的数据类型，这样可以更有效的使用内存，提高运算效率。
Time- Series：以时间为索引的Series。

DataFrame：二维的表格型数据结构。很多功能与R中的data.frame类似。可以将DataFrame理解为Series的容器。以下的内容主要以DataFrame为主。

Panel ：三维的数组，可以理解为DataFrame的容器。
"""
"""
索引DataFrame时返回的列是底层数据的一个视窗，而不是一个拷贝。因此，任何在Series上的就地修改都会影响DataFrame。列可以使用Series的 copy 函数来显式的拷贝。

"""
data = {'state': ['Ohio', 'Ohio', 'Ohio', 'Nevada', 'Nevada'],
        'year': [2000, 2001, 2002, 2001, 2002],
        'pop': [1.5, 1.7, 3.6, 2.4, 2.9]}
frame = DataFrame(data)
print 'frame:\n', frame
print "DataFrame(data, columns=['year','state','pop'])\n", DataFrame(data, columns=['year','state','pop'])
# 如果你传递了一个行，但不包括在 data 中，在结果中它会表示为NA值：
frame2 = DataFrame(data, columns=['year', 'state', 'pop', 'debt'],
                   index=['one', 'two', 'three', 'four', 'five'])
print 'frame2:\n', frame2
print 'frame2.columns:\n', frame2
print "frame2['state']:\n", frame2['state']
print "frame2.year:\n", frame2.year
print "frame2.ix['three']:\n", frame2.ix['three']

frame2['debt'] = 16.5
print 'frame2:\n', frame2
frame2['debt'] = np.arange(5.)
# range()返回的是range object，而np.nrange()返回的是numpy.ndarray()
# range()不支持步长为小数，np.arange()支持步长为小数
print 'frame2:\n', frame2
val = Series([-1.2, -1.5, -1.7], index=['two', 'four', 'five'])
frame2['debt'] = val
print 'frame2:\n', frame2
frame2['eastern'] = frame2.state == 'Ohio'
print 'frame2:\n', frame2
del frame2['eastern']
print 'frame2.columns:\n', frame2.columns
print '------------------------->'
print
pop = {'Nevada': {2001: 2.4, 2002: 2.9},
       'Ohio': {2000: 1.5, 2001: 1.7, 2002: 3.6}}
frame3 = DataFrame(pop)
print 'frame3:\n', frame3
# 转置
print 'frame3.T:\n', frame3.T
print 'DataFrame(pop, index=[2001, 2002, 2003]):\n', DataFrame(pop, index=[2001, 2002, 2003])

pdata = {'Ohio': frame3['Ohio'][:-1],
         'Nevada': frame3['Nevada'][:2]}
print "DataFrame(pdata):\n", DataFrame(pdata)
frame3.index.name = 'year'
frame3.columns.name = 'state'
frame3.name = 'frame3'
print 'frame3:\n', frame3
print 'frame3.values:\n', frame3.values
print 'frame2.values:\n', frame2.values
print "'Ohio' in frame3.columns:\n", 'Ohio' in frame3.columns
print "2003 in frame3.index:\n", 2003 in frame3.index
print '------------------------->'
print
