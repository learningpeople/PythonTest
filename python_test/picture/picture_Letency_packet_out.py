#!/usr/bin/python
from Excel_Read import *
from Excel_picture import *
import matplotlib.pyplot as plt
import numpy as np
class pacture_latency_pack_out():
    path = '/home/ubuntu-user/test_data.xlsx'
    name = 'Packet_out Response Rate'
    cols = readfile(path,name)
    lists,labels = shoplist(cols)
#    print lists,labels
    x = lists[0]
    x_label = labels[0]
    y = lists[2:5]
    y_label = labels[2:5]
    print y,y_label
    bar_picture(3,x,y,0.3,y_label)
#    plt.figure(figsize=(10,6),dpi=80)
#    ax = plt.subplot(1,1,1)
#    ax.grid(axis='y',linewidth=0.5,linestyle='--',color='0.75')
#    ax.spines['right'].set_color('none')
#    ax.spines['top'].set_color('none')
#    ax.xaxis.set_ticks_position('bottom')
#    ax.spines['bottom'].set_position(('data',0))
#    ax.yaxis.set_ticks_position('left')
#    ax.spines['left'].set_position(('data',-0.1))
#    def autolabel(rec):
#        for i in rec:
#            h = i.get_height()
#            if h > 0:
#                ax.text(i.get_x()+i.get_width()/2.0,10+h,'%.2f'%h,ha='center',va='bottom',fontsize=10)
##            else:
#                ax.text(i.get_x()+i.get_width()/2.0,10+h,'N/A',ha='center',va='bottom',fontsize=12)
#    w = 0.3
#    inx = np.arange(len(x))
#    rec1 = ax.bar(inx,y[0],w,color='r',alpha=0.5,label=y_label[0])
#    rec2 = ax.bar(inx,y[1],w,color='b',alpha=0.5,label=y_label[1])
#    rec3 = ax.bar(inx,y[2],w,color='y',alpha=0.5,label=y_label[2])
#    ax.legend(loc='upper left')
#    def autolabel(rec):
#        global ax
#        for i in rec:
#            h = i.get_height()
#            if h > 0:
#                ax.text(i.get_x()+i.get_width()/2.0,10+h,'%.2f'%h,ha='center',va='bottom',fontsize=10)
#            else:
#                ax.text(i.get_x()+i.get_width()/2.0,10+h,'N/A',ha='center',va='bottom',fontsize=12)
#    autolabel(rec1)
#    autolabel(rec2)
#    autllabel(rec3)   
#    plt.title(name)
#    num = max(enumerate(y_list))
#    plt.xlim(0,max(x)*1.1)
#    plt.ylim(0,300)
#    plt.xticks(np.arange(len(x)),x)
#    plt.yticks([])A
    
    plt.show()
# y3_label = labels[4]
    
#    x = cols_value[0][1:]
#    x_label = cols_value[0]
#    y1 = cols_vluse[1][1:]
#    y1_label = value
    
