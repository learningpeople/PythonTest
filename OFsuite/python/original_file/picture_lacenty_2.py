#!/usr/bin/python
from Excel_Read import *
from Excel_picture import *
import matplotlib.pyplot as plt
import numpy as np
from pylab import *
path = '/home/ubuntu-user/test_data.xlsx'
name = 'Packet_out Response Rate'
cols = readfile(path,name)
lists,labels = shoplist(cols)
#print lists,labels
x = lists[0]
x_label = labels[0]
x1 = x[:3]
x2 = x[3:]
y = lists[2:5]
y1 = [i[:3] for i in y]
y2 = [i[3:] for i in y]
#print y1
y_label = labels[2:5]
plt.figure(figsize=(12,8),dpi=70)
ax1 = plt.subplot(2,1,1)
ax2 = plt.subplot(2,1,2)
ax1.grid(axis='y',linewidth=0.5,linestyle='--',color='0.75')
ax2.grid(axis='y',linewidth=0.5,linestyle='--',color='0.75')
n = len(y1)
color = ['r','b','y','m','c']
for i in range(n):
    plt.sca(ax1)
    rec1=plt.plot(x1,y1[i],color=color[i],linewidth=1.0,linestyle='-',marker='o',label=y_label[i])
    plt.sca(ax2)
    rec2=plt.plot(x2,y2[i],color=color[i],linewidth=1.0,linestyle='-',marker='o',label=y_label[i])

ax1.legend(loc='upper left')
ax2.legend(loc='upper left')
ax = gca()
#def autolabel(x,y):
#    for i in y:
#        if i > 0:
#            ax.text(x,i+10,'%.2f'%i,ha='center',va='bottom',fontsize=10)
#        else:
#            ax.text(x,i+10,'N/A',ha='center',va='bottom',fontsize=12)    
#autolabel(x1,y1)
#autolabel(rec2)

  
#for xy in zip(x,y):
#    plt.sca(ax1)
#    plt.annotate(xy[1],xy=xy,xytext=(+5,+5),xycoords='data', fontsize=6)


plt.show()






















