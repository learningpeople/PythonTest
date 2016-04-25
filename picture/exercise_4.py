#!/usr/bin/python
import numpy as np
import PIL
import pylab as pl
import string
#a = np.loadtxt('/home/ubuntu-user/OFsuite_maxsw1.txt')
#print a[1]
maxsw=[]
with open('/home/ubuntu-user/OFsuite_maxsw.txt') as f:
#    print f.read()
    for line in f.readlines():
       # for i in range(0,20):
        maxsw.append(line.split())
print maxsw[0]
print maxsw[1]
print maxsw[1][0]
num = string.atof(maxsw[100][0])
numm=string.atof(maxsw[1][0])
num2=num-numm
num1=eval(maxsw[100][0])-eval(maxsw[1][0])
print num1,num2

x = np.arange(0.0,10.0,1.0)
y = x*x
print 'x=',x
print 'y=',y
f = open('/home/ubuntu-user/testdata.txt','w')
for i in range(len(x)):
    txt = str(x[i])+'\t'+str(y[i])+'\n'
    f.write(txt)
#print f.read()
f.close()
data = np.loadtxt("/home/ubuntu-user/testdata.txt")
pl.plot(data[:,0],data[:,1],'ro')
pl.xlabel('x')
pl.xlabel('y')
pl.xlim(0.0,10.0)
pl.show()


#num1 = string.atoi(maxsw[1][0])
#print num1
#print maxsw
        #maxsw=
        #print maxsw
        #print line
        #print len(line)
#    rint f.read()
