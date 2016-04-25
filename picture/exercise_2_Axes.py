#!/usr/bin/python
from pylab import *
import matplotlib.gridspec as gridspec

G = gridspec.GridSpec(3,3)
axes1 = subplot(G[0,:])
xticks([]),yticks([])
text(0.5,0.5,'Axes 1',ha='center',va='center',size=24,alpha=.5)

axes2 = subplot(G[1,:-1])
xticks([]),yticks([])
text(0.5,0.5,'Axes 2',ha='center',va='center',size=14,alpha=.5)

axes3 = subplot(G[1:,-1])
xticks([]),yticks([])
text(0.5,0.5,'Axes 3',ha='center',va='center',size=24,alpha=.10)

axes4 = subplot(G[-1,0])
xticks([]),yticks([])
text(0.5,0.1,'Axes-4',ha='left',va='center',size=24,alpha=.5)

axes5 =subplot(G[-1,1])
xticks([]),yticks([])
text(0.5,0.5,'Axes 5',ha='center',va='bottom',size=24,alpha=.5)

#plt.savefig('../figures/gridspec.png',dpi=64)
plt.show()
