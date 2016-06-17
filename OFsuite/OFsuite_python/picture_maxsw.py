#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt
from readfile_templates import *
from graph_templates import *
maxsw = []
name = []
filepath = '/home/bii/Documents/ONOS_test/onos_test_data_standalone.xlsx'
name = 'Control Channel Capacity Test'
cols = readExcel(filepath,name)
#print cols
lists,tables = shoplistaExcel(cols)
#print lists,tables
X = [int(i) for i in lists[0]]
Y = lists[3]
y1 = [int(y) for y in Y[::len(Y)/len(X)]]
y2 = [int(y) for y in Y[1::len(Y)/len(X)]]
y3 = [int(y) for y in Y[2::len(Y)/len(X)]]
y = [y1,y2,y3]
y_label = lists[1][:len(Y)/len(X)]
#print X,y1,y2,y3,y_label
bar_picture(X,y,y_label,3,0.31)
plt.title('Physical Memory Usage')
plt.xlabel('Number of Switches')
plt.ylabel('Usage(MB)')
#plt.xticks(index+bar_width,x)
plt.show()
#
#          
