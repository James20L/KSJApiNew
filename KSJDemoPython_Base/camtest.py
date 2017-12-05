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
import cv2.cv as cv
from ctypes import * 
"""
200w 1936 1216 
120w 1280 960
"""
g_nWidth = 1936
g_nHeight = 1216


nThreadFlag =1;

def KsjInit():
    libKsj = cdll.LoadLibrary('libksjapi.so')
    libKsj.KSJ_Init()
    camCount =libKsj.KSJ_DeviceGetCount()
    print camCount
    if camCount < 1:
        print "no cam fount"
    else:
        print "cam found"
    return libKsj,camCount

def CamParmSet(libKsj,num):
    global g_nWidth,g_nHeight
    for i in range(0,num):
        
        usDeviceType = c_int()
        nSerials= c_int()
        usFirmwareVersion = c_int()
  
        libKsj.KSJ_DeviceGetInformation(i,byref(usDeviceType),byref(nSerials),byref(usFirmwareVersion))  
        '''
        nSerials for distinguish camera
        '''
        print(usDeviceType)
        print(nSerials)
        print(usFirmwareVersion)
        

        libKsj.KSJ_CaptureSetFieldOfView(i,0,0,g_nWidth,g_nHeight,0,0)
        
        nColStart = c_int()
        nRowStart = c_int()
        nColSize = c_int()
        nRowSize = c_int()
        ColAddressMode = c_int()
        RowAddressMode = c_int() 
#        libKsj.KSJ_CaptureGetDefaultFieldOfView(i,byref(nColStart),byref(nRowStart),byref(nColSize),byref(nRowSize),byref(ColAddressMode),byref(RowAddressMode))        
        libKsj.KSJ_CaptureGetFieldOfView(i,byref(nColStart),byref(nRowStart),byref(nColSize),byref(nRowSize),byref(ColAddressMode),byref(RowAddressMode))        
        print  nColStart.value
        print  nRowStart.value
        print  nColSize.value
        print  nRowSize.value
        print  ColAddressMode.value
        print  RowAddressMode .value
        '''
        for set bayer mode ,this is relative to the filp do not change        
        231 set to 3
        120 set to 1        
        '''

        libKsj.KSJ_BayerSetMode(i, 3);
        '''
        for set whitbalance mode 7 stand for auto continus        
        '''

        libKsj.KSJ_WhiteBalanceSet(i,7);
	


#        fexpTime.value = 100.0              
        libKsj.KSJ_ExposureTimeSet.argtypes = (c_int,c_float)
        libKsj.KSJ_ExposureTimeSet(i,2.00);
        fexpTime = c_float()
        libKsj.KSJ_ExposureTimeGet(i,byref(fexpTime));
        print(fexpTime)
        print(fexpTime.value)

        '''
        for set gain ,do not change the 2nd parm, ,3rd parm is gain value     
        '''
        libKsj.KSJ_SetParam(i,1,30);
        '''
        for set the picture to flip ,3rd parm is the  value 0 or 1 ,do not change the 2nd parm  ,do not change     
        '''
        libKsj.KSJ_SetParam(i,11,1)
        '''
        for set the sensitivity  1 stand for high
      
        '''  
        libKsj.KSJ_SensitivitySetMode(i,1)
        '''
        for set color correction 3 is hardware present
        '''
        libKsj.KSJ_ColorCorrectionSet(i,3)
        

      

        


def CapturData(nIndex,cBuf,nHeight,nWidth,nChannelNum): 
    retValue = libKsj.KSJ_CaptureRgbData(nIndex,cBuf)
    if retValue != 0:
	print "capture error code %d"%(retValue)  
  
    nparr = np.fromstring(cBuf,np.uint8).reshape(nHeight,nWidth,nChannelNum );     
    return nparr
 
 
def QuitAll():
    global nThreadFlag
    print "exit"
    nThreadFlag=0;
    
def  CreateBuf(libKsj,num):
    buflist=[]
    nWidth = c_int()
    nHeight = c_int()
    nBitCount = c_int()
    for i in range(0,num) :
        libKsj.KSJ_CaptureGetSizeEx(i,byref(nWidth), byref(nHeight),byref(nBitCount))
        print     nWidth
        print     nHeight
        print     nBitCount
        nbufferSize = nWidth.value*nHeight.value*nBitCount.value/8
        pRawData = create_string_buffer(nbufferSize)
        buflist.append(pRawData)
    return buflist
          

def CapturDataLoop(nIndex,pDataBuf):
    print "cam loop nIndex = %d" %(nIndex)
    global nThreadFlag
    global g_nWidth,g_nHeight
    print "nThreadFlag = %d"%(nThreadFlag)
    nFrameCount =0
    nTimeStart = datetime.datetime.now() 
    nTimeStop = datetime.datetime.now()
  
    while nThreadFlag > 0:
       
	image =  CapturData(nIndex,pDataBuf,g_nHeight,g_nWidth,3)
        
	cv2.imshow("test",image)
    	cv2.waitKey(50)

        if nFrameCount == 0:
            nTimeStart = datetime.datetime.now() 
            
        if nFrameCount == 99:
            nTimeStop = datetime.datetime.now() 
            print "cam   %d " %(nIndex)
	    print (nTimeStop-nTimeStart)
            nFrameCount=-1
            
        nFrameCount=nFrameCount+1    
        time.sleep(0.001)
    print "thread quit"
#           

#face_cascade = cv2.CascadeClassifier('/home/terry/work/opencv/opencv-3.2.0/data/haarcascades/haarcascade_frontalface_default.xml')
#eye_cascade = cv2.CascadeClassifier('/home/terry/work/opencv/opencv-3.2.0/data/haarcascades/haarcascade_eye.xml')


if __name__ == '__main__':
    
    
    try:
        signal.signal(signal.SIGINT, QuitAll)
#        signal.signal(signal.SIGTERM, QuitAll)
        initRes=KsjInit()
        libKsj=initRes[0]
        camNub=initRes[1]
        print libKsj
        print camNub
        CamParmSet(libKsj,camNub)
        bufList=CreateBuf(libKsj,camNub)
        threadlist=[]
        for i in range(0,camNub):
    
            threadlist.append(threading.Thread(target=CapturDataLoop,args=(i,bufList[i])))     

#            threadlist[i].join()
            
            threadlist[i].setDaemon(True)
            threadlist[i].start()
    
        while nThreadFlag>0:
            pass
    except Exception, exc:
        print exc




        
     
     

    
        
    
    
    
    
    
    
    


#    nparr = np.fromstring(pRawData,np.int8).reshape(  nHeight.value,nWidth.value,3 );
#    nparr = np.fromstring(pRawData,np.uint8).reshape(  nHeight.value,nWidth.value,3 );
#    gray = cv2.cvtColor(nparr, cv2.COLOR_BGR2GRAY)
#    faces = face_cascade.detectMultiScale(gray, 1.3, 5)
#    for (x,y,w,h) in faces:
#        cv2.rectangle(gray,(x,y),(x+w,y+h),(255,0,0),2)
#        roi_gray = gray[y:y+h, x:x+w]
#        roi_color = gray[y:y+h, x:x+w]
#        eyes = eye_cascade.detectMultiScale(roi_gray)
#        for (ex,ey,ew,eh) in eyes:
#            cv2.rectangle(roi_color,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)    


#    cv2.imshow("test",gray)
#
#    cv2.waitKey(50)




#print type(nparr)
#
#
#
#print retValue
