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
            s = line.split()[6]
            if s != 'inserted':
                s = line.split()[30];
                if firstTime == True:
                    firstTime = False
                    firstVal = float(s)
                n1 = float(s) - firstVal   
                x.append(n1)
                s = line.split()[26];
                n = float(s) 
                if n1 > 2500000 and n1 < 32500000 :
                    y.append(n)
                print 'Mean value for ' + str1 + ' ' + str(np.mean(y))
    return (np.mean(y), np.std(y))


def get_values(str1):
    val =  get_delay(str1)
    return val

def generate_mean(str1):

    arr_ping_mean = []
    arr_ping_std = []

    val = get_values(str1 + '/a_10.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])

    val = get_values(str1 + '/a_20.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])


    val = get_values(str1 + '/a_30.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])

    val = get_values(str1 + '/a_40.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])

    val = get_values(str1 + '/a_50.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])

    val = get_values(str1 + '/a_60.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])

    val = get_values(str1 + '/a_70.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])

    return (arr_ping_mean, arr_ping_std)

'''
(y1,e1) = generate_mean('1st')
(y2,e2) = generate_mean('2nd_10')
(y3,e3) = generate_mean('2nd_20')
(y4,e4) = generate_mean('2nd_30')
(y5,e5) = generate_mean('3rd')
(y6,e6) = generate_mean('4th_10')
(y7,e7) = generate_mean('4th_20')
(y8,e8) = generate_mean('4th_30')
'''

(y10,e10) = generate_mean('6th')
(y11,e11) = generate_mean('7th')
(y12,e12) = generate_mean('8th')
(y13,e13) = generate_mean('9th')
(y14,e14) = generate_mean('10th')

x = [10,20,30,40,50,60,70] 


(fig, ax) = plt.subplots(1,2)
'''
ax[0].plot(x, y1, 'o-' , linewidth = 3.0, markersize=15, color='red')
#ax[0].errorbar(x, y1, e1)
ax[0].plot(x, y2, 'v-' , linewidth = 3.0 , markersize=15, color='green')
ax[0].plot(x, y3, '^-' , linewidth = 3.0 , markersize=15, color='blue')
ax[0].plot(x, y4, 's-' , linewidth = 3.0 , markersize=15, color='cyan')
ax[1].plot(x, y5, 'p-' , linewidth = 3.0 , markersize=15, color ='yellowgreen')
ax[1].plot(x, y6, '*-' , linewidth = 3.0 , markersize=15, color='sienna')
ax[1].plot(x, y7, 'h-' , linewidth = 3.0 , markersize=15, color='orange')
ax[1].plot(x, y8, 'd-' , linewidth = 3.0 , markersize=15, color='purple')
'''
ax[1].plot(x, y10, 'd-' , linewidth = 3.0 , markersize=15, color='red')
ax[1].plot(x, y11, 'd-' , linewidth = 3.0 , markersize=15, color='green')
ax[1].plot(x, y12, 'd-' , linewidth = 3.0 , markersize=15, color='blue')
ax[1].plot(x, y13, 'd-' , linewidth = 3.0 , markersize=15, color='orange')
ax[1].plot(x, y14, 'd-' , linewidth = 3.0 , markersize=15, color='purple')
#ax[1].errorbar(x, y12, e12)
#ax.set_title("DRB ping+iperf flow")    


ax[0].set_xlabel('Ping interval [ms]')
ax[1].set_xlabel('Ping interval [ms]')

ax[0].set_ylabel('Delay [ms]')
#ax[0].set_ylim(0,350)

ax[1].set_ylabel('Delay [ms]')
#ax[1].set_ylim(0,350)

#major_ticks = np.arange(0, 350, 2)
#ax[0].set_yticks(major_ticks)
#ax[1].set_yticks(major_ticks)


ax[0].legend(('Default', 'DRB@10', 'DRB@20', 'DRB@30' ), loc = 'right')
#ax[1].legend(('CoDel@DRB', 'CoDel@QFI_DRB@10', 'CoDel@QFI_DRB@20', 'CoDel@QFI_DRB@30', 'CoDel@UPF_limit', 'CoDel@QFI_DRB@DQL', 'BBR', 'Theoretical max.' ), loc='upper right')
ax[1].legend(('CoDel@QFI_DRB@DQL', 'BBR', 'Theoretical max.', 'Samll@DRB1'  , 'Small@DQL' ), loc='upper right')

plt.rcParams.update({'font.size': 23})

ax[0].grid()
ax[1].grid()


plt.show()


