#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt
import string
import matplotlib
#import Image,ImageDraw
#matplotlib.use('Agg')
 
maxsw = []
name = []
with open('/home/ubuntu-user/data_maxsw.txt') as f :
#    print f.read()    
    for line in  f.readlines():
        maxsw.append(line.split())
l1 = maxsw[1]
l2 = maxsw[2]
l3 = maxsw[3]
l4 = maxsw[4]
l5 = maxsw[5]
#print zip(l1,l2,l3,l4,l5)
data = zip(l1,l2,l3,l4,l5)
#print data[0]

plt.figure(figsize=(12,7),dpi=80)
#plt.axes([0.025,0.025,0.95,0.95])
ax = plt.subplot(1,1,1)
ax.grid(axis='y',linewidth=0.5,linestyle='--',color='0.75')
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
ax.xaxis.set_ticks_position('bottom')
ax.spines['bottom'].set_position(('data',0))
ax.yaxis.set_ticks_position('left')
ax.spines['left'].set_position(('data',-0.1))
x = data[0]
y1 = tuple([eval(x1) for x1 in data[2]])
#y1 = tuple([eval(x1) for x1 in data[3]])
y2 = tuple([eval(x2) for x2 in data[5]])
#y2 = tuple([eval(x2) for x2 in data[6]])
y3 = tuple([eval(x3) for x3 in data[8]])
#y3 = tuple([eval(x3) for x3 in data[9]])
#print y1,y2,y3

bar_width = 0.25
n_group = 5
index = np.arange(n_group)
rects1=ax.bar(index,y1,bar_width,alpha=0.5,color='r',label='Linear')
rects2=ax.bar(index+bar_width,y2,bar_width,alpha=0.5,color='b',label='Ring')
rects3=ax.bar(index+bar_width*2,y3,bar_width,alpha=0.5,color='y',label='leaf-spine')
    #ax.set_xticks(index+bar_width)
    #plt.xticks(x)
    #ax.set_xtickslabels((x))
ax.legend(loc='upper left')
def autolabel(rects):
    for i in rects:
        height = i.get_height()
        if height > 0:
            ax.text(i.get_x()+i.get_width()/2.0,1.05*height,'%.2f'%(height),ha='center',va='bottom',fontsize=9)
        else:
            ax.text(i.get_x()+i.get_width()/2.0,height+0.1,'N/A',ha='center',va='bottom')
autolabel(rects1)
autolabel(rects2)
autolabel(rects3)
plt.title('Physical Memory Usage')
#plt.title('Virtual Memory Usage')
plt.xlim(-0.1,len(x)+0.1)
plt.ylim(0.0,100.0)
#plt.ylim(0.0,140.0)
#plt.xlabel(maxsw[0][0])
plt.xlabel('Number of Switches')
plt.ylabel('Usage(MB)')
#plt.ylabel(maxsw[0][2])
plt.xticks(index+bar_width,x)
#plt.yticks(range(0,100,10))
plt.show()

plt.savefig('/home/ubuntu-user/Maxsw_PhysicalMemory.png',dpi=80)
          
