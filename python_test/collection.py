#!usr/bin/python
# -*- coding: utf-8 -*-
from collections import namedtuple
#import sys
#reload(sys)
#sys.setdefaultencoding('utf-8')
#namedtuple用来创建一个自定义的tuple对象，并规定tuple元素的个数，并可以用属性而不是索引来引用tuple的某个i元素。
#namedtuple('name',['attibute-list'])
Point = namedtuple('Point',['x','y'])
p = Point(1,2)
print 'p.x=%d'%p.x
print 'p.y=%d'%p.y


from collections import defaultdict
#使用dict时如果引用的key不存在，就会抛出KeyError，如果希望key不存在时，返回一个默认值，就可以用defaultdict，注：默认值是调用函数返回的，而函数在创建defaultdict对象时传入。
dd = defaultdict(lambda:'N/A')
dd['key1'] = 'abc'
print "dd['key1']=%s"%dd['key1']
print "dd['key2']=%s"%dd['key2']


from collections import OrderedDict
d = dict([('a',1),('b',2),('c',3)])
print d
od = OrderedDict([('a',1),('b',2),('c',3)])
print od
od['z'] = 1
od['y'] = 2
od['x'] = 3
print od.keys()
#实现一个FIFO（先进先出）的dict，当容量超出限制时，先删除最早添加的key：
class LastUpdateOrderedDict(OrderedDict):
    def __init__(self,capacity):
        super(LastUpdataOrderedDict,self).__init__()
        self._capacity = capacity
    def __setitem__(self,key,value):
        comtaimKey = 1 if key in silf else 0
        if (len(self) - containKey) >= self._capacity:
            last = self.popitem(last=False)
            print 'remove:',last
        if catainKey:
            del self[key]
            print 'set:',(key,value)
        else:
            print 'add:',(key,value)
        OrderedDict.__setitem__(self.key,value)      


from collections import Counter
#Counter 是一个简单的计数器。
#统计字符出现的个数：
c =Counter()
for ch in 'parograming':
     c[ch] = c[ch]+1
     print c[ch]
print c
