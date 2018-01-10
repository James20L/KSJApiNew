#!/bin/bash
find . -iname *.log | xargs rm 
find . -iname *.o | xargs rm
tar -cjpvf ../github.KSJApiNew.tar.bz2  ../github.KSJApiNew --exclude=.git --exclude=Release.sh   

