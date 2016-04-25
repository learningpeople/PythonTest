#!/usr/bin/python
from readfile_templates import *
from graph_templates import *
import matplotlib.pyplot as plt
import numpy as np
class graph_flowmodrate():
    l = ('min','max','avg',)
    filepath = '/home/ubuntu-user/OFsuite_flowmodrate.log'
    v = readlog(filepath,l)
#    print v
    x,y = shoplistlog(v)
    #print x,y
    bar_picture(x,y,x,1,0.2)
    plt.title ('Flow_mod Rata',fontsize=16)
    plt.ylim(0.0,9.0)
    plt.xlabel('Latency')
    plt.ylabel('Time(ms)')
    plt.show()
class graph_topologydisc():
    l = ('start','end')
    filepath = '/home/ubuntu-user/OFsuite_topologydisc.log'
    v = readlog(filepath,l)
#    print v
    x,y = shoplistlog(v)
#    print x,y
    Y =[[y[0][1]-y[0][0]]]
    X = ['discovery_time']
    print Y
    bar_picture(X,Y,X,1,0.2)
#    plt.show()
    plt.title('Topology Discovery Time',fontsize=16)
    plt.ylim(0.0,12.0)
    plt.xlabel('Topology')
    plt.ylabel('Times(ms)')
    plt.show()
class graph_linkconnectivity:
    l = ('linear','switches')
    filepath = '/home/ubuntu-user/OFsuite_linkconnectivity.log'
    v = readlog(filepath,l)
    x,y = shoplistlog(v)
#    print x,y
    X = [y[0][0]]
    Y = [[y[0][1]]]
    Y_label = x[1]
    bar_picture(X,Y,Y_label,1,0.2)
    plt.title('Link Connect Time',fontsize=16)
    plt.ylim(0.0,350)
    plt.xlabel('Switches')
    plt.ylabel('Time(ms)')
    plt.show()
class graph_maxsw:
    """"""
class graph_packetoutrate:
    l = ('min','max','avg')
    filepath = '/home/ubuntu-user/OFsuite_packetoutrate.log'
    v = readlog(filepath,l)
    x,y = shoplistlog(v)
#    print x,y
    bar_picture(x,y,x,1,0.2)
    plt.title('Packet_out Rate',fontsize=16)
    plt.ylim(0.0,12.0)
    plt.xlabel('Packet_out Latency')
    plt.ylabel('Time(ms)')
    plt.show()
class graph_topologyredisc:
    l = ('linear')
    filepath = '/home/ubuntu-user/OFsuite_topologyredisc.log'
    v = readlog(filepath,l)
    v = v[-1:]
    print v
    x,y = shoplistlog(v)
    print x,y
    bar_picture(x,y,x,1,0.2)
    plt.title('Topology Rediscovery Time',fontsize = 16)
    plt.ylim(0.0,11000.0)
    plt.xlabel('Switches')
    plt.ylabel('Time(ms)')
    plt.show()
#class 
#s = [' '.join(i[2:]) for i in v]
#ss = [i[]]
#print s
#for ss in s :
#    v = ''.join(s)
#    vl.append(v)
#print vl
    
    
#y = [i[1] for i in v]
#x = [i[2]+' '+i[3] for i in v]
#print x,y 

#v = []
#with open('/home/ubuntu-user/OFsuite_flowmodrate.log') as f:
#    for line in f.readlines():
#        for i in l:
#            if i in line: 
#                v.append(line.split())

#print v
