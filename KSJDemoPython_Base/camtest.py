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
# g_nWidth = 2400
# g_nHeight = 2000
# g_nStartx = 348
# g_nStarty = 40

g_nWidth = 800
g_nHeight = 200
g_nStartx = 0
g_nStarty = 0

g_skipmode = 0
g_exptime = 2.5
g_gain = 32
g_sensitity = 1
g_lut = 0
g_aemin = 0.1
g_aemax = 30
g_aetarget = 120
g_show = 1;
g_ae = 0
g_map = 1
g_setserial = 0


g_trigmodeflag = 1
g_trigermode = 0



g_force_opencv_bayer = [0, 0, 0, 0];



nWidthArray = []
nHeightArray = []
g_mono = [];

g_recordflag = 0;
g_savebmpflag = 0



nThreadFlag = 1;

def KsjInit():
    libKsj = cdll.LoadLibrary('libksjapi.so')
    libKsj.KSJ_Init()
    camCount = libKsj.KSJ_DeviceGetCount()
    print(camCount)
    if camCount < 1:
        print("no cam fount")
    else:
        print("cam found")
    return libKsj, camCount


def WriteSerial(libksj, camnum, serialstart):
    serialchar = c_char()
    ret = 0
    for index in range(0, camnum):
        ret = libksj.KSJ_GetSerials(index, byref(serialchar))
        print(serialchar.value)
        serial = 'a'
        serial = serialstart + index
        ret = libksj.KSJ_SetSerials(index, serial)
        if ret != 0:
            print("error %d" % (ret))


def CamAeCallBack(bsuccess, nResult, lpContext):
    print("callback called")
    print(bsuccess)
    print(nResult)


def CamSetAe(libKsj, num):
    for i in range(0, num):
        nBitCount = c_int()
        nColSize = c_int()
        nRowSize = c_int()
        libKsj.KSJ_CaptureGetSizeEx(i, byref(nColSize), byref(nRowSize), byref(nBitCount))

        #        libKsj.KSJ_AESetRegion(i,0,0,g_nWidth,g_nheight)

        AeX = 500
        AeY = 0
        AeW = 1280
        AeH = 280
        libKsj.KSJ_AESetRegion(i, nColSize.value - AeX, nRowSize.value - AeY - AeH, AeW, AeH)

        libKsj.KSJ_AESetExposureTimeRange.argtypes = (c_int, c_float, c_float)
        libKsj.KSJ_AESetExposureTimeRange(i, g_aemin, g_aemax)
        CB_T = CFUNCTYPE(c_void_p, c_bool, c_int, c_void_p)
        print(dir())
        lpContext = c_void_p()
        libKsj.KSJ_AESetCallback(i, CB_T(CamAeCallBack), 0);
        libKsj.KSJ_AEStart.argtypes = (c_int, c_bool, c_int, c_int);

        libKsj.KSJ_AEStart(i, True, -1, g_aetarget)


def CamSetClib(libKsj, num):
    for i in range(0, num):
        nret = libKsj.KSJ_CaptureSetCalibration(i, 1);
        print("KSJ_CaptureSetCalibration  %d" % (nret))
        #        filename = "KSJMapCali.kmc"
        #        filename = "Map.cmf"
        #        nret = libKsj.KSJ_LoadCalibrationMapFile(i,filename.encode());
        print("KSJ_LoadCalibrationMapFile  %d" % (nret))


#        nret = libKsj.KSJ_SetCalibrationMapMode(i,0);
#        print("KSJ_SetCalibrationMapMode  %d"%(nret))


def CamParmSet(libKsj, num):
    global g_mono
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

        nColStart = c_int()
        nRowStart = c_int()
        nColSize = c_int()
        nRowSize = c_int()
        ColAddressMode = c_int()
        RowAddressMode = c_int()
        #        libKsj.KSJ_CaptureGetDefaultFieldOfView(i,byref(nColStart),byref(nRowStart),byref(nColSize),byref(nRowSize),byref(ColAddressMode),byref(RowAddressMode))
        libKsj.KSJ_CaptureGetFieldOfView(i, byref(nColStart), byref(nRowStart), byref(nColSize), byref(nRowSize),
                                         byref(ColAddressMode), byref(RowAddressMode))
        print(nColStart.value)
        print(nRowStart.value)
        print(nColSize.value)
        print(nRowSize.value)
        print(ColAddressMode.value)
        print(RowAddressMode.value)
        #        nret = libKsj.KSJ_CaptureSetFieldOfView(i,g_nStartx,g_nStarty,g_nWidth,g_nHeight,g_skipmode,g_skipmode)

        #        nret = libKsj.KSJ_CaptureSetFieldOfView(i,nColStart.value,nRowStart.value,nColSize.value,nRowSize.value,0,0)

        #        libKsj.KSJ_EmptyFrameBuffer(i)

        #        nret = libKsj.KSJ_CaptureSetFieldOfView(i,nColStart.value,nRowStart.value,nColSize.value,nRowSize.value,1,1)

        nret = libKsj.KSJ_CaptureGetFieldOfView(i, byref(nColStart), byref(nRowStart), byref(nColSize), byref(nRowSize),
                                                byref(ColAddressMode), byref(RowAddressMode))
        print(nColStart.value)
        print(nRowStart.value)
        print(nColSize.value)
        print(nRowSize.value)
        print(ColAddressMode.value)
        print(RowAddressMode.value)

        nBitCount = c_int()
        libKsj.KSJ_CaptureGetSizeEx(i, byref(nColSize), byref(nRowSize), byref(nBitCount))
        nWidthArray.append(nColSize.value)
        nHeightArray.append(nRowSize.value)
        print("KSJ_CaptureSetFieldOfView nret = %d" % (nret))

        # libKsj.KSJ_SetParam(i, 1, g_gain);
        # libKsj.KSJ_SetParam(i, 2, g_gain);
        # libKsj.KSJ_SetParam(i, 3, g_gain);

        if g_trigmodeflag == 1:

            # ret = libKsj.KSJ_TriggerModeSet(i, g_trigermode)

            trigermode = c_int();
            ret = libKsj.KSJ_TriggerModeGet(i, byref(trigermode))
            print("trigermode =  {0}".format(trigermode.value))

            # ret = libKsj.KSJ_TriggerModeSet(i, g_trigermode)


            if ret != 0:
                print("error KSJ_TriggerModeSet %d" % (ret))

            libKsj.KSJ_SetFixedFrameRateEx.argtypes = (c_int, c_float)

            ret = libKsj.KSJ_SetFixedFrameRateEx(i, 10.0)

            if ret != 0:
                print("error KSJ_SetFixedFrameRateEx %d" % (ret))


        mono = c_int()

        libKsj.KSJ_QueryFunction(i, 0, byref(mono));

        if mono.value == 0:
            g_mono.append(0);
            bayermode = c_int();
            libKsj.KSJ_BayerGetMode(i, byref(bayermode));
            # libKsj.KSJ_BayerSetMode(i, bayermode.value + 4);
            libKsj.KSJ_BayerSetMode(i, bayermode.value);


            '''
            for set whitbalance mode 7 stand for auto continus
            '''

            libKsj.KSJ_WhiteBalanceSet(i, 7);
            #            libKsj.KSJ_WhiteBalanceSet(i,5);
            #            libKsj.KSJ_WhiteBalancePresettingSet(i,0)

            '''
            for set color correction 3 is hardware present
            '''
            libKsj.KSJ_ColorCorrectionSet(i, 3)
        #            libKsj.KSJ_ColorCorrectionPresettingSet(i,0)
        else:
            g_mono.append(1);

        print("g_mono is ")
        print(g_mono)

        #        fexpTime.value = 100.0
        libKsj.KSJ_ExposureTimeSet.argtypes = (c_int, c_float)
        libKsj.KSJ_ExposureTimeSet(i, g_exptime);
        fexpTime = c_float()
        libKsj.KSJ_ExposureTimeGet(i, byref(fexpTime));
        print("fexpTime = %d" % (fexpTime.value))

        '''
        for set the sensitivity  1 stand for high
      
        '''
        libKsj.KSJ_SensitivitySetMode(i, g_sensitity)
        libKsj.KSJ_LutSetEnable(i, g_lut)



def check_buf_data(buf):
    #    print(type(buf))
    #    print(dir(buf))
    for bytesindex in range(0, len(buf)):
        if int.from_bytes(buf[bytesindex], byteorder='little') < 127:
            buf[bytesindex] = 0
            print(bytesindex)
            break


#    import pdb;pdb.set_trace()


def CapturData(nIndex, cBuf, nHeight, nWidth, nChannelNum):
    if nChannelNum == 1:
        retValue = libKsj.KSJ_CaptureRawData(nIndex, cBuf)

    if nChannelNum > 1:
        retValue = libKsj.KSJ_CaptureRgbData(nIndex, cBuf)

    if retValue != 0:
        print("capture error code %d" % (retValue))
    #    check_buf_data(cBuf)

    #    exptime = c_float()
    #    libKsj.KSJ_ExposureTimeGet(nIndex,byref(exptime))

    #    print("exptime = %f"%(exptime.value))

    nparr = np.fromstring(cBuf, np.uint8).reshape(nHeight, nWidth, nChannelNum);
    #    nparr = np.fromstring(cBuf,np.uint8).reshape(nWidth,nHeight,nChannelNum );

    return nparr


def QuitAll():
    global nThreadFlagd
    print("exit")
    nThreadFlag = 0;


def CreateBuf(libKsj, num):
    global g_mono
    buflist = []
    nWidth = c_int()
    nHeight = c_int()
    nBitCount = c_int()
    for i in range(0, num):
        libKsj.KSJ_CaptureGetSizeEx(i, byref(nWidth), byref(nHeight), byref(nBitCount))
        print(nWidth)
        print(nHeight)
        print(nBitCount)
        if g_mono[i] == 0:
            nbufferSize = nWidth.value * nHeight.value * nBitCount.value / 8
        else:
            nbufferSize = nWidth.value * nHeight.value
        if g_force_opencv_bayer[i] == 1:
            nbufferSize = nWidth.value * nHeight.value
        pRawData = create_string_buffer(int(nbufferSize))
        buflist.append(pRawData)
    return buflist


def CapturDataLoop(nIndex, pDataBuf, nWidth, nHeight):


    global g_recordflag
    global nThreadFlag
    global g_mono
    print("cam loop nIndex = %d" % (nIndex))
    print("nThreadFlag = %d" % (nThreadFlag))
    nFrameCount = 0
    nTimeStart = datetime.datetime.now()
    nTimeStop = datetime.datetime.now()
    nxxWidth = c_int()
    nxxHeight = c_int()
    nBitCount = c_int()

    libKsj.KSJ_CaptureGetSizeEx(nIndex, byref(nxxWidth), byref(nxxHeight), byref(nBitCount))

    usDeviceType = c_int()
    nSerials = c_int()
    usFirmwareVersion = c_int()


    libKsj.KSJ_DeviceGetInformation(nIndex, byref(usDeviceType), byref(nSerials), byref(usFirmwareVersion))
    '''
    nSerials for distinguish camera
    '''
    print(usDeviceType)
    print(nSerials)
    print(usFirmwareVersion)
    libKsj.KSJ_SetParam(0, 13, 1);


    ts = time.time()
    st = datetime.datetime.fromtimestamp(ts).strftime('%Y%m%d%H%M%S')
    avi_path = './avi/'
    vwriter = cv2.VideoWriter_fourcc('D', 'I', 'V', 'X')
    vwriter1 = cv2.VideoWriter(avi_path + st + '.avi', vwriter, 10.0, (1280, 720))


    if g_mono[nIndex] == 1:
        channelnum = 1;
    else:
        channelnum = nBitCount.value / 8

    if g_force_opencv_bayer[nIndex] == 1:
        channelnum = 1;



    print("g_mono is ")
    print(g_mono)

    imagecolor = np.zeros((nHeight, nWidth, 3), dtype=np.uint8)


    while nThreadFlag > 0:

        image = CapturData(nIndex, pDataBuf, nHeight, nWidth, int(channelnum))
        if g_show == 1:
            if nWidth > 1920:
                image = cv2.resize(image, (int(nWidth / 2), int(nHeight / 2)))
                imagecolor = cv2.resize(imagecolor, (int(nWidth / 2), int(nHeight / 2)))
            #            image = cv2.cvtColor(image,cv2.COLOR_BGR2RGB)
            if g_force_opencv_bayer[nIndex] == 1:
                cv2.cvtColor(image, cv2.COLOR_BayerGR2RGB, imagecolor, 3)

            if g_force_opencv_bayer[nIndex] == 1:
                cv2.namedWindow("test raw" + str(nIndex), cv2.WINDOW_AUTOSIZE)
                cv2.imshow("test raw" + str(nIndex), imagecolor)
            else:
                cv2.namedWindow("test" + str(nIndex), cv2.WINDOW_AUTOSIZE)
                cv2.imshow("test" + str(nIndex), image)

            cv2.waitKey(1)

        if g_recordflag == 1:
            vwriter1.write(image)

        if g_savebmpflag == 1:
            cv2.imwrite(str(nFrameCount)+str(nIndex)+".bmp",image)



        
        if nFrameCount == 0:
            nTimeStart = datetime.datetime.now()

        if nFrameCount == 99:
            nTimeStop = datetime.datetime.now()
            deltt= (nTimeStop - nTimeStart)

            print("cam   %d  type %d  nSerials %d  fps  %f" % (nIndex, usDeviceType.value, nSerials.value, 100/(deltt.total_seconds())))

            g_recordflag = 0
            vwriter1.release()

            nFrameCount = -1

        nFrameCount = nFrameCount + 1
        time.sleep(0.001)
    print("thread quit")


#

# face_cascade = cv2.CascadeClassifier('/home/terry/work/opencv/opencv-3.2.0/data/haarcascades/haarcascade_frontalface_default.xml')
# eye_cascade = cv2.CascadeClassifier('/home/terry/work/opencv/opencv-3.2.0/data/haarcascades/haarcascade_eye.xml')


if __name__ == '__main__':

    try:
        signal.signal(signal.SIGINT, QuitAll)
        #        signal.signal(signal.SIGTERM, QuitAll)
        initRes = KsjInit()
        libKsj = initRes[0]
        camNub = initRes[1]



        print(libKsj)
        print(camNub)
        CamParmSet(libKsj, camNub)
        #    cam_parm_set(libKsj,camNub)

        if g_ae == 1:
            CamSetAe(libKsj, camNub)
        if g_map == 1:
            CamSetClib(libKsj, camNub)
        if g_setserial == 1 :
            WriteSerial(libKsj, camNub, 5)


        bufList = CreateBuf(libKsj, camNub)
        threadlist = []
        #        import pdb;pdb.set_trace()
        for i in range(0, camNub):
            threadlist.append(
                threading.Thread(target=CapturDataLoop, args=(i, bufList[i], nWidthArray[i], nHeightArray[i])))
            threadlist[i].setDaemon(True)
            threadlist[i].start()

        while nThreadFlag > 0:
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


# print type(nparr)
#
#
#
# print retValue
