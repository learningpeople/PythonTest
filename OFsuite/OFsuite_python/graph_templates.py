#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
import math
#def verify(num):
def bar_picture(X,Y,label,n_line,width):
    rec = []
    n_group = len(X)
    inx = np.arange(float(n_group))
    for i in range(n_group):
        if i is 0:
            inx[i] = inx[i] + 0.5*width
        if i is not 0:
            inx[i] = inx[i-1] + (len(Y)+0.8)*width
    #    print inx[i]
    #figure()
    plt.figure(figsize=(max(inx)+(len(Y)+1)*width,6),dpi=80)
    plt.subplot(1,1,1)
    ax = plt.gca()   
    ax.grid(axis='y',linewidth=0.5,linestyle='--',color='0.75')
    ax.spines['right'].set_color('none')
    ax.spines['top'].set_color('none')
    ax.xaxis.set_ticks_position('bottom')
    ax.spines['bottom'].set_position(('data',0.0))
    ax.yaxis.set_ticks_position('left')
    ax.spines['left'].set_position(('data',-0.1))
    color = ['b','r','y','m']
    def autolabel(rec):
        for i in rec:
            h = i.get_height()
            if h > 0:
                if isinstance(h,int) == True:
                    ax.text(i.get_x()+i.get_width()/2.0,h*1.04,'%d'%h,ha='center',va='bottom',fontsize=10)
                else:
                    ax.text(i.get_x()+i.get_width()/2.0,h*1.04,'%.2f'%h,ha='center',va='bottom',fontsize=10)
            else:
                ax.text(i.get_x()+i.get_width()/2.0,10+h,'N/A',ha='center',va='bottom',fontsize=12)
    for i in range(n_line):
        r= ax.bar(inx+width*i,Y[i],width,alpha=0.5,color=color[i],label=label[i])
        rec.append(r)
    plt.xticks(inx + len(Y)*width/2, X)
    for i in rec:
        autolabel(i)
    v_max = []
    for n in Y:
        v_max.append(math.ceil(max(n)))
    y_max = max(v_max)
    y_lim = round(y_max,-len('%d'%y_max)+1)
    ax.legend(loc='upper left')
    plt.xlim(-0.1,max(inx)+(len(Y)+1)*width)
    plt.ylim(0.0,y_lim*1.5)
#    plt.xlabel()
#    plt.ylabel()
    #plt.xticks(inx+width*1.3,X)
#    plt.xticks(inx+0.2+width*(inx-1),X)
#    plt.show()
def plot_picture(X,Y,y_label):
    plt.figure(figsize=(10,6),dpi=80)
    plt.subplot(1,1,1)
    ax = plt.gca()   
    ax.grid(axis='y',linewidth=0.5,linestyle='--',color='0.75')
    ax.spines['right'].set_color('none')
    ax.spines['top'].set_color('none')
    ax.xaxis.set_ticks_position('bottom')
    ax.spines['bottom'].set_position(('data',0.0))
    ax.yaxis.set_ticks_position('left')
    ax.spines['left'].set_position(('data',min(X)-10))
    color = ['b','r','y','m']
    n = len(Y)
    for i in range(n):
        plt.sca(ax)
        rec=plt.plot(X,Y[i],color=color[i],linewidth=1.0,linestyle='-',label=y_label[i],marker='o')
    ax.legend(loc='upper left')
    plt.xlim(min(X)-10,max(X)+10)
    

