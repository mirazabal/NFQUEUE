import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np

def get_MAC_utilization(str1):
    counter = 0
    total_num_used_res = 0
    total_num_res = 0
    print 'value of str1 = ' + str1
    f = open(str1, 'r')
    y = []
    y2 = []
    x = []
    firstTime = True
    firstVal = 0
    for line in f:
        if line.startswith('Packets deque at MAC'):
            try:
                s = line.split()[17];
                if firstTime == True:
                    firstTime = False
                    firstVal = float(s)
                n1 = float(s) - firstVal  

                s = line.split()[6];
                n2 = float(s) 
                if n1 > 2500000 and n1 < 32500000:
                    x.append(n1)
                    y.append(n2)
                    y2.append( int(line.split()[13]))
                    counter += 1
                    total_num_used_res += n2
                    total_num_res += int(line.split()[13])
            except Exception:
                pass

    print 'Total number of resources = ' + str(total_num_res) + 'total number of resources used ' +  str( total_num_used_res ) + ' in thsi times = '  + str(counter)
    return (y,y2,x)


def generate_utilization_mean(str1):

    val1 = get_MAC_utilization(str1 + '/a_10.txt') 
#    val2 = get_MAC_utilization(str1 + '/a_20.txt') 
#    val3 = get_MAC_utilization(str1 + '/a_30.txt') 
#    val4 = get_MAC_utilization(str1 + '/a_40.txt') 
#    val5 = get_MAC_utilization(str1 + '/a_50.txt') 
#    val6 = get_MAC_utilization(str1 + '/a_60.txt') 
#    val7 = get_MAC_utilization(str1 + '/a_70.txt') 

#    return  (val1,val2,val3, val4, val5, val6, val7) 
    return val1


#x10 = generate_utilization_mean('6th')
#x11 = generate_utilization_mean('7th')
#x12 = generate_utilization_mean('8th')
#x13 = generate_utilization_mean('9th')
(y,y2,x) = generate_utilization_mean('10th')

(fig, ax) = plt.subplots(1,1)

ax.plot( x , y, color = 'red')
ax.plot( x, y2, color = 'blue')


ax.set_xlabel('time in usec')
#ax[1].set_xlabel('Utilization %')

ax.set_ylabel('Number of packets')

ax.legend(( 'Small+DQL' , 'Channel max. cap.' ), loc='upper right')

plt.show()


