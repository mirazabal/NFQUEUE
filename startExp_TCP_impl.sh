#!/bin/bash
#sleep 1

if [ ! -d "$DIRECTORY" ]; then
mkdir $DIRECTORY
fi

cd $DIRECTORY

if [ ! -d "$SCENARIO" ]; then
mkdir $SCENARIO
fi

cd $SCENARIO

if [ ! -d "$CASE" ]; then
mkdir $CASE
fi

cd $CASE

sudo ./../../../$EXE_A 0 > $A_DEST_FILE &

sleep 1
sshpass -p "linux" ssh -tt -o StrictHostKeyChecking=no root@192.168.1.202 << HERE 
cd /home/pi/

if [ ! -d "$DIRECTORY" ]; then
mkdir $DIRECTORY
fi
cd $DIRECTORY

if [ ! -d "$SCENARIO" ]; then
mkdir $SCENARIO
fi
cd $SCENARIO

if [ ! -d "$CASE" ]; then
mkdir $CASE
fi
cd $CASE



ping 192.168.1.229 -s 100 -i 0.035 -c 857 > $PING_DEST_FILE &
iperf3 -c 192.168.1.229 -t 30 -M 1500 -P $NUM_CONN > $IPERF_DEST_FILE &
sleep 50
HERE
sleep 50

ps -A | grep ../../../$EXE_A | awk '{print $1}' | xargs sudo kill -9
