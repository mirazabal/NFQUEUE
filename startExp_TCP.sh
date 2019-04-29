#!/bin/bash

_A_DEST_FILE="a_"
_PING_DEST_FILE="ping_"
_IPERF_DEST_FILE="iperf_"
_FILE_END=".txt"
EXE_A="a.out"
CASE="1st"
DIRECTORY="JOURNAL_TCP"
SCENARIO="1st_scenario"
array=(1 2 4 8 16 32 64)

array_exe_sce_0=('a11.out' 'a121.out' 'a122.out'  'a123.out' 'a13.out' 'a141.out' 'a142.out' 'a143.out' 'a15.out' 'a16.out' 'a17.out' 'a18.out')
array_exe_sce_1=('a21.out' 'a221.out' 'a222.out'  'a223.out' 'a23.out' 'a241.out' 'a242.out' 'a243.out' 'a25.out' 'a26.out' 'a27.out' 'a28.out')

#array_exe_sce_1=('a.out', 'a.out', 'a.out' , 'a.out', 'a.out', 'a.out', 'a.out', 'a.out', 'a.out')
#array_exe_sce_2=('a.out', 'a.out', 'a.out' , 'a.out', 'a.out', 'a.out', 'a.out', 'a.out', 'a.out')

array_cases=('1st' '2nd_10' '2nd_20' '2nd_30' '3rd' '4th_10' '4th_20' '4th_30' '5th' '6th' '7th' '8th')
array_scenarios=('1st_scenario' '2nd_scenario')

idx_sce=0
while [ $idx_sce -le 1 ];
do
	SCENARIO=${array_scenarios[$idx_sce]}

	idx_cases=0
	while [ $idx_cases -le 11 ];
	do


		if [ $idx_sce == 0 ]; then    
			EXE_A=${array_exe_sce_0[$idx_cases]}
		fi

		if [ $idx_sce == 1 ]; then    
			EXE_A=${array_exe_sce_1[$idx_cases]}
		fi

		CASE=${array_cases[$idx_cases]}

		for k in "${array[@]}"
		do
			A_DEST_FILE="$_A_DEST_FILE$k.txt"
			PING_DEST_FILE="$_PING_DEST_FILE$k.txt"
 			IPERF_DEST_FILE="$_IPERF_DEST_FILE$k.txt"
  		NUM_CONN=$k

			export EXE_A
			export A_DEST_FILE
			export PING_DEST_FILE
			export IPERF_DEST_FILE
			export NUM_CONN
			export CASE
			export DIRECTORY
			export SCENARIO
			./startExp_TCP_impl.sh & 
			pid[0]=$!
			sleep 50
			trap INT "kill $(pid[0]);"

		done

		ps -A | grep $EXE_A  | awk '{print $1}' | xargs sudo kill -9
		idx_cases=$(( $idx_cases + 1 )) 

	done
	idx_sce=$(( $idx_sce + 1 )) 
done

kill 0

