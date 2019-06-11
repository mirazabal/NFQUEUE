#!/bin/bash
#sleep 1
echo "the variable is equal to  $EXE_A"


cd /home/mikel/workspace/NFQUEUE/

ps -A | grep $EXE_A  | awk '{print $1}' | xargs sudo kill -9

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

if [ ! "$CASE" == "7th" ]; then
echo "cubic" > /proc/sys/net/ipv4/tcp_congestion_control
fi

if [ "$CASE" == "7th" ]; then
echo "bbr" > /proc/sys/net/ipv4/tcp_congestion_control
fi

iperf3 -c 192.168.1.145 -t 45 -M 1500  $NUM_CONN > $IPERF_DEST_FILE &
sleep 3
cd /home/pi/UDP/client

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

./../../../a.out 192.168.1.145 10020 $PING_LAT $PING_QUANTITY > $IPERF_DEST_FILE &

sleep 50
HERE
sleep 50

ps -A | grep /../../../$EXE_A  | awk '{print $1}' | xargs sudo kill -9

kill 0

