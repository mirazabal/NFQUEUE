#!/bin/bash

_A_DEST_FILE="a_"
_PING_DEST_FILE="ping_"
_IPERF_DEST_FILE="iperf_"
_FILE_END=".txt"
EXE_A="a.out"
CASE="1st"
DIRECTORY="JOURNAL_PING_DYN_FINAL_TRAIN"
#SCENARIO="2nd_scenario"
PING_LAT="10"
PING_QUANTITY="3000"

#array_exe_sce_0=('a11.out' 'a121.out' 'a122.out' 'a123.out' 'a13.out' 'a141.out' 'a142.out' 'a143.out' 'a15.out' 'a16.out' 'a17.out' 'a18.out'  'a19.out' 'a110.out' 'a111.out' 'a112.out')
#array_exe_sce_0=('a16.out' 'a17.out' 'a18.out'  'a19.out' 'a110.out' 'a111.out' 'a112.out')
array_exe_sce_0=( 'a21.out' 'a23.out' 'a26.out' 'a27.out' 'a28.out' 'a29.out' 'a210.out' 'a211.out' 'a212.out')
#array_exe_sce_0=( 'a11.out' 'a17.out' 'a110.out' 'a111.out'  'a112.out' 'a113.out' 'a114.out' 'a115.out' 'a116.out' 'a117.out')
#array_exe_sce_0=( 'a111.out')
#array_exe_sce_0=( 'a114.out')
#array_exe_sce_0=('a211.out')
#array_exe_sce_1=('a21.out' 'a221.out' 'a222.out' 'a223.out' 'a23.out' 'a241.out' 'a242.out' 'a243.out' 'a25.out' 'a26.out' 'a27.out' 'a28.out' 'a29.out' 'a210.out')

#array_cases=('1st' '2nd_10' '2nd_20' '2nd_30' '3rd' '4th_10' '4th_20' '4th_30' '5th' '6th' '7th' '8th' '9th' '10th' '11th' '12th' )
array_cases=('1st' '3rd' '6th' '7th' '8th' '9th' '10th' '11th' '12th' )
#array_cases=('11th')
#array_cases=('14th')
#array_cases=('11th')
array_scenarios=('2nd_scenario')
#array_scenarios=('1st_scenario' '2nd_scenario')


array_ms=('10' '20' '30' '40' '50' '60' '70' )
#array_ms=('10' )
#array_lat=( '50' '70' )
#array_quantity=('9000' '4500' '3000' '2250' '1800' '1500' '1320' )
array_quantity=('15000' '7500' '5000' '6750' '3000' '2500' '2150' )
#array_quantity=('15000')
#array_quantity=( '600' '430' )

#array_ms=( '30' '50' '70')

idx_sce=0
while [ $idx_sce -le 1 ];
do
  SCENARIO=${array_scenarios[$idx_sce]}

  idx_cases=0
  while [ $idx_cases -le 8 ];
  do

    if [ $idx_sce == 0 ]; then    
      EXE_A=${array_exe_sce_0[$idx_cases]}
    fi

    if [ $idx_sce == 1 ]; then    
      EXE_A=${array_exe_sce_1[$idx_cases]}
    fi

    CASE=${array_cases[$idx_cases]}
    echo "the value of the exe is equal to ${array_exe_sce[$idx_cases]}"

    idx_ping=0
    while [ $idx_ping -le 6 ];
    do
      A_DEST_FILE="$_A_DEST_FILE${array_ms[$idx_ping]}.txt"
      PING_DEST_FILE="$_PING_DEST_FILE${array_ms[$idx_ping]}.txt"
      IPERF_DEST_FILE="$_IPERF_DEST_FILE${array_ms[$idx_ping]}.txt"

      PING_LAT=${array_ms[$idx_ping]}
      PING_QUANTITY=${array_quantity[$idx_ping]}


      export EXE_A
      export A_DEST_FILE
      export PING_DEST_FILE
      export IPERF_DEST_FILE
      export PING_LAT
      export PING_QUANTITY
      export CASE
      export DIRECTORY
      export SCENARIO
      ./startExp_ping_impl.sh & 
      pid[0]=$!
      sleep 210
      ps -A | grep $EXE_A  | awk '{print $1}' | xargs sudo kill -9
      trap INT "kill $(pid[0]);"

      idx_ping=$(( $idx_ping + 1 )) 
    done

    echo "into new case"
    ps -A | grep $EXE_A  | awk '{print $1}' | xargs sudo kill -9
    idx_cases=$(( $idx_cases + 1 )) 

  done
  idx_sce=$(( $idx_sce + 1 )) 
done

kill 0

