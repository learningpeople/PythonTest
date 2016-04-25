#!/usr/bin/python
u"#coding = utf-8"
import PIL
import numpy as np
import pylab as pl
class picture_01_lineplots:
    """"""
    
x = [1,2,3,4]
y1 = map(lambda a:a*a,x)
y2 = map(lambda b:b*2,x)
print y1,y2

plot1=pl.plot(x,y1,'r',label="red line",linewidth=4)
#pl.plot(x,y2,'g')
plot2=pl.plot(x,y2,'go',label='green circles')

pl.title("Plot of y vs.x")
pl.xlabel('x axis')
pl.ylabel('y axis')

pl.xlim(0.0,7.0)
pl.ylim(0.0,30)

#pl.legend([plot1,plot2], ('red line','green circles'), 'best', numpoints=1
pl.legend(loc='upper left')


pl.show()


