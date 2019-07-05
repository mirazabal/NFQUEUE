import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as mpatches

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
                        if n1 > 500000 and n1 < 35000000 :
                            y.append(n)
                    except Exception:
                        pass
            except Exception:
                pass
    print 'Mean value for ' + str1 + ' ' + str(np.mean(y))
    return (y, np.std(y))

def get_delay_values(str1):
    val =  get_delay(str1)
    return val

def generate_delay_mean(str1):

    arr_ping_delay = []
    arr_ping_std = []

    val = get_delay_values(str1 + '/a_10.txt') 
    arr_ping_delay.append(val[0])
    arr_ping_std.append(val[1])

    val = get_delay_values(str1 + '/a_20.txt') 
    arr_ping_delay.append(val[0])
    arr_ping_std.append(val[1])


    val = get_delay_values(str1 + '/a_30.txt') 
    arr_ping_delay.append(val[0])
    arr_ping_std.append(val[1])

    val = get_delay_values(str1 + '/a_40.txt') 
    arr_ping_delay.append(val[0])
    arr_ping_std.append(val[1])

    val = get_delay_values(str1 + '/a_50.txt') 
    arr_ping_delay.append(val[0])
    arr_ping_std.append(val[1])

    val = get_delay_values(str1 + '/a_60.txt') 
    arr_ping_delay.append(val[0])
    arr_ping_std.append(val[1])

    val = get_delay_values(str1 + '/a_70.txt') 
    arr_ping_delay.append(val[0])
    arr_ping_std.append(val[1])

    return arr_ping_delay

y1 = generate_delay_mean('1st')
y3 = generate_delay_mean('3th')
y6 = generate_delay_mean('6th')
y7 = generate_delay_mean('7th')
y10 = generate_delay_mean('10th')
y11 = generate_delay_mean('11th')
y12 = generate_delay_mean('12th')



ticks = ['10', '20', '30', '40', '50', '60', '70']

def set_box_color(bp, color):
    plt.setp(bp['boxes'], color=color)
    plt.setp(bp['whiskers'], color=color)
    plt.setp(bp['caps'], color=color)
    plt.setp(bp['medians'], color=color)

plt.figure()

b_1 = plt.boxplot(y1, positions=np.array(xrange(len( y1)))*7.0-1.8, sym='', widths=0.5)
b_3 = plt.boxplot(y3, positions=np.array(xrange(len( y3)))*7.0-1.2, sym='', widths=0.5)
b_6 = plt.boxplot(y6, positions=np.array(xrange(len( y6)))*7.0-0.6, sym='', widths=0.5)
b_7 = plt.boxplot(y7, positions=np.array(xrange(len( y7)))*7.0, sym='', widths=0.5)
b_10 = plt.boxplot(y10, positions=np.array(xrange(len( y10)))*7.0+0.6 , sym='', widths=0.5)
b_11 = plt.boxplot(y11, positions=np.array(xrange(len( y11)))*7.0+1.2 , sym='', widths=0.5)
b_12 = plt.boxplot(y12, positions=np.array(xrange(len( y12)))*7.0+1.8, sym='', widths=0.5)


#bpl = plt.boxplot(y10d, sym='', widths=0.6)
#bpr = plt.boxplot(y11d, sym='', widths=0.6)

set_box_color(b_1, 'cyan') # colors are from http://colorbrewer2.org/
set_box_color(b_3, 'blue') # colors are from http://colorbrewer2.org/
set_box_color(b_6, 'red') # colors are from http://colorbrewer2.org/
set_box_color(b_7, 'green')
set_box_color(b_10, 'purple')
set_box_color(b_11, 'yellow')
set_box_color(b_12, 'pink')

plt.xlabel('Ping interval [ms]')
plt.ylabel('Delay in [ms]')


                    # draw temporary red and blue lines and use them to create a legend
plt.plot([], c='cyan', label='Vanilla')
plt.plot([], c='blue', label='CoDel')
plt.plot([], c='red', label='DQL')
plt.plot([], c='green', label='BBR')
plt.plot([], c='purple', label='DynRLC')
plt.plot([], c='yellow', label='CQI_PACER')
plt.plot([], c='pink', label='CQI_PACER_ASYNC')
plt.legend()

plt.xticks(xrange(0, len(ticks) * 7, 7), ticks)
plt.xlim(-5, len(ticks)*7)
#plt.ylim(0, 8)
plt.tight_layout()
plt.savefig('boxcompare.png')


plt.show()

