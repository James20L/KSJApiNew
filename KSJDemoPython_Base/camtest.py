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
"""
200w 1936 1216 
120w 1280 960
"""
#g_nWidth = 2400
#g_nHeight = 2000
#g_nStartx = 348
#g_nStarty = 40

g_nWidth = 800
g_nHeight = 200
g_nStartx = 0
g_nStarty = 0

g_skipmode  = 0
g_exptime = 20
g_gain = 100
g_sensitity = 2
g_lut = 1
g_aemin = 0.1
g_aemax = 30
g_aetarget =120
g_show  = 1;
g_ae = 0
g_map = 1



nWidthArray = []
nHeightArray = []
g_mono = [];
nThreadFlag =1;


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

def CamAeCallBack(bsuccess,nResult,lpContext):
    print("callback called")
    print(bsuccess)
    print(nResult)
def CamSetAe(libKsj,num):
    for i in range(0,num):
        nBitCount = c_int()
        nColSize  = c_int()
        nRowSize  = c_int()
        libKsj.KSJ_CaptureGetSizeEx(i,byref(nColSize), byref(nRowSize),byref(nBitCount))

#        libKsj.KSJ_AESetRegion(i,0,0,g_nWidth,g_nheight)

        AeX = 500
        AeY = 0
        AeW = 1280
        AeH = 280
        libKsj.KSJ_AESetRegion(i,nColSize.value-AeX,nRowSize.value - AeY- AeH,AeW,AeH)

        libKsj.KSJ_AESetExposureTimeRange.argtypes = (c_int,c_float,c_float)
        libKsj.KSJ_AESetExposureTimeRange(i,g_aemin,g_aemax)
        CB_T=CFUNCTYPE(c_void_p,c_bool, c_int,c_void_p)
        print(dir())
        lpContext = c_void_p()
        libKsj.KSJ_AESetCallback(i,CB_T(CamAeCallBack),0);
        libKsj.KSJ_AEStart.argtypes = (c_int, c_bool, c_int ,c_int);

        libKsj.KSJ_AEStart(i,True,-1,g_aetarget)
        
def CamSetClib(libKsj,num):
    for i in range(0,num):
        nret = libKsj.KSJ_CaptureSetCalibration(i,1);
        print("KSJ_CaptureSetCalibration  %d"%(nret))
        filename = "Map.cmf"
        nret = libKsj.KSJ_LoadCalibrationMapFile(i,filename.encode());
        print("KSJ_LoadCalibrationMapFile  %d"%(nret))
        nret = libKsj.KSJ_SetCalibrationMapMode(i,0);
        print("KSJ_SetCalibrationMapMode  %d"%(nret))


def CamParmSet(libKsj,num):

    global g_mono
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
        
        nColStart = c_int()
        nRowStart = c_int()
        nColSize = c_int()
        nRowSize = c_int()
        ColAddressMode = c_int()
        RowAddressMode = c_int()
#        libKsj.KSJ_CaptureGetDefaultFieldOfView(i,byref(nColStart),byref(nRowStart),byref(nColSize),byref(nRowSize),byref(ColAddressMode),byref(RowAddressMode))
        libKsj.KSJ_CaptureGetFieldOfView(i,byref(nColStart),byref(nRowStart),byref(nColSize),byref(nRowSize),byref(ColAddressMode),byref(RowAddressMode))
        print(nColStart.value)
        print(nRowStart.value)
        print(nColSize.value)
        print(nRowSize.value)
        print(ColAddressMode.value)
        print(RowAddressMode .value)
#        nret = libKsj.KSJ_CaptureSetFieldOfView(i,g_nStartx,g_nStarty,g_nWidth,g_nHeight,g_skipmode,g_skipmode)

#        nret = libKsj.KSJ_CaptureSetFieldOfView(i,nColStart.value,nRowStart.value,nColSize.value,nRowSize.value,0,0)

#        libKsj.KSJ_EmptyFrameBuffer(i)


#        nret = libKsj.KSJ_CaptureSetFieldOfView(i,nColStart.value,nRowStart.value,nColSize.value,nRowSize.value,1,1)

        nret = libKsj.KSJ_CaptureGetFieldOfView(i,byref(nColStart),byref(nRowStart),byref(nColSize),byref(nRowSize),byref(ColAddressMode),byref(RowAddressMode))
        print(nColStart.value)
        print(nRowStart.value)
        print(nColSize.value)
        print(nRowSize.value)
        print(ColAddressMode.value)
        print(RowAddressMode .value)

        nBitCount = c_int()
        libKsj.KSJ_CaptureGetSizeEx(i,byref(nColSize), byref(nRowSize),byref(nBitCount))
#        import pdb;pdb.set_trace()
        nWidthArray.append(nColSize.value)
        nHeightArray.append(nRowSize.value)
        print("KSJ_CaptureSetFieldOfView nret = %d"%(nret))
        mono = c_int()

        libKsj.KSJ_QueryFunction(i, 0, byref(mono));

        if mono.value == 0:
            g_mono.append(0);
            bayermode = c_int();
            libKsj.KSJ_BayerGetMode(i, byref(bayermode));
            libKsj.KSJ_BayerSetMode(i, bayermode.value+4);

            '''
            for set whitbalance mode 7 stand for auto continus
            '''

            libKsj.KSJ_WhiteBalanceSet(i,7);
#            libKsj.KSJ_WhiteBalanceSet(i,5);
#            libKsj.KSJ_WhiteBalancePresettingSet(i,0)

            '''
            for set color correction 3 is hardware present
            '''
            libKsj.KSJ_ColorCorrectionSet(i,3)
#            libKsj.KSJ_ColorCorrectionPresettingSet(i,0)
        else:
            g_mono.append(1);

        print("g_mono is ")
        print(g_mono)



#    if g_map == 1 :
#
#
#        nret = libKsj.KSJ_CaptureSetCalibration(i,1);
#        print("KSJ_CaptureSetCalibration  %d"%(nret))
#
#        filename = "Map.cmf"
#
#        nret = libKsj.KSJ_LoadCalibrationMapFile(i,filename.encode());
#
#        print("KSJ_LoadCalibrationMapFile  %d"%(nret))
#        nret = libKsj.KSJ_SetCalibrationMapMode(i,0);
#        print("KSJ_SetCalibrationMapMode  %d"%(nret))

 #       libKsj.KSJ_CaptureSetFieldOfView(i,0,0,g_nWidth,g_nHeight,0,0)
        


        '''
        for set ,mirror ,do not change the 2nd parm, ,3rd parm is gain value     
        '''
#        libKsj.KSJ_SetParam(i,13,1);
        
        '''
        for set flip ,do not change the 2nd parm, ,3rd parm is gain value     
        '''
 #       libKsj.KSJ_SetParam(i,11,1);

        '''
        for set bayer mode ,this is relative to the filp do not change
        231 set to 3
        120 set to 1        
        '''



#        fexpTime.value = 100.0              
        libKsj.KSJ_ExposureTimeSet.argtypes = (c_int,c_float)
        libKsj.KSJ_ExposureTimeSet(i,g_exptime);
        fexpTime = c_float()
        libKsj.KSJ_ExposureTimeGet(i,byref(fexpTime));
        print("fexpTime = %d"%(fexpTime.value))



        '''
        for set the sensitivity  1 stand for high
      
        '''  
        libKsj.KSJ_SensitivitySetMode(i,g_sensitity)
        libKsj.KSJ_LutSetEnable(i,g_lut)

def check_buf_data(buf):
#    print(type(buf))
#    print(dir(buf))
    for bytesindex in range(0,len(buf)):
        if int.from_bytes(buf[bytesindex],byteorder='little') <127:
            buf[bytesindex]=0
            print(bytesindex)
            break

#    import pdb;pdb.set_trace()



def CapturData(nIndex,cBuf,nHeight,nWidth,nChannelNum): 
    if nChannelNum==1:
        retValue = libKsj.KSJ_CaptureRawData(nIndex,cBuf)
    if  nChannelNum>1:
        retValue = libKsj.KSJ_CaptureRgbData(nIndex,cBuf)
    if retValue != 0:
        print("capture error code %d"%(retValue))
#    check_buf_data(cBuf)


#    exptime = c_float()
#    libKsj.KSJ_ExposureTimeGet(nIndex,byref(exptime))

#    print("exptime = %f"%(exptime.value))
  
    nparr = np.fromstring(cBuf,np.uint8).reshape(nHeight,nWidth,nChannelNum ); 
#    nparr = np.fromstring(cBuf,np.uint8).reshape(nWidth,nHeight,nChannelNum );     
       
    return nparr
 
 
 
def QuitAll():
    global nThreadFlagd
    print("exit")
    nThreadFlag=0;
    
def  CreateBuf(libKsj,num):
    global g_mono
    buflist=[]
    nWidth = c_int()
    nHeight = c_int()
    nBitCount = c_int()
    for i in range(0,num) :
        libKsj.KSJ_CaptureGetSizeEx(i,byref(nWidth), byref(nHeight),byref(nBitCount))
        print(nWidth)
        print(nHeight)
        print(nBitCount)
        if g_mono[i] == 0 :
            nbufferSize = nWidth.value*nHeight.value*nBitCount.value/8
        else:
            nbufferSize = nWidth.value*nHeight.value
#        import pdb;pdb.set_trace()
        pRawData = create_string_buffer(int(nbufferSize))
        buflist.append(pRawData)
    return buflist
          

def CapturDataLoop(nIndex,pDataBuf,nWidth,nHeight):
    print("cam loop nIndex = %d" %(nIndex))
    global nThreadFlag
    global g_mono
    print("nThreadFlag = %d"%(nThreadFlag))
    nFrameCount =0
    nTimeStart = datetime.datetime.now() 
    nTimeStop = datetime.datetime.now()
    nxxWidth = c_int()
    nxxHeight = c_int()
    nBitCount = c_int()

    libKsj.KSJ_CaptureGetSizeEx(i,byref(nxxWidth), byref(nxxHeight),byref(nBitCount))
  
    if g_mono[nIndex]==1:
        channelnum =1;
    else:
        channelnum = nBitCount.value/8


    print("g_mono is ")
    print(g_mono)

    while nThreadFlag > 0:
       
        image =  CapturData(nIndex,pDataBuf,nHeight,nWidth,int(channelnum))
        if g_show == 1:
            if nWidth > 1920:
                image=cv2.resize(image,(int(nWidth/2),int(nHeight/2) ))
            cv2.imshow("test"+str(nIndex),image)
            cv2.waitKey(1)

        if nFrameCount == 0:
            nTimeStart = datetime.datetime.now() 
            
        if nFrameCount == 99:
            nTimeStop = datetime.datetime.now() 
            print ("cam   %d " %(nIndex))
            print (nTimeStop-nTimeStart)
            nFrameCount=-1
            
        nFrameCount=nFrameCount+1    
        time.sleep(0.001)
    print("thread quit")
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
        print(libKsj)
        print(camNub)
        CamParmSet(libKsj,camNub)
        if g_ae == 1:
            CamSetAe(libKsj,camNub)
        if g_map == 1:          
            CamSetClib(libKsj,camNub)

        bufList=CreateBuf(libKsj,camNub)
        threadlist=[]
#        import pdb;pdb.set_trace()
        for i in range(0,camNub):   
            threadlist.append(threading.Thread(target=CapturDataLoop,args=(i,bufList[i],nWidthArray[i],nHeightArray[i])))                 
            threadlist[i].setDaemon(True)
            threadlist[i].start()
    
        while nThreadFlag>0:
            pass
    except Exception as exc:
        print(exc)




        
     
     

    
        
    
    
    
    
    
    
    


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
