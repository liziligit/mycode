# -*- coding:UTF-8 -*-
import matplotlib as mpl
mpl.use('TkAgg')
import matplotlib.pyplot as plt
import random,time
import threading
import numpy as np
from matplotlib.animation import FuncAnimation

def write_to_txt():
    
    with open('test.txt','a') as f:
        while True:
            f.write('%s %f \n' % (time.ctime().split()[-2],random.random()))
            f.flush()
            time.sleep(0.5+random.random())

def read_txt():
    
    with open('test.txt') as f:
#        lines=f.readlines()
        f.seek(0,2)
        cnt=0
        while True:
            lines = f.readlines()
            if lines:
                try:
                    lines=[float(l.split()[-1]) for l in lines]
                except:
                    continue
                for line in lines:
                    yield cnt,line
                    cnt += 0.1
            time.sleep(1)


#写线程
write_thread = threading.Thread(target=write_to_txt)
write_thread.start()
#读线程
#read_thread =threading.Thread(target=read_txt,args=(data,))
#read_thread.start()


def init():
    ax.set_ylim(-5, 2)
    ax.set_xlim(0, 3)
    del xdata[:]
    del ydata[:]
    line.set_data(xdata, ydata)
    return line,

fig, ax = plt.subplots()
line, = ax.plot([], [],'ro-')
ax.grid()
plt.xticks([])
xdata, ydata = [], []


def run(d):
    # update the data
    t, y = d
    xdata.append(t)
    ydata.append(y)
    if len(xdata)>30:        
        del xdata[:-30]
        del ydata[:-30]
        xmin, xmax = ax.get_xlim()
        ax.set_xlim(xmin+0.1,xmax+0.1)
        
    line.set_data(xdata, ydata)

    return line,

ani = FuncAnimation(fig, run, read_txt, blit=False, interval=20,
                              repeat=False, init_func=init)
plt.show()

