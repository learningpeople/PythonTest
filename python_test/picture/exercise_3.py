#1/usr/bin/python
from pylab import *
import numpy as np
import matplotlib.pyplot as plt
plt.figure(figsize=(10,5),dpi=80)
ax=plt.subplot(1,1,1)
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
ax.xaxis.set_ticks_position('bottom')
ax.spines['bottom'].set_position(('data',0))
ax.yaxis.set_ticks_position('left')
ax.spines['left'].set_position(('data',0))
n = 12
x = np.arange(n)
y1 = (1-x/float(n)*np.random.uniform(0.5,1.0,n))
y2 = (1-x/float(n)*np.random.uniform(0.5,1.0,n))

plt.bar(x,+y1,facecolor='#9999ff',edgecolor='white')
plt.bar(x,-y2,facecolor='#ff9999',edgecolor='white')

for a,b in zip(x,y1):
    text(a+0.4,b+0.05,'%.2f'%b,ha='center',va='bottom')
for a,b in zip(x,y2):
    text(a+0.4,-b-0.05,'%.2f'%b,ha='center',va='top')
plt.xlim(-.5,n),plt.xticks([])
plt.ylim(-1.25,+1.25),plt.yticks([])

#savefig('../mnt/hgfs/bar_ex.png',dpi=48)

plt.show()    
