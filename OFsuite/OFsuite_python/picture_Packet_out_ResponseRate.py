#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
from readfile_templates import *
from graph_templates import *

filepath = '/home/bii/Documents/ONOS_test/onos_test_data_standalone.xlsx'
name = 'Packet_out Response Rate'
cols = readExcel(filepath,name)
lists,labels = shoplistaExcel(cols)
x = [int(i) for i in lists[0]]
y = [[int(i) for i in lists[-1]]]
y_label = labels[-1:]
#print x,y,y_label
bar_picture(x,y,y_label,1,0.5)
plt.title('Packet_out Response Rate',fontsize=16)
#plt.xlim(-0.1,len(X))
#plt.ylim(0.0,6000.0)
plt.xlabel('Packet_in Rate')
plt.ylabel('packet_out Rate')
#plt.xticks(inx+width,X)
plt.show()
