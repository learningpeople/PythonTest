#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt

t = np.dtype([('word',str,40),('quantity',int)])

word,count = np.loadtxt('/home/ubuntu-user/OFsuite_maxsw.txt',delimiter=',',usecols=(0,1),dtype=t,unpack=True)

X = np.arange(20)
Y = np.count[-20:]

plt.figure(figsize=(14,6),dpi=80)

plt.bar(X,Y,facecolor="#9999ff",edgecolor="white")

for x,y in zip(X,Y):
    plt.text(x+0.4,y+0.05,'%d'%y,ha='center',va='bottom')

plt.xlim(0,X.max()*1.1),plt.xticks(X,word[-20:])
plt.ylim(0,Y.max()*1.1),plt.xticks([])

plt.savefig('/home/ubuntu-user/bar_ex.png',dpi=300)

plt.show()
