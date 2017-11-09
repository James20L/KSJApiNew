package com.catchbest;


import android.util.Log;

import java.nio.ByteBuffer;
import java.util.Objects;


/**
 * Created by terry on 17-9-14.
 */



public class cam {

    public int m_devicecount;


    public cam() {

        m_devicecount = 0;
    }


    public native int Init();

    public native int CaptureRawData(int index, Object buffer);

    public native int CaptureRGBData(int index, Object buffer);

    public native int DeviceGetCount();

    public native int CaptureGetSize(int index, int[] width, int[] height);

    public native int CaptureSetFieldOfView(int nIndex, int nxStart, int nyStart, int nWidth, int nHeight);

    public native int GetTriggerMode(int index);

    public native int SetTriggerMode(int index, int mode);

    public native int SetBayerMode(int index, int mode);

    public native int SoftStartCapture(int index);

    public native int ReadRawData(int index, Object buffer);

    public native int ReadRGBData(int index, Object buffer);

    public native byte[] CaptureRGBdataArray(int index, int width, int height);

    public native byte[] CaptureRAWdataArray(int index, int width, int height);


    public native int[] CaptureRGBdataIntArray(int index, int width, int height);


    public native int[] CaptureRGBdataIntArrayAfterStart(int index, int width, int height);


    public native int ExposureTimeSet(int index, int time);

    public native int WhiteBalanceSet(int index, int mode);


    static {
        System.loadLibrary("usb1.0");
        System.loadLibrary("ksjapijni");
    }

}
