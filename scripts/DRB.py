import matplotlib.pyplot as plt
import numpy as np

f = open("7th/a_40.txt", 'r')
y = []
x = []
firstTime = True
firstVal = 0
for line in f:
    if line.startswith("DRB queue idx = 0"):
        s = line.split()[8];
        
        n = float(s) 
        y.append(n)
        s = line.split()[12];
        if firstTime == True:
           firstTime = False
           firstVal = float(s)
        n = float(s) - firstVal   
        x.append(n)

(fig, ax) = plt.subplots(1,1)
ax.plot(x, y)
ax.set_title("DRB ping+iperf flow")    
ax.set_xlabel('Microseconds')
ax.set_ylabel('Queue size')

plt.show()

