#!/bin/bash

FILE=timetaken_wrt_threads.plot

#$1 is the mode for running this script
# 1 -> MAP_POPULATE, 0 -> NONE

if [[ "$#" -ne 1 ]]; 
then
    echo "give Mode{0,1} -> MAP_POPULATE "
    exit -1
fi

echo "This test requires 4GB of DiskSpace and atleast 4GB RAM"
##dd if=/dev/urandom of=randomfile.txt bs=4096 count=1048576

make -j
a=`nproc`
echo "Number of processors = $a"
#set -x
rm $FILE ##reset the file for plot.. Rename your file for 

for ((i = 1 ; i <= $a ; i++))
do
    ./SharedmmapPopulate $i $1 | tail -n1 | tee /dev/tty | awk '{print '"$i"', $8}' >> $FILE
done
#set +x
