#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
import xlrd
from Excel_Read import *
from Excel_picture import *
valuelist= []
filepath = '/home/ubuntu-user/test_data.xlsx'
test_data = 'Topology Discovery Time'
col = readfile(filepath,test_data)
print col
lists,labels = shoplist(col)
print lists,labels
x = lists[1]
y = lists[3:4]
label = lists[1]
print y
bar_picture(x,y,label,1,0.3)
#plt.show()


#for i in col:
#    co = notNone(i)
#    valuelist.append(co)
#print valuelist




#sheet = test_data.sheet_by_name('Topology Discovery Time')
#sheet = test_data.sheet_by_name('Topology Rediscovery Time')
#sheet = test_data.sheet_by_name('Packet_out Response Rate')
#sheet = test_data.sheet_by_name('Flow Setup Rate')
#for row in range(sheet.nrows):
#    row_valuelist.append(sheet.row_values(row))
#print row_valuelist
#l1 = row_valuelist[4]
#l2 = row_valuelist[5]
#l3 = row_valuelist[6]
#l4 = row_valuelist[7]
#l5 = row_valuelist[8]
#l6 = row_valuelist[9]
    
#values = zip(l1,l2,l3,l4)#,l5,l6)
#X = values[0]
#Y = values[5]
#print values,X,Y
#plt.gca()
#plt.figure(figsize=(10,7),dpi=80,facecolor='w')
#plt.figure(figsize=(10,7),dpi=80)
#ax= plt.subplot(1,1,1)

#ax.yaxis.set_major_locator(plt.MultipleLocator(500.0))
#ax.grid(which='major',axis='y',linewidth=0.5,linestyle='--',color='0.75')
#ax.spines['right'].set_color('none')
#ax.spines['top'].set_color('none')
#ax.xaxis.set_ticks_position('bottom')
#ax.spines['bottom'].set_position(('data',0))
#ax.yaxis.set_ticks_position('left')
#ax.spines['left'].set_position(('data',-0.1))

#n=len(X)
#color = ['r','b','y']
#inx = np.arange(n)
#width = 0.27
#for i,y in enumerate(Y):
#    ax.bar(i+0.1,y,width,alpha=0.5,color='m',label=X[i])
#    ax.text((i+0.25),y+10,'%.2f'%(y),ha='center',va='bottom',fontsize=10)
#    ax.text(inx+0.2+width/2,y+10,'%.2f'%y,ha='center',va='bottom',fontsize=10)
#    bar(left,height,width,**kwargs)

#bar=ax.bar(inx+0.2,Y,width,alpha=1,color='c',edgecolor='w')#,label='packet_out rate')
#for i in bar:
#    h =float(i.get_height)
#    h = i.get_height()
#    h = i.get_y() 
#   print h,i.get_x(),i.get_width()
#    ax.text(i.get_x()+i.get_width()/2.0,h*1.05,'%.2f'%(h),ha='center',va='bottom',fontsize=10)
#ax.legend(loc='upper left')
plt.title('Topology Discovery Time')
#plt.title('Topology Rediscovery Time')
#plt.title('Packet_out Response Rate')
#plt.title('Flow Setup Rate',fontsize=16)
#plt.xlim(-0.1,len(X))
plt.ylim(0.0,600.0)
#plt.ylim(0.0,2000.0,)
plt.xlabel('Topology Types')
#plt.xlabel('Packet_in Rate')
plt.ylabel('Flow_mod Rate')
#plt.ylabel('Time(ms)')
#plt.xticks(inx+width,X)
plt.show()
#plt.gcf()
#plt.savefig('TopologyDiscoveryTime.png',format='png')
#plt.close()
