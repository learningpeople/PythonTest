#!/usr/bin/python
#-*- coding:utf-8 -*-
from readfile_templates import *
from graph_templates import *
import matplotlib.pyplot as plt

filepath = '/home/bii/Documents/ODL-test/odl_test_data.xlsx'
name = 'Role Intimation Time'
cols = readExcel(filepath,name)
print cols
X,Y = shoplistaExcel(cols)
print X,Y
x = [int(x) for x in X[0]]
y = X[-1:]
y_label = ['Time(s)']
print x,y,y_label
bar_picture(x,y,y_label,1,0.3)
plt.title(name)
plt.xlabel('Number of Switches')
plt.ylabel('Time(s)')
plt.show()
