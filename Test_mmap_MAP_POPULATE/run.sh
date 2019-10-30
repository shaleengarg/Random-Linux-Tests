#!/bin/bash

#$1 is the mode for running this script
# 1 -> MAP_POPULATE, 0 -> NONE

if [[ "$#" -ne 1 ]]; 
then
    echo "give Mode{0,1} -> MAP_POPULATE "
    exit -1
fi

make
a=`nproc`
echo "Namber of processors = $a"
#set -x
for ((i = 1 ; i <= $a ; i++))
do
    ./SharedmmapPopulate $i $1 | tail -n1 | awk '{print '"$i"', $8}' >> timetaken_wrt_threads.plot
done
#set +x
