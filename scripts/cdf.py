import numpy as np
from pylab import *

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
file_name_2 = sys.argv[2] 
if not file_name or not file_name_2:
    print 'Usage of this script, please add 2 argv to the script'
    sys.exit()


(y1,x1) = get_delay(file_name + '/a_10.txt')
(y2,x2) = get_delay(file_name + '/a_20.txt')
(y3,x3) = get_delay(file_name + '/a_30.txt')
(y4,x4) = get_delay(file_name + '/a_40.txt')
(y5,x5) = get_delay(file_name + '/a_50.txt')
(y6,x6) = get_delay(file_name + '/a_60.txt')
(y7,x7) = get_delay(file_name + '/a_70.txt')

(y21,x21) = get_delay(file_name_2 + '/a_10.txt')
(y22,x22) = get_delay(file_name_2 + '/a_20.txt')
(y23,x23) = get_delay(file_name_2 + '/a_30.txt')
(y24,x24) = get_delay(file_name_2 + '/a_40.txt')
(y25,x25) = get_delay(file_name_2 + '/a_50.txt')
(y26,x26) = get_delay(file_name_2 + '/a_60.txt')
(y27,x27) = get_delay(file_name_2 + '/a_70.txt')





(fig, ax) = plt.subplots(1,1)


x = np.sort(y1)
y = np.arange(len(x))/float(len(x))

#plt.plot(x, y)
x_2 = np.sort(y21)
y_2 = np.arange(len(x_2))/float(len(x_2))


ax.plot(x,y)
ax.plot(x_2,y_2)

#ax.hist(y1, len(y1) , density=True, histtype='step', cumulative=True, label='Empirical')
#ax.hist(y21, len(y21) , density=True, histtype='step', cumulative=True, label='Empirical')

ax.set_ylim(0,1)

#print y1


#sum_val = 0
#for val in y1:
#    sum_val += val

#newList = [y / sum_val for y in y1]


# Compute the CDF
#CY = np.cumsum(newList)


#ax.plot(y1, CY)
#ax[1].plot(x2, y2)
#ax[2].plot(x3, y3)
#ax[3].plot(x4, y4)
#ax[4].plot(x5, y5)
#ax[5].plot(x6, y6)
#ax[6].plot(x7, y7)

#ax.set_title("DRB ping+iperf flow")    
#ax.set_xlabel('Microseconds')
#ax.set_ylabel('Queue size')

#plt.gca().invert_yaxis()
plt.show()


