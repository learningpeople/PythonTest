#!/usr/bin/python

import numpy as np
import pylab as pl

data = np.random.normal(5.0,3.0,1000)
class p:
#pl.hist(data)
    pl.hist(data,histtype='stepfilled')
    pl.xlabel("data")
    pl.show()
