import matplotlib.pyplot as plt
import numpy as np
import sys


def get_delay(file_name):
    #f = open("a_delete.txt", 'r')
    f = open( file_name, 'r')

    y = []
    x = []
    firstTime = True
    firstVal = 0

    for line in f:
        if line.startswith("UDP Packet with"):
            try:
                s = line.split()[26];

                n = float(s) 
                y.append(n)
                s = line.split()[30];
                if firstTime == True:
                    firstTime = False
                    firstVal = float(s)
                n = float(s) - firstVal   
                x.append(n)
            except Exception:
                pass
    print 'Mean value = ' + str(np.mean(y))
    return (y,x)


file_name = sys.argv[1] 
if not file_name:
    print 'Usage of this script, please add an argv to the script'
    sys.exit()



(y1,x1) = get_delay(file_name + '/a_10.txt')
(y2,x2) = get_delay(file_name + '/a_20.txt')
(y3,x3) = get_delay(file_name + '/a_30.txt')
(y4,x4) = get_delay(file_name + '/a_40.txt')
(y5,x5) = get_delay(file_name + '/a_50.txt')
(y6,x6) = get_delay(file_name + '/a_60.txt')
(y7,x7) = get_delay(file_name + '/a_70.txt')


(fig, ax) = plt.subplots(1,7)

ax[0].plot(x1, y1)
ax[1].plot(x2, y2)
ax[2].plot(x3, y3)
ax[3].plot(x4, y4)
ax[4].plot(x5, y5)
ax[5].plot(x6, y6)
ax[6].plot(x7, y7)


#ax.set_title("DRB ping+iperf flow")    
#ax.set_xlabel('Microseconds')
#ax.set_ylabel('Queue size')

plt.show()

