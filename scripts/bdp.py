import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np

def get_delay(str1):
    f = open(str1, 'r')

    y = []
    x = []
    firstTime = True
    firstVal = 0

    for line in f:
        if line.startswith("UDP Packet with"):
            try:

                s = line.split()[30];
                if firstTime == True:
                 firstTime = False
                 firstVal = float(s)
                n1 = float(s) - firstVal   
                x.append(n1)

                s = line.split()[26];
                n2 = float(s) 

                if n1 > 2500000 and n1 < 32500000:
                    y.append(n2)
            except Exception:
               pass

    print 'Mean value for ' + str1 + ' ' + str(np.mean(y))
    return (np.mean(y), np.std(y))

def get_values(str1):
    f = open(str1, 'r')
    y = []
    x = []
    firstTime = True
    firstVal = 0

    for line in f:
        if line.startswith("UDP Packet with"):
            s = line.split()[6]
            if s == 'inserted':
                s = line.split()[18]
                if firstTime == True:
                     firstTime = False
                firstVal = float(s)
                n = float(s) - firstVal   
                x.append(n)

                s = line.split()[14];
                n1 = float(s) 

                y.append(n1)

    
    print 'Mean value = ' + str(np.mean(y))
    return (np.mean(y), np.std(y) )

def generate_ping_mean(str1):

    arr_ping_mean = []
    arr_ping_std = []

    RTT_min = 10
    val1 = get_delay(str1 + '/a_10.txt') 
    val2 = get_delay(str1 + '/a_20.txt') 
    val3 = get_delay(str1 + '/a_30.txt') 
    val4 = get_delay(str1 + '/a_40.txt') 
    val5 = get_delay(str1 + '/a_50.txt') 
    val6 = get_delay(str1 + '/a_60.txt') 
    val7 = get_delay(str1 + '/a_70.txt') 

#    arr_ping_mean.append(val[0])
#    arr_ping_std.append(val[1])
#    return arr_ping_mean

    return  (val1[0],val2[0],val3[0], val4[0], val5[0], val6[0], val7[0]) 

def get_MAC_utilization(str1):
    counter = 0
    total_num_used_res = 0
    total_num_res = 0
    print 'value of str1 = ' + str1
    f = open(str1, 'r')
    y = []
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
                x.append(n1)

                s = line.split()[6];
                n2 = float(s) 
                if n1 > 2500000 and n1 < 32500000:
                    y.append(n2)
                    counter += 1
                    total_num_used_res += n2
                    total_num_res += int(line.split()[13])
            except Exception:
                pass

    print 'Total number of resources = ' + str(total_num_res) + 'total number of resources used ' +  str( total_num_used_res ) + ' in thsi times = '  + str(counter)
    return  total_num_used_res/ total_num_res 


def generate_utilization_mean(str1):

    arr_ping_mean = []
    arr_ping_std = []

    RTT_min = 10
    val1 = get_MAC_utilization(str1 + '/a_10.txt') 
    val2 = get_MAC_utilization(str1 + '/a_20.txt') 
    val3 = get_MAC_utilization(str1 + '/a_30.txt') 
    val4 = get_MAC_utilization(str1 + '/a_40.txt') 
    val5 = get_MAC_utilization(str1 + '/a_50.txt') 
    val6 = get_MAC_utilization(str1 + '/a_60.txt') 
    val7 = get_MAC_utilization(str1 + '/a_70.txt') 

#    arr_ping_mean.append(val[0])
#    arr_ping_std.append(val[1])
#    return arr_ping_mean

    return  (val1,val2,val3, val4, val5, val6, val7) 




y1 = generate_ping_mean('1st') 
y5 = generate_ping_mean('3th')

y10 = generate_ping_mean('6th')
y11 = generate_ping_mean('7th')
y12 = generate_ping_mean('8th')
y13 = generate_ping_mean('9th')
y14 = generate_ping_mean('10th')
y15 = generate_ping_mean('11th')
y16 = generate_ping_mean('12th')


x1 = generate_utilization_mean('1st')
x5 = generate_utilization_mean('3th')

x10 = generate_utilization_mean('6th')
x11 = generate_utilization_mean('7th')

x14 = generate_utilization_mean('10th')
x15 = generate_utilization_mean('11th')
x16 = generate_utilization_mean('12th')


(fig, ax) = plt.subplots(1,7)


for x in range(0, 7):
    ax[x].plot(x1[x], y1[x], 'd-' , linewidth = 3.0 , markersize=15, color='cyan')
    ax[x].plot(x5[x], y5[x], 'd-' , linewidth = 3.0 , markersize=15, color='blue')
    ax[x].plot(x10[x], y10[x], 'd-' , linewidth = 3.0 , markersize=15, color='red')
    ax[x].plot(x11[x], y11[x], 'd-' , linewidth = 3.0 , markersize=15, color='green')
   # ax[x].plot(x12[x], y12[x], 'd-' , linewidth = 3.0 , markersize=15, color='blue')
   # ax[x].plot(x13[x], y13[x], 'd-' , linewidth = 3.0 , markersize=15, color='orange')
    ax[x].plot(x14[x], y14[x], 'd-' , linewidth = 3.0 , markersize=15, color='purple')
    ax[x].plot(x15[x], y15[x], 'd-' , linewidth = 3.0 , markersize=15, color='yellow')
    ax[x].plot(x16[x], y16[x], 'd-' , linewidth = 3.0 , markersize=15, color='pink')

ax[0].legend(('Vanilla', 'CoDel', 'DQL', 'BBR', 'DynRLC', 'CQI_PACER', 'CQI_PACER_ASYNC' ), loc = 'left')
#ax[1].legend(('CoDel@DRB', 'CoDel@QFI_DRB@10', 'CoDel@QFI_DRB@20', 'CoDel@QFI_DRB@30', 'CoDel@UPF_limit', 'CoDel@QFI_DRB@DQL', 'BBR', 'Max.' ), loc='upper left')
#ax[0].legend(('CQI'), loc='upper right')
plt.rcParams.update({'font.size': 9})

ax[0].set_xlabel('Utilization %')
#ax[1].set_xlabel('Utilization %')

ax[0].set_ylabel('Delay in ms')

#ax[1].set_ylabel('Normalized Delay')
#ax[1].set_ylim(8,15)

for x in range (0,7):
#    ax[x].set_ylim(5000,50000)
    ax[x].set_xlim(0.90,1.0)
#    ax[x].grid()
    #ax[1].grid()

plt.show()

