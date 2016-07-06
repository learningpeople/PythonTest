#!/usr/bin/python
from readfile_templates import *
from graph_templates import *
import matplotlib.pyplot as plt
import numpy as np
#from pyplot import *
def graph_flowmodrate(arg):
    l = ('min','max','avg','rate',)
    logfile = arg#'log/OFsuite_flowmodrate.log'
    v = readlog(logfile,l)
    X,Y = shoplistlog(v)
    x1 = X[2:]
    y1 = [Y[0][2:]]
    y_label = ['latency']
    #print x1,y1
    bar_picture(x1,y1,y_label,1,0.2)
    plt.title ('Flow_mod Latency',fontsize=16)
    plt.xlabel('Latencies')
    plt.ylabel('Time(ms)')
    x2 = [int(x) for x in Y[0][:1]]
    y2 = [[int(y)for y in Y[0][1:2]]]
    y2_label = [X[1]]
    #print x2,y2,y2_label
    bar_picture(x2,y2,y2_label,1,0.3)
    plt.title('Flow_mad Rate',fontsize=16)
    plt.xlabel('Packet_in Rate')
    plt.ylabel('Flow_mode Rate')
    plt.show()
def graph_topologydisc(arg):
    l = ('end','ring','linear','leaf-spine')
    filepath = arg#'log/OFsuite_topologyredisc.log'
    v = readlog(filepath,l)
    x,y = shoplistlog(v)
    X = [int(y[0][0])]
    Y = [[y[0][1]]]
    x_label = [x[1]]
    bar_picture(X,Y,x_label,1,0.2)
    plt.title('Topology Discovery Time',fontsize=16)
    plt.xlabel('Number of Switches')
    plt.ylabel('Times(ms)')
    plt.show()
def graph_linkconnectivity(arg):
    l = ('linear','switches')
    filepath = arg#'log/OFsuite_linkconnectivity.log'
    v = readlog(filepath,l)
    x,y = shoplistlog(v)
    X = [y[0][0]]
    Y = [[y[0][1]]]
    Y_label = x[1]
    bar_picture(X,Y,Y_label,1,0.2)
    plt.title('Link Connect Time',fontsize=16)
    plt.xlabel('Switches')
    plt.ylabel('Time(ms)')
    plt.show()
def graph_packetoutrate(arg):
    
    l = ('min','max','avg',)
    filepath = arg#'log/OFsuite_packetoutrate.log'
    v = readlog(filepath,l)
    x,y = shoplistlog(v)
    #figure(1)
    bar_picture(x,y,x,1,0.2)
    plt.title('Packet_out Latency',fontsize=16)
    plt.xlabel('Latencies')
    plt.ylabel('Time(ms)')
    ll = ('rate',)
    v1 = readlog(filepath,ll)
    X,Y = shoplistlog(v1)
    #print X,Y
    y_label = X[1:]
    x1 = [int(x) for x in Y[0][:1]]
    y1 = [[int(y) for y in Y[0][1:]]]
    #print x,y,y_label
    #figure(2)
    bar_picture(x1,y1,y_label,1,0.3)
    plt.title('Packout_out Rate')
    plt.xlabel('Packet_in')
    plt.ylabel('Packet_out')
    plt.show()
def graph_topologyredisc(arg):
#    l = ('linear')
    l = ('end','ring','linear','leaf-spine')
    filepath = arg#'log/OFsuite_topologyredisc.log'
    v = readlog(filepath,l)
    x,y = shoplistlog(v)
    X = [int(y[0][0])]
    Y = [[y[0][1]]]
    x_label = [x[1]]
    bar_picture(X,Y,x_label,1,0.2)
    plt.title('Topology Rediscovery Time',fontsize = 16)
    plt.xlabel('Type of Switches')
    plt.ylabel('Time(ms)')
    plt.show()
def graph_recovery_time(arg):
    l=('recovery',)
    filepath = arg
    v = readlog(filepath,l)
    X,Y = shoplistlog(v)
    x = [int(x) for x in Y[0][0]]
    y = [[Y[0][1]]]
    y_label = [X[1]]
    bar_picture(x,y,y_label,1,0.2)
    plt.title('Maater-slave Recover time')
    plt.xlabel('Number of controller')
    plt.ylabel('times')
    plt.show()
def graph_auxi_channel(arg):
    filepath = arg

if __name__ == '__main__':
    print ''
else:
    print ''
