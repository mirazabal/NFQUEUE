import os
import shutil

dirName = "build"

bashCommand =  "sudo rm -r *.out" 
os.system(bashCommand)


if os.path.exists(dirName):
    shutil.rmtree(dirName) 

if not os.path.exists(dirName):
    os.makedirs(dirName)
    print("Directory " , dirName ,  " Created ")
else:    
    print("Directory " , dirName ,  " already exists")    

exeNames = ['a11.out', 'a121.out', 'a122.out', 'a123.out', 'a13.out', 'a141.out',  'a142.out', 'a143.out', 'a15.out', 'a16.out', 'a17.out', 'a18.out', 'a19.out', 'a110.out', 'a111.out' , 'a112.out', 'a113.out', 'a114.out', 'a115.out', 'a116.out','a117.out', 'a21.out', 'a221.out', 'a222.out', 'a223.out', 'a23.out', 'a241.out', 'a242.out', 'a243.out', 'a25.out', 'a26.out', 'a27.out', 'a28.out', 'a29.out', 'a210.out', 'a211.out', 'a212.out']
#exeNames = ['a11.out', 'a121.out']

flagNames = ['-DFIRST_1st_f=1', '-DFIRST_2nd_10_f=1', '-DFIRST_2nd_20_f=1', '-DFIRST_2nd_30_f=1', '-DFIRST_3rd_f=1','-DFIRST_4th_10_f=1','-DFIRST_4th_20_f=1','-DFIRST_4th_30_f=1','-DFIRST_5th_f=1','-DFIRST_6th_f=1','-DFIRST_7th_f=1','-DFIRST_8th_f=1', '-DFIRST_9th_f=1', '-DFIRST_10th_f=1', '-DFIRST_11th_f=1',  '-DFIRST_12th_f=1',  '-DFIRST_13th_f=1', '-DFIRST_14th_f=1',  '-DFIRST_15th_f=1',  '-DFIRST_16th_f=1',  '-DFIRST_17th_f=1', '-DSECOND_1st_f=1', '-DSECOND_2nd_10_f=1', '-DSECOND_2nd_20_f=1', '-DSECOND_2nd_30_f=1', '-DSECOND_3rd_f=1','-DSECOND_4th_10_f=1','-DSECOND_4th_20_f=1','-DSECOND_4th_30_f=1','-DSECOND_5th_f=1','-DSECOND_6th_f=1','-DSECOND_7th_f=1','-DSECOND_8th_f=1', '-DSECOND_9th_f=1', '-DSECOND_10th_f=1', '-DSECOND_11th_f=1',  '-DSECOND_12th_f=1' ]
#flagNames = ['-DFIRST_1st_f=1', '-DFIRST_2nd_10_f=1']

for idx,exe in enumerate(exeNames):
    if os.path.exists(dirName):
        shutil.rmtree(dirName) 

    if not os.path.exists(dirName):
        os.makedirs(dirName)
        print("Directory " , dirName ,  " Created ")
    else:    
        print("Directory " , dirName ,  " already exists")    

    bashCommand =  "cd build/ && cmake .. " + flagNames[idx] + "  && make"
    os.system(bashCommand)

    bashCommand = "cp  build/5G_QoS . && mv 5G_QoS " + exe
    os.system(bashCommand)

