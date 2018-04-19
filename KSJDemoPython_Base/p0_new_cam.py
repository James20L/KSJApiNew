import numpy as np
import os
import socket
import sys
import signal
import uuid
from queue import Queue
from threading import Thread
import time
import datetime
import cv2
from imutils.video import WebcamVideoStream
from ctypes import *

import pygame.camera
from imutils.video import WebcamVideoStream
from ctypes import *
from pathlib import Path

# from track_head import predict, initialize

SEQUENCE_FILE = "./f0.txt"
guid = uuid.uuid4()
home = str(Path.home())
print(home)
now = datetime.datetime.now()
datestr = str(now.year).zfill(4) + str(now.month).zfill(2) + str(now.day).zfill(2)
SIMPLIFIED_PATHS = [os.path.join(home, 'datasets/freezerzw/' + datestr + '.SIMPLE/'), str(guid)]
PATHS = [os.path.join(home, 'datasets/freezerzw/' + datestr), str(guid)]
FREEZER_CAM_NUM = 2
WIDTH = 1936
HEIGHT = 1216
MAX_INDICES_QUEUE_LENGTH = 20000

should_stop = False

def signal_handler(signal, frame):
    print('稍等一下程序马上结束')
    global should_stop
    should_stop = True

signal.signal(signal.SIGINT, signal_handler)

path = os.path.join(*PATHS)
if not os.path.exists(path):
    os.makedirs(path)
print(path)

sim_path = os.path.join(*SIMPLIFIED_PATHS)
if not os.path.exists(sim_path):
    os.makedirs(sim_path)
print(sim_path)

def ksj_init():
    lib_ksj = cdll.LoadLibrary('libksjapi.so')
    lib_ksj.KSJ_Init()
    cam_num =lib_ksj.KSJ_DeviceGetCount()
    print(cam_num)
    if cam_num < 1:
        print("no cam found")
    else:
        print("cam found")
    return lib_ksj, cam_num

def cam_parm_set(libKsj, num):
    for i in range(0, num):
        usDeviceType = c_int()
        nSerials = c_int()
        usFirmwareVersion = c_int()
        libKsj.KSJ_DeviceGetInformation(i, byref(usDeviceType), byref(nSerials), byref(usFirmwareVersion))  
        '''
        nSerials for distinguish camera
        '''
        print(usDeviceType)
        print(nSerials)
        print(usFirmwareVersion)
        libKsj.KSJ_CaptureSetFieldOfView(i, 0, 0, WIDTH, HEIGHT, 0, 0)
        nColStart = c_int()
        nRowStart = c_int()
        nColSize = c_int()
        nRowSize = c_int()
        ColAddressMode = c_int()
        RowAddressMode = c_int() 
#        libKsj.KSJ_CaptureGetDefaultFieldOfView(i,byref(nColStart),byref(nRowStart),byref(nColSize),byref(nRowSize),byref(ColAddressMode),byref(RowAddressMode))        
        libKsj.KSJ_CaptureGetFieldOfView(i, byref(nColStart), byref(nRowStart), byref(nColSize), byref(nRowSize), byref(ColAddressMode), byref(RowAddressMode))        
        print(nColStart.value)
        print(nRowStart.value)
        print(nColSize.value)
        print(nRowSize.value)
        print(ColAddressMode.value)
        print(RowAddressMode .value)
        '''
        for set bayer mode ,this is relative to the filp do not change        
        '''
        libKsj.KSJ_BayerSetMode(i, 3);
        '''
        for set whitbalance mode 7 stand for auto continus        
        '''
        libKsj.KSJ_WhiteBalanceSet(i, 7);
#        nRet = libKsj.KSJ_WhiteBalanceSet(i, 5);
        print("libKsj.KSJ_WhiteBalanceSet");
       # print(nRet);

#        nRet = libKsj.KSJ_WhiteBalancePresettingSet(i, 0)
        print("libKsj.KSJ_WhiteBalancePresettingSet");
       # print(nRet);
        fexpTime = c_float()
        fexpTime = 1.8
        libKsj.KSJ_ExposureTimeSet.argtypes = (c_int, c_float)
        libKsj.KSJ_ExposureTimeSet(i, fexpTime);
        '''
        for set gain ,do not change the 2nd parm, 3rd parm is gain value 
        for 120w set RGB
        for 230w set 1,xxx
        '''
        libKsj.KSJ_SetParam(i, 1, 100);
        libKsj.KSJ_SetParam(i, 2, 100);
        libKsj.KSJ_SetParam(i, 3, 100);
        '''
        for set the sensitivity 2 stand for high
        move 2 bits
        '''  
        libKsj.KSJ_SensitivitySetMode(i, 2)
        '''
        for set color correction 3 is hardware present
        after set bayer, do color enhancement
        '''
        libKsj.KSJ_ColorCorrectionSet(i, 3)

        #libKsj.KSJ_AESetRegion(i, 320, 240, 640, 480)
        #libKsj.KSJ_AESetExposureTimeRange.argtypes = (c_int, c_float, c_float)
        #libKsj.KSJ_AESetExposureTimeRange(i, 0.1, 3.00)
        #libKsj.KSJ_AEStart.argtypes = (c_int, c_bool, c_int ,c_int);        
        
        #libKsj.KSJ_AEStart(i, True, -1, 180)

        


def CaptureData(nIndex, cBuf, nHeight, nWidth, nChannelNum): 
    retValue = lib_ksj.KSJ_CaptureRgbData(nIndex, cBuf)
    if retValue != 0:
        print("capture error code %d" % (retValue))
    nparr = np.fromstring(cBuf, np.uint8).reshape(nHeight, nWidth, nChannelNum)
    return nparr
 
def create_buf(libKsj, num):
    buflist = []
    nWidth = c_int()
    nHeight = c_int()
    nBitCount = c_int()
    for i in range(0, num) :
        libKsj.KSJ_CaptureGetSizeEx(i, byref(nWidth), byref(nHeight), byref(nBitCount))
        print(nWidth)
        print(nHeight)
        print(nBitCount)
        nbufferSize = int(nWidth.value * nHeight.value * nBitCount.value / 8)
        pRawData = create_string_buffer(nbufferSize)
        buflist.append(pRawData)
    return buflist

def unix_time_millis(dt, epoch):
    return int((dt - epoch).total_seconds() * 1000.0)

def save_data(imgs, path, indices_q, timestamps):
    for idx, img in enumerate(imgs):
        txt_path = os.path.join(path, str(idx) + '.txt')
        img_path = os.path.join(path, str(idx))
        if not os.path.exists(img_path):
            os.makedirs(img_path)
        index = str(int(indices_q.qsize() / FREEZER_CAM_NUM))
        img_path = os.path.join(img_path, index + '.jpg')
        if path is not sim_path:
            indices_q.put([1])
        txt_file = open(txt_path, "a")
        img_file = open(img_path, "wb+")
        txt_file.write(str(timestamps[idx]) + '\n')
        txt_file.close()
        img_file.write(img)
        img_file.close()
        if idx == 0:
            print('index: {0}'.format(index))
    

def main_thread(indices_q, buf_list):
    import datetime

    t1 = datetime.datetime.now()
    while True:
        imgs = []
        timestamps = []
        # for cam_idx, cam in enumerate(top_cams):
        for cam_idx, buf in enumerate(buf_list):
            img = CaptureData(cam_idx, buf, HEIGHT, WIDTH, 3)
            img_str = cv2.imencode('.jpg', img)[1].tostring()
            dt = datetime.datetime.now()
            epoch = datetime.datetime.utcfromtimestamp(0)
            timestamps.append(unix_time_millis(dt=dt, epoch=epoch))
            imgs.append(img_str)
        save_data(imgs, path, indices_q, timestamps)
        index = int(indices_q.qsize() / FREEZER_CAM_NUM)
        if index % 2 == 0:
            save_data(imgs, sim_path, indices_q, timestamps)
        delta = datetime.datetime.now() - t1
        if delta > datetime.timedelta(seconds = 20) or should_stop:
            print('cam: {0} and cnt:{1} and time {2}, fps: {3}'.format(cam_idx, index, delta, index / float(delta.total_seconds())))
            break

if __name__ == "__main__":
    init_res = ksj_init()
    lib_ksj = init_res[0]
    cam_num = init_res[1]
    global FREEZER_CAM_NUM
    FREEZER_CAM_NUM = cam_num

    cam_parm_set(lib_ksj, cam_num)
    buf_list = create_buf(lib_ksj, cam_num)

    indices_q = Queue(maxsize=MAX_INDICES_QUEUE_LENGTH)
    main_t = Thread(target=main_thread, args=(indices_q, buf_list))
    main_t.start()
    main_t.join()
    lib_ksj.KSJ_UnInit()

