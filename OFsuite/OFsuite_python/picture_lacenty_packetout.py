#!/usr/bin/python
from readfile_templates import *
from graph_templates import *
import matplotlib.pyplot as plt
import numpy as np
from pylab import *
path = '/home/bii/Documents/ONOS_test/onos_test_data_standalone.xlsx'
name = 'Packet_out Response Rate'
cols = readExcel(path,name)
lists,labels = shoplistaExcel(cols)
#print lists,labels
x = lists[0]
y_label = labels[2:5]
y = lists[2:5]
#print x,y,y_label
plot_picture(x,y,y_label)
plt.title('Packet_out Response Latency')
plt.xlabel('packet_in rate')
plt.ylabel('Latency(ms)')
plt.show()
#

