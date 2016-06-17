#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
from  pylab import *
class picture_1:
    x = np.linspace(-np.pi,np.pi,256,endpoint=True)
    C,S = np.cos(x),np.sin(x)
    plt.plot(x,C)
    plt.plot(x,S)
    plt.show
