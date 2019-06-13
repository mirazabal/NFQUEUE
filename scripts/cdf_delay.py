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


#file_name = sys.argv[1] 
#file_name_2 = sys.argv[2] 
#if not file_name or not file_name_2:
#    print 'Usage of this script, please add 2 argv to the script'
#    sys.exit()



def get_avg_delay(file_name):
    (y1,x1) = get_delay(file_name + '/a_10.txt')
    (y2,x2) = get_delay(file_name + '/a_20.txt')
    (y3,x3) = get_delay(file_name + '/a_30.txt')
    (y4,x4) = get_delay(file_name + '/a_40.txt')
    (y5,x5) = get_delay(file_name + '/a_50.txt')
    (y6,x6) = get_delay(file_name + '/a_60.txt')
    (y7,x7) = get_delay(file_name + '/a_70.txt')
    return ((y1+y2+y3+y4+y5+y6+y7), (x1+x2+x3+x4+x5+x6+x7))


(y1,x1) = get_avg_delay('1st')
(y3,x3) = get_avg_delay('3th')
(y6,x6) = get_avg_delay('6th')
(y7,x7) = get_avg_delay('7th')
#(y8,x8) = get_avg_delay('8th')
#(y9,x9) = get_avg_delay('9th')
(y10,x10) = get_avg_delay('10th')
(y11,x11) = get_avg_delay('11th')
(y12,x12) = get_avg_delay('12th')



(fig, ax) = plt.subplots(1,1)

y1_sort = np.sort(y1)
y1_cdf = np.arange(len(y1_sort))/float(len(y1_sort))


y3_sort = np.sort(y3)
y3_cdf = np.arange(len(y3_sort))/float(len(y3_sort))


y6_sort = np.sort(y6)
y6_cdf = np.arange(len(y6_sort))/float(len(y6_sort))


y7_sort = np.sort(y7)
y7_cdf = np.arange(len(y7_sort))/float(len(y7_sort))


#y8_sort = np.sort(y8)
#y8_cdf = np.arange(len(y8_sort))/float(len(y8_sort))


#y9_sort = np.sort(y9)
#y9_cdf = np.arange(len(y9_sort))/float(len(y9_sort))


y10_sort = np.sort(y10)
y10_cdf = np.arange(len(y10_sort))/float(len(y10_sort))

y11_sort = np.sort(y11)
y11_cdf = np.arange(len(y11_sort))/float(len(y11_sort))

y12_sort = np.sort(y12)
y12_cdf = np.arange(len(y12_sort))/float(len(y12_sort))


#plt.plot(x, y)
#x_2 = np.sort(y21)
#y_2 = np.arange(len(x_2))/float(len(x_2))


ax.plot(y1_sort,y1_cdf , color = 'cyan')
ax.plot(y3_sort,y3_cdf , color = 'blue')
ax.plot(y6_sort,y6_cdf , color = 'red')
ax.plot(y7_sort,y7_cdf, color = 'green' )
#ax.plot(y8_sort,y8_cdf, color = 'blue' )
#ax.plot(y9_sort,y9_cdf , color = 'orange')
ax.plot(y10_sort,y10_cdf, color = 'purple')
ax.plot(y11_sort,y11_cdf, color = 'yellow')
ax.plot(y12_sort,y12_cdf, color = 'pink')

#ax.plot(x_2,y_2)

#ax.hist(y1, len(y1) , density=True, histtype='step', cumulative=True, label='Empirical')
#ax.hist(y21, len(y21) , density=True, histtype='step', cumulative=True, label='Empirical')

ax.set_ylim(0,1)
ax.set_xlim(0)

ax.legend(('Vanilla', 'CoDel','DQL', 'BBR', 'DynRLC', 'CQI_PACER',  'CQI_PACER_ASYNC'), loc='upper right')

ax.set_title("Queue delay CDF")    
ax.set_xlabel('Delay in ms')
ax.set_ylabel('CDF')

plt.show()


