#!/bin/bash
find . -iname *.log | xargs rm 
find . -iname *.o | xargs rm
excludedirs = --exclud=KSJDemoAndroid_Base --exclude=.git --exclude=Release.sh
tar -cjpvf ../github.KSJApiNew.tar.bz2  ../github.KSJApiNew  $excludedirs

