import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np

'''
#def get_mean_and_var(str1,str2):
#    f = open(str1, 'r')
#    y = []
#    x = []
#    firstTime = True
#    firstVal = 0
#    for line in f:
#        if line.startswith(str2):
#            s = line.split()[12];
#            if firstTime == True:
#                firstTime = False
#                firstVal = float(s)
#            n1 = float(s) - firstVal  
#            if n1 > 40000000:
#                break
#            x.append(n1)

            s = line.split()[8];
            n = float(s) 
            if n ==0:
                continue
            if n1 > 2500000 and n1 < 32500000:
                y.append(n)
            
    return (np.mean(y),np.std(y))

def get_DRB_QFI_UPF(str1):
    arr = get_mean_and_var(str1,'DRB')
    arr1 = get_mean_and_var(str1,'QFI')
    arr2 = get_mean_and_var(str1,'UPF')
    return (arr,arr1,arr2)
'''

def get_values(str1):
    f = open(str1, 'r')
    y = []
    x = []
    firstTime = True
    firstVal = 0
    codelDropped = 0
    for line in f:

#        if line.startswith(" dropping packet"):
#            codelDropped = codelDropped + 1

        if line.startswith("UDP Packet with"):
            s = line.split()[6]
            if s == 'inserted':
                try:
                    s = line.split()[18]
                    if firstTime == True:
                        firstTime = False
                        firstVal = float(s)
                    n1 = float(s) - firstVal   
                    x.append(n1)

                    s = line.split()[14];
                    n2 = float(s) 
                    if n1 > 2500000 and n1 < 32500000:
                        y.append(n2 - codelDropped)
    
                except Exception:
                    pass
    print 'Mean value = ' + str(np.mean(y))
    return (np.mean(y), np.std(y) )


#    arr_UPF_mean = []
#    arr_UPF_std = []
#    arr_QFI_mean = []
#    arr_QFI_std = []
#    arr_DRB_mean = []
#    arr_DRB_std = []

#    val = get_DRB_QFI_UPF(str1)

#    arr_DRB_mean.append(val[0][0])
#    arr_DRB_std.append(val[0][1])

#    arr_QFI_mean.append(val[1][0])
#    arr_QFI_std.append(val[1][1])

#    arr_UPF_mean.append(val[2][0])
#    arr_UPF_std.append(val[2][1])
#    return (val[0][0] + val[1][0], val[0][1] + val[1][1])






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

(y10d,e10d) = generate_delay_mean('6th')
(y11d,e11d) = generate_delay_mean('7th')
(y12d,e12d) = generate_delay_mean('8th')
(y13d,e13d) = generate_delay_mean('9th')
(y14d,e14d) = generate_delay_mean('10th')




(y10,e10) = generate_mean('6th')
(y11,e11) = generate_mean('7th')
(y12,e12) = generate_mean('8th')
(y13,e13) = generate_mean('9th')
(y14,e14) = generate_mean('10th')

x = [10,20,30,40,50,60,70] 



(fig, ax) = plt.subplots(1,2)
'''
ax[0].plot(x, y1, 'o-' , linewidth = 3.0, markersize=15, color='red')
ax[0].plot(x, y2, 'v-' , linewidth = 3.0 , markersize=15, color='green')
ax[0].plot(x, y3, '^-' , linewidth = 3.0 , markersize=15, color='blue')
ax[0].plot(x, y4, 's-' , linewidth = 3.0 , markersize=15, color='cyan')
ax[1].plot(x, y5, 'p-' , linewidth = 3.0 , markersize=15, color ='yellowgreen')
ax[1].plot(x, y6, '*-' , linewidth = 3.0 , markersize=15, color='sienna')
ax[1].plot(x, y7, 'h-' , linewidth = 3.0 , markersize=15, color='orange')
ax[1].plot(x, y8, 'd-' , linewidth = 3.0 , markersize=15, color='purple')
'''
ax[0].plot(x, y10d, 'd-' , linewidth = 3.0 , markersize=15, color='red')
ax[0].plot(x, y11d, 'd-' , linewidth = 3.0 , markersize=15, color='green')
ax[0].plot(x, y12d, 'd-' , linewidth = 3.0 , markersize=15, color='blue')
ax[0].plot(x, y13d, 'd-' , linewidth = 3.0 , markersize=15, color='orange')
ax[0].plot(x, y14d, 'd-' , linewidth = 3.0 , markersize=15, color='purple')


ax[1].plot(x, y10, 'd-' , linewidth = 3.0 , markersize=15, color='red')
ax[1].plot(x, y11, 'd-' , linewidth = 3.0 , markersize=15, color='green')
ax[1].plot(x, y12, 'd-' , linewidth = 3.0 , markersize=15, color='blue')
ax[1].plot(x, y13, 'd-' , linewidth = 3.0 , markersize=15, color='orange')
ax[1].plot(x, y14, 'd-' , linewidth = 3.0 , markersize=15, color='purple')
ax[1].errorbar(x, y12, e12)

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

ax[0].legend(('CoDel@QFI_DRB@DQL', 'BBR', 'Theoretical max.', 'Small@DRB10', 'Small+DQL' ), loc='upper right')

ax[1].legend(('CoDel@QFI_DRB@DQL', 'BBR', 'Theoretical max.', 'Small@DRB10', 'Small+DQL'), loc='upper right')

plt.rcParams.update({'font.size': 23})

ax[0].grid()
ax[1].grid()

plt.show()

