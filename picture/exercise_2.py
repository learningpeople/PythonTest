#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
from pylab import *

plt.figure(figsize=(10,6),dpi=40)
plt.subplot(1,1,1)
x = np.linspace(-np.pi,np.pi,256,endpoint=True)
C,S =np.cos(x),np.sin(x)

plt.plot(x,C,color="blue",linewidth=1.0,linestyle="-")
plt.plot(x,S,color="red",linewidth=1.0,linestyle="-")

plt.xlim(x.min()*1.1,x.max()*1.1)
##plt.xlim(-4.0,4.0)
##plt.xticks(np.linspace(-4,4,9,endpoint=True))
##xticks([-np.pi,-np.pi/2,0,np.pi/2,np.pi])
plt.xticks([-np.pi,-np.pi/2,0,np.pi/2,np.pi],
           [r'$-\pi$',r'$-\pi/2$',r'$0$',r'$+\pi/2$',r'$+\pi$'])
#plt.xlim(x.min()*1.1,x.max()*1.1)

plt.ylim(C.min()*1.1,C.max()*1.1)
##plt.ylim(-1.0,1.0)
##plt.yticks(np.linspace(-1,1,5,endpoint=True))
##yticks([-np.pi,-no.pi/2,0,np/pi/2,np.pi],[r'$-\pi$',r'$-\pi/2$',r'$0$',r'$+\pi/2$'r'$+\po$'])
plt.yticks([-1,+1],[r'$-1$',r'+1'])

plot(x,C,color="blue",linewidth=1.5,linestyle="-",label="cosLine")
plot(x,S,color="red",linewidth=2.5,linestyle="-",label="sinLine")

legend(loc='upper right')

ax = gca()
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
ax.xaxis.set_ticks_position('bottom')
ax.spines['bottom'].set_position(('data',0))
ax.yaxis.set_ticks_position('left')
ax.spines['left'].set_position(('data',0))

t = 2*np.pi/3
plt.plot([t,t],[0,np.cos(t)],color='blue',linewidth=2.5,linestyle='--')
scatter([t,],[np.cos(t),],50,color='blue')
#plt.annotate(r'$sin(\frac{2\pi}{3})=\frac{\sprt{3}}{2}$',
#         xy=(t,np.sin(t)),xycoords='data',
#         xytext=(+10.+30),textcoords='offset points',fontsize=16,
#         arrowprops=dict(arrowstyle="->",connectionstyle="arc3,rad=.2"))         arrowprops=dict(arrowstyle="->", connectionstyle="arc3,rad=.2"))
plt.annotate(r'$\sin(\frac{2\pi}{3})=\frac{\sqrt{3}}{2}$',
             xy=(t, np.sin(t)),  xycoords='data',
             xytext=(+10, +30), textcoords='offset points', fontsize=16,
             arrowprops=dict(arrowstyle="->", connectionstyle="arc3,rad=.2"))
plt.plot([t,t],[0,np.sin(t)], color='red', linewidth=2.5, linestyle="--")
scatter([t,],[np.sin(t),],50,color='red')
plt.annotate(r'$\cos(\frac{2\pi}{3}=-\frac{1}{2})$',
         xy=(t,np.cos(t)), xycoords='data',
         xytext=(-90, -50), textcoords='offset points',fontsize=6,
         arrowprops=dict(arrowstyle="->", connectionstyle="arc3,rad=.2"))
#plt.annotate(r'$\cos(\frac{2\pi}{3})=-\frac{1}{2}$',
#             xy=(t, np.cos(t)),  xycoords='data',
#             xytext=(-90, -50), textcoords='offset points', fontsize=16,
#             arrowprops=dict(arrowstyle="->", connectionstyle="arc3,rad=.2"))

for label in ax.get_xticklabels()+ax.get_yticklabels():
    label.set_fontsize(16)
    label.set_bbox(dict(facecolor="white",edgecolor="None",alpha=0.65))

plt.show()
