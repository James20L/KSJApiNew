# -*- coding: utf-8 -*-
"""
Created on Tue Nov 21 12:18:11 2017

@author: root
"""
import time
import datetime
import signal
import threading
import time
import numpy as np
import cv2
from ctypes import * 

nThreadFlag =1;
g_nWidth = 1280
g_nheight = 960

def KsjInit():
    libKsj = cdll.LoadLibrary('libksjapi.so')
    libKsj.KSJ_Init()
    camCount =libKsj.KSJ_DeviceGetCount()
    print(camCount)
    if camCount < 1:
        print("no cam fount")
    else:
        print("cam found")
    return libKsj,camCount

def CamParmSet(libKsj,num):
        
global
    for i in range(0,num):
        libKsj.KSJ_CaptureSetFieldOfView(i,0,0,g_nWidth,g_nheight,0,0)
        nColStart = c_int()
        nRowStart = c_int()
        nColSize = c_int()
        nRowSize = c_int()
        ColAddressMode = c_int()
        RowAddressMode = c_int() 
        libKsj.KSJ_CaptureGetFieldOfView(i,byref(nColStart),byref(nRowStart),byref(nColSize),byref(nRowSize),byref(ColAddressMode),byref(RowAddressMode))        
        print(nColStart.value)
        print(nRowStart.value)
        print(nColSize.value)
        print(nRowSize.value)
        print(ColAddressMode.value)
        print(RowAddressMode.value)

        libKsj.KSJ_WhiteBalanceSet(i,7);
        fexpTime = c_float()
        fexpTime=5.00        
        libKsj.KSJ_ExposureTimeSet.argtypes = (c_int,c_float)
        libKsj.KSJ_ExposureTimeSet(i,fexpTime);


def CapturData(nIndex,cBuf,nHeight,nWidth,nChannelNum): 
    retValue = libKsj.KSJ_CaptureRgbData(nIndex,cBuf)
    if retValue != 0:
        print("capture error code %d"%(retValue))
  
    nparr = np.fromstring(cBuf,np.uint8).reshape(nHeight,nWidth,nChannelNum );     
    return nparr
 
def QuitAll():
    global nThreadFlag
    print("exit")
    nThreadFlag=0;
    
def CreateBuf(libKsj,num):
    buflist=[]
    nWidth = c_int()
    nHeight = c_int()
    nBitCount = c_int()
    for i in range(0,num) :
        libKsj.KSJ_CaptureGetSizeEx(i,byref(nWidth), byref(nHeight),byref(nBitCount))
        print(nWidth)
        print(nHeight)
        print(nBitCount)
        nbufferSize = int(nWidth.value*nHeight.value*nBitCount.value/8)
        pRawData = create_string_buffer(nbufferSize)
        buflist.append(pRawData)
    return buflist
          

def CapturDataLoop(nIndex,pDataBuf):
    print("cam loop nIndex = %d" %(nIndex))
    global nThreadFlag
    print("nThreadFlag = %d"%(nThreadFlag))
    nFrameCount =0
    nTimeStart = datetime.datetime.now() 
    nTimeStop = datetime.datetime.now()
  
    while nThreadFlag > 0:
       
        image =  CapturData(nIndex,pDataBuf,1216,1936,3)
        
        # cv2.imshow("test",image)
        # cv2.waitKey(50)

        if nFrameCount == 0:
            nTimeStart = datetime.datetime.now() 
            
        if nFrameCount == 99:
            nTimeStop = datetime.datetime.now() 
            print("cam   %d " %(nIndex))
            print(nTimeStop-nTimeStart)
            nFrameCount=-1
            
        nFrameCount=nFrameCount+1    
        # time.sleep(0.001)
    print("thread quit")

if __name__ == '__main__':
    initRes=KsjInit()
    libKsj=initRes[0]
    camNub=initRes[1]
    print(libKsj)
    print(camNub)
    CamParmSet(libKsj,camNub)
    bufList=CreateBuf(libKsj,camNub)
    threadlist=[]
    for i in range(0,camNub):
    
        threadlist.append(threading.Thread(target=CapturDataLoop,args=(i,bufList[i])))     

        threadlist[i].setDaemon(True)
        threadlist[i].start()
    
    while nThreadFlag>0:
        pass
