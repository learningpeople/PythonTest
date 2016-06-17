#!/usr/bin/python
#import xlrd
import matplotlib.pyplot as plt
import numpy as np
from readfile_templates import *
from graph_templates import *
#import math
r = []
l = []
ll = []
filepath = '/home/bii/Documents/ONOS_test/onos_test_data_standalone.xlsx'
name = 'Reactive Path Setup Time'
cols = readExcel(filepath,name)
lists,labels = shoplistaExcel(cols)
#print lists,labels
x = [int(i) for i in lists[0]]
Y = lists[-1:]
y1 = [y for y in Y[0][::3]]
y2 = [y for y in Y[0][1::3]]
y3 = [y for y in Y[0][2::3]]
y = [y1,y2,y3]
y_label = lists[1][:3]
#print x,y,y_label
bar_picture(x,y,y_label,3,0.28)
plt.title('Reactive Path Setup Time',fontsize=16)
#plt.xlim(-0.1,len(x))
#plt.ylim(0.0,1500.0)
plt.xlabel('Number of Switches')
plt.ylabel('Path Setup Time(ms)')
#plt.xticks(inx+w,x)
plt.show()
#
