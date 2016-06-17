#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
from readfile_templates import *
from graph_templates import *

filepath = '/home/bii/Documents/ONOS_test/onos_test_data_standalone.xlsx'
name='Flow Setup Rate'
cols = readExcel(filepath,name)
lists,tables = shoplistaExcel(cols)
#print lists,tables
x = [int(i) for i in lists[0]]
y = [[int(i) for i in lists[-1]]]
y_table = tables[-1:]
#print x,y,y_table
bar_picture(x,y,y_table,1,0.4)
plt.title('Flow Setup Rate',fontsize=16)
#plt.xlim(-0.1,len(X))
#plt.ylim(0.0,2000.0,)
plt.xlabel('Packet_in Rate')
plt.ylabel('Flow_mod Rate')
#plt.xticks(inx+width,X)
plt.show()
