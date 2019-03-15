#!/bin/bash
#echo "===== Set Evn ======"
#sudo sh -c 'echo 1000 > /sys/module/usbcore/parameters/usbfs_memory_mb'
echo "===== Start app ......====="
sudo LD_LIBRARY_PATH=/home/firefly/KSJ/101/github.KSJApiNew/KSJApi.bin/linux/arm64 /home/firefly/KSJ/101/github.KSJApiNew/KSJDemoQT_QImage/KSJShow/KSJShow 
