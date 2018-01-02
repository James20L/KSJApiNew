#!/bin/bash

for((i=0;i<1000;i++))
do

echo "loop $i"
LD_LIBRARY_PATH=../github.KSJApi.bin/linux/x64   ./catchbesttest

done



