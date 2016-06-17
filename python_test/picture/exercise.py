#!/usr/bin/python
from Excel_Read import *
from Excel_picture import *
import xlrd
import matplotlib.pyplot as plt
import numpy as np
class picture_FlowSetupRate:
    filepath = '/home/ubuntu-user/test_data.xlsx'
    name = 'Flow Setup Rate'
    value_list = readfile(filepath,name)
    value = []
#    for i in value_list:
#        v = notNone(i)
#        value.append(v)
#    l = len(value)
#    X = value[0][1:]
#    x_label = value[0][0]
#    Y = value[l-1][1:]
#    y_label = value[l-1][0]
#print X,Y
#    shoplist(value_list)
    X,Y,x_label,y_label = shoplist(value_list)
    width = 0.3
    label = 'Flow_mod rate'
    n_line = 1
    bar_picture(n_line,X,Y,name,width,label)
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    y_max = max(Y)*1.25
    inx = np.arange(len(X))
    plt.title(name,fontsize=16)
    plt.xlim(-0.1,len(X)+0.1)
    plt.ylim(0.0,y_max)
    plt.xticks(inx+width,X)
    plt.show()
class picture_ControlChannelCapacityTest():
    filepath = '/home/ubuntu-user/test_data.xlsx'
    name = 'Control Channel Capacity Test'
    value_list = readfile(filepath,name)
    X,Y,x_label,y_label = shoplist(value_list)
    width = 0.2
    label = ''
    bar_picture(3,X,Y,name,0.2,label)

