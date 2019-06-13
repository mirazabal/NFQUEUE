import math

arr_CQI_2_MCS = [0,2,4,6,8,9,10,13,16,17,19,21,23,25,28]

arr_MCS_2_ITBS = [0,1,2,3,4,5,6,7,8,9,9,10,11,12,13,14,15,15,16,17,18,19,20,21,22,23,24,25,26] 

# for 25 PRB
arr_ITBS_2_TBS = [680,904,1096,1416,1800,2216,2600,3112,3496,4008,4392,4968,5736,6456,7224,7736,7992,9144,9912,10680,11448,12576,13536,14112,15264,15840,18336,16416]

def get_CQIs(str1):
    f = open(str1, 'r')
    x = []
    next(f) # skip first line
    for line in f:
        my_list =  line.split(",")
        s = my_list[10]
        if s != '-':
            x.append( int(s) )
    return x



def get_num_packets(cqis):
    arr_data = []
    for cqi in cqis:
        mcs = arr_CQI_2_MCS[cqi-1]
        ITBS = arr_MCS_2_ITBS[mcs]
        TBS = arr_ITBS_2_TBS[ITBS]
        TBS_bits = (TBS*1000)/(1460*8*100)
        min_val = (int)(max(1, math.floor(TBS_bits)))
        packet =  min(15,min_val)
        arr_data.append(packet)
    return arr_data 

cqis = get_CQIs('../Dataset/pedestrian/A_2017.11.21_15.03.50.csv')
print cqis


packets = get_num_packets(cqis)


print packets


f = open('packets.txt','w') 

f.write( str(len(packets))  + "\n" ) 
for pack in packets:

    f.write(str(pack) + "\n" ) 
f.close()

