import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np

def get_values(str1):
    f = open(str1, 'r')
    y = []
    x = []
    firstTime = True
    firstVal = 0
    codelDropped = 0
    for line in f:
        if line.startswith("UDP Packet with"):
            try:
                s = line.split()[30];
                if firstTime == True:
                    firstTime = False
                    firstVal = float(s)
                n1 = float(s) - firstVal   
                s = line.split()[41];

                if n1 > 2500000 and n1 < 32500000 :
                    x.append(n1)
                    y.append(float(s))
            except Exception:
                pass
    print 'Mean value for ' + str1 + ' ' + str(np.mean(y))
    return (np.mean(y), np.std(y))

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

    return (arr_ping_mean,arr_ping_std)

def get_delay(str1):
    f = open(str1, 'r')

    y = []
    x = []
    firstTime = True
    firstVal = 0

    for line in f:
        if line.startswith("UDP Packet with"):
            try:
                s = line.split()[6]
                if s != 'inserted':
                    try:
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
                    except Exception:
                        pass
            except Exception:
                pass
    print 'Mean value for ' + str1 + ' ' + str(np.mean(y))
    return (np.mean(y), np.std(y))


def get_delay_values(str1):
    val =  get_delay(str1)
    return val

def generate_delay_mean(str1):

    arr_ping_mean = []
    arr_ping_std = []

    val = get_delay_values(str1 + '/a_10.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])

    val = get_delay_values(str1 + '/a_20.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])


    val = get_delay_values(str1 + '/a_30.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])

    val = get_delay_values(str1 + '/a_40.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])

    val = get_delay_values(str1 + '/a_50.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])

    val = get_delay_values(str1 + '/a_60.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])

    val = get_delay_values(str1 + '/a_70.txt') 
    arr_ping_mean.append(val[0])
    arr_ping_std.append(val[1])

    return (arr_ping_mean, arr_ping_std)


(y1d,e1d) = generate_delay_mean('1st')
(y5d,e5d) = generate_delay_mean('3th')
(y10d,e10d) = generate_delay_mean('6th')
(y11d,e11d) = generate_delay_mean('7th')

(y14d,e14d) = generate_delay_mean('10th')
(y15d,e15d) = generate_delay_mean('11th')
(y16d,e16d) = generate_delay_mean('12th')


(y1,e1) = generate_mean('1st')
(y5,e5) = generate_mean('3th')
(y10,e10) = generate_mean('6th')
(y11,e11) = generate_mean('7th')
#(y12,e12) = generate_mean('8th')
#(y13,e13) = generate_mean('9th')
(y14,e14) = generate_mean('10th')
(y15,e15) = generate_mean('11th')
(y16,e16) = generate_mean('12th')

x = [10,20,30,40,50,60,70] 



(fig, ax) = plt.subplots(1,2)
ax[0].plot(x, y1d, 'd-' , linewidth = 3.0 , markersize=15, color='cyan')
ax[0].plot(x, y5d, 'd-' , linewidth = 3.0 , markersize=15, color='blue')
ax[0].plot(x, y10d, 'd-' , linewidth = 3.0 , markersize=15, color='red')
ax[0].plot(x, y11d, 'd-' , linewidth = 3.0 , markersize=15, color='green')
#ax[0].plot(x, y12d, 'd-' , linewidth = 3.0 , markersize=15, color='blue')
#ax[0].plot(x, y13d, 'd-' , linewidth = 3.0 , markersize=15, color='orange')
ax[0].plot(x, y14d, 'd-' , linewidth = 3.0 , markersize=15, color='purple')
ax[0].plot(x, y15d, 'd-' , linewidth = 3.0 , markersize=15, color='yellow')
ax[0].plot(x, y16d, 'd-' , linewidth = 3.0 , markersize=15, color='pink')


ax[1].plot(x, y1, 'd-' , linewidth = 3.0 , markersize=15, color='cyan')
ax[1].plot(x, y5, 'd-' , linewidth = 3.0 , markersize=15, color='blue')
ax[1].plot(x, y10, 'd-' , linewidth = 3.0 , markersize=15, color='red')
ax[1].plot(x, y11, 'd-' , linewidth = 3.0 , markersize=15, color='green')
#ax[1].plot(x, y12, 'd-' , linewidth = 3.0 , markersize=15, color='blue')
#ax[1].plot(x, y13, 'd-' , linewidth = 3.0 , markersize=15, color='orange')
ax[1].plot(x, y14, 'd-' , linewidth = 3.0 , markersize=15, color='purple')
ax[1].plot(x, y15, 'd-' , linewidth = 3.0 , markersize=15, color='yellow')
ax[1].plot(x, y16, 'd-' , linewidth = 3.0 , markersize=15, color='pink')
#ax[1].errorbar(x, y12, e12)

#ax.set_title("DRB ping+iperf flow")    

ax[0].set_xlabel('Ping interval [ms]')
ax[1].set_xlabel('Ping interval [ms]')

ax[0].set_ylabel('Delay in ms')
#ax[0].set_ylim(0,350)

ax[1].set_ylabel('Queue size average')
#ax[1].set_ylim(0,350)

#major_ticks = np.arange(0, 350, 5)
#ax[0].set_yticks(major_ticks)
#ax[1].set_yticks(major_ticks)

#ax[0].legend(('Default', 'DRB@10', 'DRB@20', 'DRB@30' ), loc = 'right')
#ax[1].legend(('CoDel@DRB', 'CoDel@QFI_DRB@10', 'CoDel@QFI_DRB@20', 'CoDel@QFI_DRB@30', 'CoDel@UPF_limit', 'CoDel@QFI_DRB@DQL', 'BBR', 'Theoretical max.' ), loc='upper right')

#ax[0].legend(('CoDel@QFI_DRB@DQL', 'BBR', 'Theoretical max.', 'Small@DRB10', 'Small+DQL' ), loc='upper right')

ax[0].legend(('Vanilla', 'CoDel' ,'DQL', 'BBR', 'DynRLC', 'CQI_PACER', 'CQI_PACER_ASYNC' ), loc = 'left')
ax[1].legend(('Vanilla',  'CoDel' ,'DQL', 'BBR', 'DynRLC', 'CQI_PACER', 'CQI_PACER_ASYNC' ), loc = 'left')

plt.rcParams.update({'font.size': 23})

ax[0].grid()
ax[1].grid()

plt.show()

