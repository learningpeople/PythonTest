#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
from readfile_templates import *
from graph_templates import *
r = []
l = []
ll = []
filepath = '/home/bii/Documents/ONOS_test/onos_test_data_standalone.xlsx'
name = 'Topology Discovery Time'
cols = readExcel(filepath,name)
lists,labels = shoplistaExcel(cols)
x = [int(i) for i in lists[0]]
Y = lists[-1:]
y_label = lists[1][:3]
y = pivot(Y,y_label)
#print lists,labels
#print x,Y,y_label
#print x,y,y_label
bar_picture(x,y,y_label,3,0.5)

plt.title('Topology Discovery Time',fontsize=16)
#plt.xlim(-0.1,len(x))
#plt.ylim(0.0,600.0)
plt.xlabel('Number of Switches')
plt.ylabel('Time(ms)')
#plt.xticks(inx+w,x)
plt.show()
#
