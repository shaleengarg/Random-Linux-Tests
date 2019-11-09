#!/bin/bash

FILE1=Run_NOMAP_POPULATE.plot
FILE2=Run_MAP_POPULATE.plot
FINALPLOT=NO_mmap_MAP_POPULATE.plot

#$1 is the mode for running this script
# 1 -> MAP_POPULATE, 0 -> NONE

#if [[ "$#" -ne 1 ]]; 
#then
#    echo "give Mode{0,1} -> MAP_POPULATE "
#    exit -1
#fi


echo "This test requires 4GB of DiskSpace and atleast 4GB RAM"
dd if=/dev/urandom of=randomfile.txt bs=4096 count=1048576

make -j
a=`nproc`
echo "Number of processors = $a"

rm $FILE1 ##reset the file for plot.. Rename your file for 
for ((i = 1 ; i <= $a ; i++))
do
    ./SharedmmapPopulate $i 0 | tail -n1 | tee /dev/tty | awk '{print '"$i"', $8}' >> $FILE1
done


echo "Now trying with MAP_POPULATE"

rm $FILE2 ##reset the file for plot.. Rename your file for 
for ((i = 1 ; i <= $a ; i++))
do
    ./SharedmmapPopulate $i 1 | tail -n1 | tee /dev/tty | awk '{print '"$i"', $8}' >> $FILE2
done

join -1 1 -2 1 $FILE1 $FILE2 > $FINALPLOT
