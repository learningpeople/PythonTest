import numpy as np
import matplotlib.pyplot as plt

n = 256
X = np.linspace(-np.pi,np.pi,endpoint=True)
Y = np.sin(2*X)

plt.axes([0.025,0.025,0.5,0.95])

plt.plot(X,Y+1,color='blue',alpha=1.0)
plt.fill_between(X,Y+1,color='r',alpha=.25)

plt.plot(X,Y-1,color='blue',alpha=1.0)
plt.fill_between(X,-1,Y-1,(Y-1)>-1,color='g',alpha=.5)
plt.fill_between(X,-1,Y-1,(Y-1)<-1,color='y',alpha=.5)

plt.xlim(-np.pi,np.pi),plt.xticks([])
plt.ylim(-2.5,2.5),plt.yticks([])

#plt.savefig('../figures/plot_ex.png'dpi=48)
plt.show()
