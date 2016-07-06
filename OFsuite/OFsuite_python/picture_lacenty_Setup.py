#!/usr/bin/python
from readfile_templates import *
from graph_templates import *
import matplotlib.pyplot as plt
import numpy as np
from pylab import *
path = '/home/bii/Documents/ONOS_test/onos_test_data_standalone.xlsx'
name = 'Flow Setup Rate'
cols = readExcel(path,name)
lists,labels = shoplistaExcel(cols)
x = lists[0]
y = lists[2:5]
y_label = labels[2:5]
plot_picture(x,y,y_label)
plt.title('Flow Setup Latency')
plt.xlabel('Packet_in Rate')
plt.ylabel('Latency(ms)')
plt.show()
#
