#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
def bar_picture(X,Y,label,n_line,width):
    rec = []
    plt.figure(figsize=(10,6),dpi=80)
#    ax = plt.subplot(1,1,1)
    plt.subplot(1,1,1)
    ax = plt.gca()   
    ax.grid(axis='y',linewidth=0.5,linestyle='--',color='0.75')
    ax.spines['right'].set_color('none')
    ax.spines['top'].set_color('none')
    ax.xaxis.set_ticks_position('bottom')
    ax.spines['bottom'].set_position(('data',0))
    ax.yaxis.set_ticks_position('left')
    ax.spines['left'].set_position(('data',-0.1))
    n_group = len(X)
    inx = np.arange(n_group)
#    print inx
    color = ['b','r','y','m']
    def autolabel(rec):
        for i in rec:
            h = i.get_height()
#            print h
            if h > 0:
                ax.text(i.get_x()+i.get_width()/2.0,h*1.04,'%.2f'%h,ha='center',va='bottom',fontsize=10)
            else:
                ax.text(i.get_x()+i.get_width()/2.0,10+h,'N/A',ha='center',va='bottom',fontsize=12)
#    for i in range(n_line):
 #   for y in Y:
    for i in range(n_line):
#        print Y[i]
        r= ax.bar(inx+0.1+width*i,Y[i],width,alpha=0.5,color=color[i],label=label[i])
#        autolabel(r)
        rec.append(r)
#    print rec
    for i in rec:
        autolabel(i)
#        print a        

#    return rec
    
#    if n_line > 1:
#        for y in Y:
#            toax.bar(inx+0.1,y,width=width,alpha=0.5,color)
#            for i in range(n_line):
#                rec[i] = ax.bar(i+0.1,y[i],width=width,alpha=0.5,color=color[i],label=label[i])
#            autolabel(rec[i])
#    else: 
#        rec = ax.bar(inx+0.1,Y,width,alpha=0.5,color=color[3],label=label)
#        autolabel(rec)
#    return rec
#    plt.xticks(inx+width,X)
#    y_max = max(Y)*1.25
#    ax.legend(loc='upper left')
#    plt.title(name,fontsize=16)
#    plt.xlim(-0.1,len(X)+0.1)
#    plt.ylim(0.0,y_max)
#    plt.xticks(inx+width,X)

#    def autolabel(rec):
#        for i in rec:
#            h = i.get_height()
#            if h > 0:
#                ax.text(i.get_x()+i.get_width()/2.0,10+h,'%.2f'%h,ha='center',va='bottom',fontsize=10)
#            else:
#                ax.text(i.get_x()+i.get_width()/2.0,10+h,'N/A',ha='center',va='bottom',fontsize=12)
    
#    y_max = max(Y)*1.25
    ax.legend(loc='upper left')
#    plt.title(name,fontsize=16)
    plt.xlim(-0.1,len(X)*1.05)
#    plt.ylim(0.0,y_max)
#    plt.xlabel()
#    plt.ylabel()
    plt.xticks(inx+width,X)
#    plt.show()


def plot_picture(X,Y,y_label,x_label):
    plt.figure(figsize=(10,6),dpi=80)
    ax = plt.subplot(1,1,1)
    ax.spines['right'].set_color('none')
    ax.spines['top'].set_color('none')
    ax.xaxis.set_ticks_position('bottom')
    ax.spines['bottom'].set_position(('data',0))
    ax.yaxis.set_ticks_position('left')
    ax.spines['left'].set_position(('data',0))
    color = ['r','b','y','m','c']
    n = len(Y)
#    m = len(X)
#    inx = np.arange(m)
    for i in range(n):
        ax.plot(X,Y[i],color=color[i],linewidth=1.0,linestyle='-',label=y_label[i])
    ax.legend(loc='upper left')
#    plt.xlim(X.min()*1.1,X.max()*1.1)
#    plt.ylim(Y.min()*1.1,Y.max()*1.1)
    plt.xticks([])
    plt.yticks([])
    

