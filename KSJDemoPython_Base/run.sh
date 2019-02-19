#!/bin/bash
sudo  chmod -R 777 /dev/bus/usb
sudo  LD_LIBRARY_PATH=../KSJApi.bin/linux/x64 python3 camtest.py
