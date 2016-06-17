#!/usr/bin/python
from readfile_templates import *
from graph_templates import *
import matplotlib.pyplot as plt
import numpy as np
def graph_flowmodrate(arg):
    l = ('min','max','avg',)
    logfile = arg#'log/OFsuite_flowmodrate.log'
#    path = get_path()
#    filepath = os.path.join(path,logfile) 
    v = readlog(logfile,l)
#    print v
    x,y = shoplistlog(v)
    #print x,y
    bar_picture(x,y,x,1,0.2)
    plt.title ('Flow_mod Rata',fontsize=16)
#    plt.ylim(0.0,9.0)
    plt.xlabel('Latency')
    plt.ylabel('Time(ms)')
    plt.show()
def graph_topologydisc(arg):
    l = ('end','ring','linear','leaf-spine')
    filepath = arg#'log/OFsuite_topologyredisc.log'
    v = readlog(filepath,l)
#    v = v[-1:]
#    l = ('start','end')
    #filepath = '/home/bii/workspace/test_tool/OFsuite_performance/log/OFsuite_topologydisc.log'
#    filepath = arg
#    v = readlog(filepath,l)
#    print v
    x,y = shoplistlog(v)
#    print x,y
    X = [int(y[0][0])]
    Y = [[y[0][1]]]
    x_label = [x[1]]
#    print X,Y,x_label
    bar_picture(X,Y,x_label,1,0.2)
#    plt.show()
#    y_max = round(y[0][1],-2)
#    print y_max
    plt.title('Topology Discovery Time',fontsize=16)
#    plt.ylim(0.0,y_max)
    plt.xlabel('Number of Switches')
    plt.ylabel('Times(ms)')
    plt.show()
def graph_linkconnectivity(arg):
    l = ('linear','switches')
    filepath = arg#'log/OFsuite_linkconnectivity.log'
    v = readlog(filepath,l)
    x,y = shoplistlog(v)
#    print x,y
    X = [y[0][0]]
    Y = [[y[0][1]]]
    Y_label = x[1]
    bar_picture(X,Y,Y_label,1,0.2)
    plt.title('Link Connect Time',fontsize=16)
#    plt.ylim(0.0,350)
    plt.xlabel('Switches')
    plt.ylabel('Time(ms)')
    plt.show()
def graph_packetoutrate(arg):
    l = ('min','max','avg')
    filepath = arg#'log/OFsuite_packetoutrate.log'
    v = readlog(filepath,l)
    x,y = shoplistlog(v)
#    print x,y
    bar_picture(x,y,x,1,0.2)
    plt.title('Packet_out Rate',fontsize=16)
#    plt.ylim(0.0,12.0)
    plt.xlabel('Packet_out Latency')
    plt.ylabel('Time(ms)')
    plt.show()
def graph_topologyredisc(arg):
#    l = ('linear')
    l = ('end','ring','linear','leaf-spine')
    filepath = arg#'log/OFsuite_topologyredisc.log'
    v = readlog(filepath,l)
#    v = v[-1:]
#    print v
    x,y = shoplistlog(v)
#    print x,y
#    bar_picture(x,y,x,1,0.2)
    X = [int(y[0][0])]
    Y = [[y[0][1]]]
    x_label = [x[1]]
#    print X,Y,x_label
    bar_picture(X,Y,x_label,1,0.2)
    plt.title('Topology Rediscovery Time',fontsize = 16)
#plt.ylim(0.0,11000.0)
    plt.xlabel('Type of Switches')
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
    
    
#[i[1] for i in v]
#x = [i[2]+' '+i[3] for i in v]
#print x,y 

#v = []
#with open('/home/ubuntu-user/OFsuite_flowmodrate.log') as f:
#    for line in f.readlines():
#        for i in l:
#            if i in line: 
#                v.append(line.split())

#print v
if __name__ == '__main__':
    print 'test result'
else:
    print 'test result'
