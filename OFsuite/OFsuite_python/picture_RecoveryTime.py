#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
from readfile_templates import *
from graph_templates import *

filepath = '/home/bii/Documents/ODL-test/odl_test_data_cluster.xlsx'
name='Master-slave Recovery Time'
cols = readExcel(filepath,name)
lists,tables = shoplistaExcel(cols)
print lists,tables
x = [int(i) for i in lists[0]]
y = lists[-1:]
y_table = ('3 controllers',)
#print x,y,y_table
bar_picture(x,y,y_table,1,0.4)
plt.title('Master-slave Recovery Time',fontsize=16)
#plt.xlim(-0.1,len(X))
#plt.ylim(0.0,2000.0,)
plt.xlabel('Number of controllers')
plt.ylabel('Time(ms)')
#plt.xticks(inx+width,X)
plt.show()
