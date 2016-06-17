#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
from readfile_templates import *
from graph_templates import *
#import xlrd

row_valuelist= []
filepath = '/home/ubuntu-user/test_data.xlsx'
name = 'Topology Rediscovery Time'
cols = readExcel(filepath,name)
lists,labels = shoplistaExcel(cols)
x = lists[1]
y = lists[-1:]
y_label = labels[-1:]
bar_picture(x,y,y_label,1,0.27)
plt.title('Topology Rediscovery Time',fontsize=16)
#plt.xlim(-0.1,len(X))
#plt.ylim(0.0,60000.0)
plt.xlabel('Topology Types')
plt.ylabel('Time(ms)')
#plt.xticks(inx+width,X)
plt.show()
