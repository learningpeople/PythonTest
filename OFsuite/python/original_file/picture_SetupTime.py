#!/usr/bin/python
import xlrd
import matplotlib.pyplot as plt
import numpy as np
import math
r = []
l = []
ll = []
#l = []
filepath = '/home/bii/workspace/temp/test_data.xlsx'
test_data = xlrd.open_workbook(filepath)
sheet = test_data.sheet_by_name('Reactive Path Setup Time')
for row in range(sheet.nrows):
    r.append(sheet.row_values(row))
print r
print r[2:]
values = zip(r[2],r[3],r[4],r[5],r[6],r[7],r[8],r[9],r[10],r[11],r[12],r[13],r[14],r[15],r[16]) 
print values
#values = zip()
#print values
#x = tuple([v.append(i) for i in values[0] if i != ''])
for i in values[0]:
    if i != '':
        l.append(int(i))
#print l
x = tuple(l)
for ii in values[3]:
#    print max(ii)
    if ii == 'N/A':
        ii = 0
#        return ii
    ll.append(ii)
#y_max = math.ceil(max(ll))
#print ll,max(ll),y_max
 #    else:
#        return ii

a1 = tuple(ll[:3])
a2 = tuple(ll[3:6])
a3 = tuple(ll[6:9])
a4 = tuple(ll[9:12])
a5 = tuple(ll[12:])
#print a1,a2,a3
b = zip(a1,a2,a3,a4,a5)
y1 = b[0]
y2 = b[1]
y3 = b[2]
print x,y1,y2,y3

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
n = 5
inx = np.arange(n)
rec1 = ax.bar(inx,y1,w,color='r',alpha=0.5,label='Linear')#,edgecolor='w',label='Linear')
rec2 = ax.bar(inx+w,y2,w,color='b',alpha=0.5,label='Ring')#,edgecolor='w',label='Ring')
rec3 = ax.bar(inx+w*2,y3,w,color='y',alpha=0.5,label='leaf_spine')#,edgecolor='w',label = 'leaf_spine')
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
plt.ylim(0.0,1500.0)
plt.xlabel('Number of Switches')
plt.ylabel('Path Setup Time(ms)')
plt.xticks(inx+w,x)
plt.show()


#print x
#x = [eval(i) for i in values[0]
#x = tuple([i for i in values[0] if i > 0
#print x

