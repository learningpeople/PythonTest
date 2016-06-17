#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt
from readfile_templates import *
from graph_templates import *
maxsw = []
name = []
filepath = '/home/ubuntu-user/test_data.xlsx'
name = 'Control Channel Capacity Test'
cols = readExcel(filepath,name)
lists,labels = shoplistaExcel(cols)
x = [int(i) for i in lists[0]]
Y = lists[4]
y1 = [y for y in Y[::3]]
y2 = [y for y in Y[1::3]]
y3 = [y for y in Y[2::3]]
y = [y1,y2,y3]
y_label = lists[1][:3]
#print x,y1,y2,y3,y_label
bar_picture(x,y,y_label,3,0.32)
plt.title('Virtual Memory Usage')
#plt.xlim(-0.1,len(x)+0.1)
#plt.ylim(0.0,140.0)
plt.xlabel('Number of Switches')
plt.ylabel('Usage(MB)')
#plt.xticks(index+bar_width,x)
plt.show()
#
#plt.savefig('/home/ubuntu-user/Maxsw_PhysicalMemory.png',dpi=80)
#          
