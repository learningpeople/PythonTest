#!/usr/bin/python
from Excel_Read import *
from Excel_picture import *
import matplotlib.pyplot as plt
import numpy as np
path = '/home/ubuntu-user/test_data.xlsx'
name = 'Packet_out Response Rate'
cols = readfile(path,name)
lists,labels = shoplist(cols)
print lists,labels
x = lists[0]
x_label = labels[0]
y = lists[2:5]
y_label = labels[2:5]

plt.figure(figsize=(10,7),dpi=80)
ax = plt.subplot(1,1,1)
ax.grid(which='major',axis='y',linewidth=0.5,linestyle='--',color='0.75')
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
ax.xaxis.set_ticks_position('bottom')
ax.spines['bottom'].set_position(('data',0))
ax.yaxis.set_ticks_position('left')
ax.spines['left'].set_position(('data',-0.1))
w = 0.28
n = len(x)
inx = np.arange(n)
rec1 = ax.bar(inx,y[0],w,color='r',alpha=0.5,label=y_label[0])#,edgecolor='w',label='Linear')
rec2 = ax.bar(inx,y[1],w,color='b',alpha=0.5,label=y_label[1])#,edgecolor='w',label='Ring')
rec3 = ax.bar(inx,y[2],w,color='y',alpha=0.5,label=y_label[2])#,edgecolor='w',label = 'leaf_spine')
ax.legend(loc='upper left')
def autolabel(rec):
    for i in rec:
        h = i.get_height()
        if h > 0 :
            ax.text(i.get_x()+i.get_width()/2,10+h,'%.2f'%h,ha='center',va='bottom',fontsize=9)
        else:
            ax.text(i.get_x()+i.get_width()/2,h+10,'N/A',ha='center',va='bottom',fontsize=12)
autolabel(rec1)
autolabel(rec2)
autolabel(rec3)
plt.title('Reactive Path Setup Time',fontsize=16)
plt.xlim(-0.1,len(x))
plt.ylim(0.0,300.0)
plt.xlabel('Number of Switches')
plt.ylabel('Path Setup Time(ms)')
plt.xticks(inx+w,x)
plt.show()


#print x
#x = [eval(i) for i in values[0]
#x = tuple([i for i in values[0] if i > 0
#print x

