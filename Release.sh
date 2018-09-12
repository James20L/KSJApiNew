#!/bin/bash

if [ "$1" ]; then
	echo "releae for $1"
else
	echo "please set  $1 for platform"
	exit
fi


excludelistfile="$1""_exclude"

echo "$excludelistfile"


#find . -iname *.log | xargs rm 
#find . -iname *.o | xargs rm

excludedirs="--exclude-from=$excludelistfile"
echo $excludedirs
releasefilename="../github.KSJApiNew_"$1".tar.bz2"
tar -cjpvf $releasefilename   $excludedirs  ../github.KSJApiNew 





