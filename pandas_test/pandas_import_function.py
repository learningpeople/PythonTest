# /usr/bin/evn python
# -*-coding:utf-8-*-
"""
    __author__:sa
"""
from pandas import Series, DataFrame
import pandas as pd
import numpy as np

"""
reindex(index, [columns,] method, fill_value, limit, level, copy)
method  插值（填充）方法 # "ffill" 前向（或进位）填充 "bfill"后向（或进位）填充
fill_value  代替重新索引时引入的缺失数据值
limit   当前向或后向填充时，最大的填充间隙
level   在多层索引上匹配简单索引，否则选择一个子集
copy    如果新索引与就的相等则底层数据不会拷贝。默认为True(即始终拷贝）
"""

obj = Series([4.5, 7.2, -5.3, 'yellow'], index=['d', 'b', 'a', 'c'])
print 'obj:\n', obj
obj2 = obj.reindex(['a', 'b', 'c', 'd', 'e'], fill_value=0)
print 'obj2:\n', obj2
print Series(obj, index=['a', 'b', 'c', 'd', 'e'])
obj3 = obj2.reindex(range(6), method='bfill')
# "ffill" 前向（或进位）填充
# "bfill"后向（或进位）填充
# method选项,要求obj2.index 必须从小到大排序,填充是必须是有值向无值的方向填充，否则值都会被nan填充
print 'obj3:\n', obj3

frame = DataFrame(np.arange(9).reshape((3, 3)), index=['a', 'c', 'd'], columns=['Ohio', 'Texas', 'California'])
# reshape 重塑列表格式
print 'frame:\n', frame
states = ['Texas', 'Utah', 'California']
frame2 = frame.reindex(index=['a', 'b', 'c', 'd'], columns=states, method='ffill')
print 'frame2:\n', frame2
print "frame.ix[['a', 'b', 'c', 'd'], states]:\n", frame.ix[['a', 'b', 'c', 'd'], states]

obj = Series(np.arange(5.), index=['a', 'b', 'c', 'd', 'e'])
new_obj = obj.drop('c')
print 'new_obj:\n', new_obj
print "obj.drop(['d', 'c']):\n", obj.drop(['d', 'c'])
print "obj['b']:\n", obj['b']
print "obj[1]:\n",  obj[1]
print "obj[2:4]:\n",  obj[2:4]
# 使用标签来切片和正常的Python切片并不一样，它会把结束点也包括在内
print "obj['b':'c']:\n",  obj['b':'c']
obj['b':'c'] = 5
print "obj['b':'c'] = 5:\n", obj
print "obj[['b', 'a', 'd']]:\n",  obj[['b', 'a', 'd']]

"""
obj[val]	从DataFrame选择单一列或连续列。特殊情况下的便利：布尔数组（过滤行），切片（行切片），或布尔DataFrame（根据一些标准来设置值）。
obj.ix[val]	从DataFrame的行集选择单行
obj.ix[:, val]	从列集选择单列
obj.ix[val1, val2]	选择行和列
reindex 方法	转换一个或多个轴到新的索引
xs 方法	通过标签选择单行或单列到一个Series
icol, irow 方法	通过整数位置，分别的选择单行或单列到一个Series
get_value, set_value 方法	通过行和列标选择一个单值
"""
data = DataFrame(np.arange(16).reshape((4, 4)),
                 index=['Ohio', 'Colorado', 'Utah', 'New York'],
                 columns=['one', 'two', 'three', 'four'])

print "data[data['three'] > 5]:\n", data[data['three'] > 5]

print "data[:2]:\n",  data[:2]
print "data[['three', 'one']]:\n",  data[['three', 'one']]
print "data.ix[0]:\n",  data.ix[0]  #
print "data.ix['Colorado',['two','three']]:\n",  data.ix['Colorado', ['two', 'three']]
print "data.ix[data['three'] > 5,:2]:\n", data.ix[data['three'] > 5, :2]
data[data['three'] > 5] = 0  #
print "data:\n", data

print "data.drop(['Colorado', 'Ohio']):\n", data.drop(['Colorado', 'Ohio'])
print "data.drop('two', axis=1):\n", data.drop('two', axis=1)
print "data.drop(['two', 'four'], axis=1):\n", data.drop(['two', 'four'], axis=1)

"""
灵活的算术方法
add	加法(+)
sub	减法(-)
div	除法(/)
mul	乘法(*)
"""
data1 = DataFrame(np.arange(16).reshape((4, 4)),
                  index=['Ohio', 'Colorado', 'Utah', 'New York'],
                  columns=['one', 'two', 'three', 'four'])
data2 = DataFrame(np.arange(9).reshape((3, 3)),
                  index=['Ohio', 'Colorado', 'Utah'],
                  columns=['one', 'two', 'three']
                  )
# 在索引不重合的地方引入了NA值。数据缺失在算术运算中会传播。
print data2
print data1
print data2+data1
print data2.reindex(columns=data.columns, fill_value=0)
print data.reindex(columns=data2.columns)
series = data1.ix[0]
print series
print data1-series
# series1=data1['Ohio']
# print series1
# print data1- series1
print data1.Dtype