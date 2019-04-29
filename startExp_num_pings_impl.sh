#!/bin/bash
#sleep 1
echo "the variable is equal to  $EXE_A"

cd /home/oai/workspace/libnetfilter_queue/mib/

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

iperf3 -c 192.168.1.229 -t 45 -M 1500  $NUM_CONN > $IPERF_DEST_FILE &
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

echo "number of ping = $NUM_PINGS"
echo "Latency =  $PING_LAT"

if [ $MIB_NUM_PINGS -eq 1 ]; then
./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "1_1_$IPERF_DEST_FILE" &
fi

if [ $MIB_NUM_PINGS -eq 2 ]; then
./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "2_1_$IPERF_DEST_FILE" &
./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "2_2_$IPERF_DEST_FILE" &
fi

if [ $MIB_NUM_PINGS -eq 4 ]; then
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "4_1_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "4_2_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "4_3_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "4_4_$IPERF_DEST_FILE" &
fi

if [ $MIB_NUM_PINGS -eq 8 ]; then
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "8_1_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "8_2_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "8_3_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "8_4_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "8_5_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "8_6_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "8_7_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "8_8_$IPERF_DEST_FILE" &
fi

if [ $MIB_NUM_PINGS -eq 16 ]; then
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_1_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_2_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_3_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_4_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_5_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_6_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_7_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_8_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_9_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_10_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_11_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_12_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_13_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_14_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_15_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "16_16_$IPERF_DEST_FILE" &
fi

if [ $MIB_NUM_PINGS -eq 32 ]; then
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_1_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_2_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_3_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_4_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_5_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_6_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_7_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_8_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_9_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_10_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_11_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_12_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_13_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_14_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_15_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_16_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_17_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_18_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_19_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_20_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_21_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_22_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_23_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_24_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_25_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_26_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_27_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_28_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_29_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_30_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_31_$IPERF_DEST_FILE" &
	./../../../a.out 192.168.1.229 10020 $PING_LAT $PING_QUANTITY > "32_32_$IPERF_DEST_FILE" &
fi


sleep 50

HERE
sleep 50

ps -A | grep ../../../$EXE_A | awk '{print $1}' | xargs sudo kill -9

kill 0
