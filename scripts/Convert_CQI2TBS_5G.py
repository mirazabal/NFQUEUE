

arr_MCS = [[2,120], [2,157] , [2,193], [2,251], [2, 308 ], [2, 379], [2, 449], [2,526], [2,602], [2,679], [4,340], [4,378], [4,434], [4,490], [4,553], [ 4, 616], [4, 658], [6,438], [6,466], [6,517], [6,567], [6,616], [6,666], [6,719], [6,772], [6,822], [6,873], [6,910], [6,948] ]

arr_CQI_2_MCS = [0,2,4,6,8,9,10,13,16,17,19,21,23,25,28]


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

def get_R_Qm(cqi):
    print 'CQI val =  ' + str(cqi)
    ind = arr_CQI_2_MCS[cqi-1]
    print 'ind val = ' + str(ind)
    return arr_MCS[ ind ]

def calc_Ninfo(cqis):
    
    Ninfos = []
    
    nPRB = 25
    symInSlot = 14
    DCI_format_1 = 0
    overhead_PDSCH = 0
    Nre = (12* symInSlot - DCI_format_1 - overhead_PDSCH )*nPRB
    v = 1 # number of layers???
    for cqi in cqis:
        val = get_R_Qm(cqi)
        Qm = val[0]
        R = val[1]
        Ninfo = Nre * R * Qm * v    
        Ninfos.append(Ninfo)
    return Ninfos


cqis = get_CQIs('../Dataset/pedestrian/A_2017.11.21_15.03.50.csv')

Ninfos = calc_Ninfo(cqis)

print Ninfos
print len(Ninfos)
#print cqis
#print len(cqis)



