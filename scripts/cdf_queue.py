import numpy as np
from pylab import *
'''
def get_size(str1):
    f = open(str1, 'r')
    y = []
    x = []
    firstTime = True
    firstVal = 0
    for line in f:
        try:
            if line.startswith("UDP Packet with id ="):
                line_splitted = line.split(' ')
                print str(line_splitted.size())
                if line_splitted.size() < 42:
                    continue
                s = line.split()[30]
                if firstTime == True:
                    firstTime = False
                    firstVal = float(s)
                n1 = float(s) - firstVal   

                s = line.split()[43];
                print 'Total num packets = ' + s
                n2 = float(s) 
                if n1 > 2500000 and n1 < 32500000:
                    y.append(n2)
                    x.append(n1)
        except Exception:
            print 'very bad style...'
    print 'Mean value = ' + str(np.mean(y))
    return (y, x)
'''

def get_size(file_name):
    f = open( file_name, 'r')

    y = []
    x = []
    firstTime = True
    firstVal = 0

    for line in f:
        if line.startswith("UDP Packet with"):
            try:
                s = line.split()[41];
                n1 = float(s) 
                s = line.split()[30];
                if firstTime == True:
                    firstTime = False
                    firstVal = float(s)
                n2 = float(s) - firstVal   
                if n2 > 2500000 and n1 < 32500000:
                    y.append(n1)
                    x.append(n2)

            except Exception:
                pass
    print 'Mean value = ' + str(np.mean(y))
    return (y,x)





def get_avg_size(file_name):
    (y1,x1) = get_size(file_name + '/a_10.txt')
    (y2,x2) = get_size(file_name + '/a_20.txt')
    (y3,x3) = get_size(file_name + '/a_30.txt')
    (y4,x4) = get_size(file_name + '/a_40.txt')
    (y5,x5) = get_size(file_name + '/a_50.txt')
    (y6,x6) = get_size(file_name + '/a_60.txt')
    (y7,x7) = get_size(file_name + '/a_70.txt')
    return ((y1+y2+y3+y4+y5+y6+y7), (x1+x2+x3+x4+x5+x6+x7))

(y6,x6) = get_avg_size('6th')
(y7,x7) = get_avg_size('7th')
#(y8,x8) = get_avg_size('8th')
#(y9,x9) = get_avg_size('9th')
(y10,x10) = get_avg_size('10th')
(y11,x11) = get_avg_size('11th')
(y12,x12) = get_avg_size('12th')



(fig, ax) = plt.subplots(1,1)


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



ax.plot(y6_sort,y6_cdf , color = 'red')
ax.plot(y7_sort,y7_cdf, color = 'green' )
#ax.plot(y8_sort,y8_cdf, color = 'blue' )
#ax.plot(y9_sort,y9_cdf , color = 'orange')
ax.plot(y10_sort,y10_cdf, color = 'purple')
ax.plot(y11_sort,y11_cdf, color = 'yellow')
ax.plot(y12_sort,y12_cdf, color = 'pink')


ax.set_ylim(0,1)
ax.set_xlim(0)

ax.legend(('DQL', 'BBR', 'DynRLC', 'CQI_PACER',  'CQI_PACER_ASYNC'), loc='upper right')


# Compute the CDF
#CY = np.cumsum(newList)

ax.set_title("Queue size CDF")    
ax.set_xlabel('Queue size')
ax.set_ylabel('CDF')

#plt.gca().invert_yaxis()
plt.show()


