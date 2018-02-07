package com.phone.fuxi.catchbest;

import android.Manifest;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Bitmap;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Button;
import android.widget.ImageView;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Iterator;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import com.catchbest.KSJ_BAYERMODE;
import com.catchbest.KSJ_PARAM;
import com.catchbest.KSJ_TRIGGRMODE;
import com.catchbest.KSJ_WB_MODE;
import com.catchbest.cam;

import static com.catchbest.KSJ_PARAM.KSJ_BLUE;
import static com.catchbest.KSJ_PARAM.KSJ_GREEN;
import static com.catchbest.KSJ_PARAM.KSJ_RED;
import static java.lang.Thread.sleep;









public class MainActivity extends AppCompatActivity  implements  SurfaceHolder.Callback {


    public static boolean upgradeRootPermission(String pkgCodePath) {
        Process process = null;
        DataOutputStream os = null;
        try {
            String cmd="chmod -R 777 " + pkgCodePath ;
            process = Runtime.getRuntime().exec("su"); //切换到root帐号
            os = new DataOutputStream(process.getOutputStream());
            os.writeBytes(cmd + "\n");
            os.writeBytes("exit\n");
            os.flush();
            process.waitFor();
        } catch (Exception e) {
            return false;
        } finally {
            try {
                if (os != null) {
                    os.close();
                }
                process.destroy();
            } catch (Exception e) {
            }
        }
        return true;
    }

    Context mContext = this;
    BroadcastReceiver mUsbReceiver;

    public ByteBuffer[] m_receiverbuffer;


    private SurfaceHolder m_PreviewHolder;

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public int m_nwidth;
    public int m_nheight;


    public ImageView[] m_imgvw;

    cam ksjcam;
    Lock m_Lock;

    public int prepare_imagevw() {
        m_imgvw = new ImageView[2];
        m_imgvw[0] = (ImageView) findViewById(R.id.imageView);
        m_imgvw[1] = (ImageView) findViewById(R.id.imageView1);

        return 0;

    }

    boolean captureThreadGo = false;
    Handler hd = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
        }
    };

    @Override
    protected void onPause() {
        captureThreadGo = false;
        super.onPause();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (data != null) {

            int ShutterWidth = data.getExtras().getInt("ShutterWidth", 0);

//        Toast debugToast = Toast.makeText(getApplicationContext(),"ShutterWidth  == "+ShutterWidth, Toast.LENGTH_LONG);
//        debugToast.show();

            int Green2Gain = data.getExtras().getInt("Green2Gain", 0);

            int RedGain = data.getExtras().getInt("RedGain", 0);

            int BlueGain = data.getExtras().getInt("BlueGain", 0);


            Toast debugToast3 = Toast.makeText(getApplicationContext(), "BlueGain  == " + ShutterWidth, Toast.LENGTH_LONG);
            debugToast3.show();



//            ca.setBlueGain(BlueGain);
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        toolbar.setOnMenuItemClickListener(new Toolbar.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem item) {
                Toast debugToast = Toast.makeText(getApplicationContext(), "toolbar clicked", Toast.LENGTH_LONG);
                debugToast.show();

                Intent st = new Intent(getApplicationContext(), Main2Activity.class);

                startActivityForResult(st, 1);

                return false;
            }
        });

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {


                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();


            }
        });



//        upgradeRootPermission("/dev/bus/usb");

        SurfaceView Preview = (SurfaceView)findViewById(R.id.surfaceView);
        m_PreviewHolder = Preview.getHolder();
        m_PreviewHolder.addCallback(this);



        prepare_imagevw();

        int[] widtharray = new int[1];
        int[] heightarray=new int[1];

        int[] DeviceTypeArray = new int[1];
        int[] SerialsArray=new int[1];
        int[] FirmwareVersionArray = new int[1];
        ;

        Log.e("zhanwei", "UsbManager openDevice start");
        UsbManager manager = (UsbManager) mContext.getSystemService(Context.USB_SERVICE);
        final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";

        HashMap<String, UsbDevice> deviceList = manager.getDeviceList();
        Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();

        PendingIntent mPermissionIntent = PendingIntent.getBroadcast(mContext, 0,
                new Intent(ACTION_USB_PERMISSION), 0);
        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        mContext.registerReceiver(mUsbReceiver, filter);

        int fd = -1;
        while(deviceIterator.hasNext()){
            UsbDevice device = deviceIterator.next();

            Log.i("zhanwei", device.getDeviceName() + " " + Integer.toHexString(device.getVendorId()) +
                    " " + Integer.toHexString(device.getProductId()));

            manager.requestPermission(device, mPermissionIntent);
            UsbDeviceConnection connection = manager.openDevice(device);
            if(connection != null){
                fd = connection.getFileDescriptor();
            } else
                Log.e("zhanwei", "UsbManager openDevice failed");
            break;
        }




        ksjcam = new cam();

        if(fd <= 0) return;

        ksjcam.PreInit(fd);
        Log.e("zhanwei", "PreInit");


        Toast debugToast = Toast.makeText(getApplicationContext(), "fd =      "+String.valueOf(fd), Toast.LENGTH_LONG);
        debugToast.show();

        if(fd>0) {

            Log.e("zhanwei", "before ksjcam.Init()  = " + String.valueOf(fd));
            int  ret = ksjcam.Init();
            Log.e("zhanwei", "after ksjcam.Init()  = " + String.valueOf(fd));

            {
                debugToast = Toast.makeText(getApplicationContext(), "   ksjcam.Init() ret =      " + String.valueOf(ret), Toast.LENGTH_SHORT);
                debugToast.show();
            }


            Log.e("zhanwei", "ksjcam.m_devicecount = " + String.valueOf(ksjcam.m_devicecount));
            {
                debugToast = Toast.makeText(getApplicationContext(), "   ksjcam.m_devicecount =      " + String.valueOf(ksjcam.m_devicecount), Toast.LENGTH_SHORT);
                debugToast.show();
            }

            m_Lock  = new ReentrantLock();
            Log.e("zhanwei", "ksjcam.m_devicecount = " + String.valueOf(ksjcam.m_devicecount));
            {
                debugToast = Toast.makeText(getApplicationContext(), "   ksjcam.m_devicecount =      " + String.valueOf(ksjcam.m_devicecount), Toast.LENGTH_SHORT);
                debugToast.show();
            }


            ksjcam.m_devicecount = ksjcam.DeviceGetCount();

            Log.e("zhanwei", "ksjcam.m_devicecount = " + String.valueOf(ksjcam.m_devicecount));
            {
                debugToast = Toast.makeText(getApplicationContext(), "   ksjcam.m_devicecount =      " + String.valueOf(ksjcam.m_devicecount), Toast.LENGTH_LONG);
                debugToast.show();
            }
            for (int i = 0; i < ksjcam.m_devicecount; i++) {

                ksjcam.CaptureSetFieldOfView(i, 0, 0, 1280, 960);

                ksjcam.SetBayerMode(i,15);


                ksjcam.SetParam(i, KSJ_RED.ordinal(),80);
                ksjcam.SetParam(i, KSJ_GREEN.ordinal(),80);
                ksjcam.SetParam(i, KSJ_BLUE.ordinal(),80);


                ksjcam.DeviceGetInformation(i,DeviceTypeArray,SerialsArray,FirmwareVersionArray);
                int nSerial = SerialsArray[0];
                Log.e("zhanwei", "nSerial = " + String.valueOf(nSerial)+"   index = "+ String.valueOf(i));

                ksjcam.CaptureGetSize(i, widtharray, heightarray);
                ksjcam.LutSetEnable(i,0);

                ksjcam.SetTriggerMode(i, KSJ_TRIGGRMODE.KSJ_TRIGGER_SOFTWARE.ordinal());

//            ksjcam.WhiteBalanceSet(i, KSJ_WB_MODE.KSJ_HWB_AUTO_CONITNUOUS.ordinal());
                ksjcam.WhiteBalanceSet(i, KSJ_WB_MODE.KSJ_HWB_PRESETTINGS.ordinal());

                ksjcam.ExposureTimeSet(i, 20);

                m_nwidth = widtharray[0];
                m_nheight = heightarray[0];

                Log.e("zhanwei", "m_nwidth = " + String.valueOf(m_nwidth));
                Log.e("zhanwei", "m_nheight = " + String.valueOf(m_nheight));
            }


            try {
                sleep(300);
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
//        ca.setShutterWidth(100);
            try {
                sleep(300);
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }



        }


        Button btcapture = (Button) findViewById(R.id.capture_bitmap);

        btcapture.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {


                m_Lock.lock();

//                ksjcam.SetBayerMode(0,19);

                ksjcam.SetBayerMode(0, KSJ_BAYERMODE.KSJ_GBRG_GRAY8.ordinal());

                ksjcam.CaptureBitmap(0,"/sdcard/bitmapcaptured.bmp");

//                ksjcam.SetBayerMode(0,15);


                m_Lock.unlock();

                Toast debugToast = Toast.makeText(getApplicationContext(), "captured /sdcard/bitmapcaptured.bmp", Toast.LENGTH_LONG);

                debugToast.show();


            }
        });




        Button bt = (Button) findViewById(R.id.button_starstop);
        bt.setOnClickListener(new View.OnClickListener() {

                                  @Override
                                  public void onClick(View v) {

                                      {
                                          if (captureThreadGo == true) {

                                              captureThreadGo = false;


                                              return;

                                          }
                                          captureThreadGo = true;
                                          startCaptureThread(ksjcam.m_devicecount, m_nwidth, m_nheight); //130W
//
//                                          String[] mypermis = new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE};
//                                          if(PermissionHelper.checkPermission(MainActivity.this, mypermis)) {

//                                          }
                                      }
                                  }
                              }
        );
    }


    public void startCaptureThread(int camnum, final int width, final int height) {
        {
            Toast debugToast = Toast.makeText(getApplicationContext(), "   ksjcam.m_devicecount =      " + String.valueOf(ksjcam.m_devicecount)   +"width "+String.valueOf(width)+String.valueOf(ksjcam.m_devicecount)   +"height "+String.valueOf(height), Toast.LENGTH_SHORT);
            debugToast.show();
        }

        if(0 == camnum) return;

        Toast debugToast = Toast.makeText(getApplicationContext(), "startCaptureThread", Toast.LENGTH_LONG);
        debugToast.show();
        Thread captureThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (captureThreadGo) {
//                    captureAndshow_TwoStep(0,width,height);
                    Log.d("zhanwei", "before captureAndshow_Capture");


                    if(m_PreviewHolder.getSurface().isValid()) {
                        m_Lock.lock();

                        ksjcam.CaptureBySurface(0, m_PreviewHolder.getSurface(), 0); // 0 no save 1 save bmp at /sdcard/

//                    ksjcam.CaptureBySurfaceSave(0,m_PreviewHolder.getSurface(),1,"/dev/capture.bmp");

                        m_Lock.unlock();
//                    captureAndshow_Capture(0, width, height);
                    }
                }

            }

        });
        Log.d("zhanwei", "startCaptureThread 0");
        captureThread.start();


        if (camnum == 2) {
            Thread captureThread1 = new Thread(new Runnable() {
                @Override
                public void run() {


                    while (captureThreadGo) {
                        //                      captureAndshow_TwoStep(1, width, height);
                        captureAndshow_Capture(1,width,height);


                    }

                }

            });
            captureThread1.start();
        }
    }

    public int captureAndshow_TwoStep(final int index, int width, int height) {
        int ret = ksjcam.SoftStartCapture(index);//trigger

        int[] dataarray1 = ksjcam.CaptureRGBdataIntArrayAfterStart(index, width, height);//read

        if(dataarray1!=null  && dataarray1.length>0) {
            final Bitmap bmp = CreateBitmap_from_int_rgba_data(width, height, dataarray1);

            hd.post(new Runnable() {
                @Override
                public void run() {
                    m_imgvw[index].setImageBitmap(bmp);

                }
            });
        }else
        {
            Log.e("zhanwei", "read date failed");
        }
        return 0;

    }

    public int captureAndshow_Capture(final int index, int width, int height) {
        int[] dataarray = ksjcam.CaptureRGBdataIntArray(index, width, height);//trigger

        if(dataarray!=null  && dataarray.length>0) {

            final Bitmap bmp = CreateBitmap_from_int_rgba_data(width, height, dataarray);


            hd.post(new Runnable() {
                @Override
                public void run() {
                    m_imgvw[index].setImageBitmap(bmp);

                }
            });

        }else
        {

            Log.e("zhanwei", "read date failed");
        }


        return 0;

    }


    public String saveBitmapToSDCard(Bitmap bitmap, String imagename) {
        String path = "/sdcard/" + "img-" + imagename + ".PNG";
        //      String path = "img-" + imagename + ".jpg";
        Log.d("zhanwei", "saveBitmapToSDCard1      " + bitmap.toString());
        Log.d("zhanwei", "saveBitmapToSDCard1 getByteCount     " + String.valueOf(bitmap.getByteCount()));

        FileOutputStream fos = null;
        try {
            fos = new FileOutputStream(path);
            if (fos != null) {

                Log.d("zhanwei", "saveBitmapToSDCard1");

                boolean result;
                result = bitmap.compress(Bitmap.CompressFormat.PNG, 90, fos);
                Log.d("zhanwei", "saveBitmapToSDCard1 result    " + String.valueOf(result));
                fos.flush();
                fos.close();
                Log.d("zhanwei", "saveBitmapToSDCard2");
            }

            return path;
        } catch (Exception e) {
            e.printStackTrace();
        }

        return null;
    }

    //not varifyed
    public void mySaveFile(String name, byte[] buf) {
        String filename = "myfile";
        FileOutputStream outputStream;
        try {
            outputStream = openFileOutput(filename, Context.MODE_PRIVATE);
            outputStream.write(buf);
            outputStream.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    public Bitmap myCreateBitmap(int width, int height, byte[] buf) {
        int w = width;//1280;
        int h = height;//1024;
        int[] pixels = new int[w * h];
        Bitmap bmp = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);

        for (int i = 0; i < w; ++i) {
            for (int j = 0; j < h; ++j) {
//                            int c=getPixel(buf, 1, i, j, w, h);
                int c = buf[i + j * w];
                pixels[i + j * w] = (255 << 24) + (c << 16) + (c << 8) + c;
            }
        }

        bmp.setPixels(pixels, 0, w, 0, 0, w, h);

        return bmp;
    }

    //a r g b high to low
    public static int graybyteArrayToInt(byte[] b, int pos) {
        return (b[pos] & 0xFF) |
                (b[pos] & 0xFF) << 8 |
                (b[pos] & 0xFF) << 16 |
                (255 & 0xFF) << 24;
    }

//    //a r g b high to low
//    public static int byteArrayToInt(byte[] b, int pos) {
//        return b[pos + 2] & 0xFF |
//                (b[pos + 1] & 0xFF) << 8 |
//                (b[pos] & 0xFF) << 16 |
//                (255 & 0xFF) << 24;
//    }

    public static int byteArrayToInt(byte[] b, int pos) {

        int value = 0;

        value += (int) (b[pos + 0] << 0);
        value += (int) (b[pos + 1] << 8);
        value += (int) (b[pos + 2] << 16);
        value += (int) (0xff << 24);

        return value;


    }


    //a r g b high to low
//    public static int byteArrayToInt(byte[] b, int pos) {
//        return 255 & 0xFF |
//                (b[pos + 1] & 0xFF) << 8 |
//                (0 & 0xFF) << 16 |
//                (255 & 0xFF) << 24;
//    }


    public static int byteArrayToIntfake(byte[] b, int pos) {
        return 0 & 0xFF |
                (255 & 0xFF) << 8 |
                (255 & 0xFF) << 16 |
                (255 & 0xFF) << 24;
    }

    public Bitmap CreateBitmap_from_raw_data(int width, int height, byte[] buf) {
        int w = width;//1280;
        int h = height;//1024;

        //     Log.d("zhanwei", "buf.length    " + String.valueOf(buf.length));

        int[] pixels = new int[w * h];
        int count = 0;
        for (; count < w * h; count++) {
            pixels[count] = graybyteArrayToInt(buf, count);

        }

        Bitmap bmp = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        bmp.setHasAlpha(false);
        bmp.setPixels(pixels, 0, w, 0, 0, w, h);

        return bmp;
    }


    public Bitmap CreateBitmap_from_byte_rgba_data(int width, int height, byte[] buf) {


        int w = width;//1280;
        int h = height;//1024;

        Log.d("zhanwei", "buf.length    " + String.valueOf(buf.length));

        int[] pixels = new int[w * h];
        int count = 0;
        for (; count < w * h; count++) {

            pixels[count] = byteArrayToInt(buf, count);

        }

        Bitmap bmp = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        bmp.setHasAlpha(false);
        bmp.setPixels(pixels, 0, w, 0, 0, w, h);

        return bmp;
    }

    public Bitmap CreateBitmap_from_int_rgba_data(int width, int height, int[] buf) {


        int w = width;//1280;
        int h = height;//1024;

//        Log.d("zhanwei", "buf.length    " + String.valueOf(buf.length));

        Bitmap bmp = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);

        bmp.setHasAlpha(false);

        bmp.setPixels(buf, 0, w, 0, 0, w, h);

        return bmp;
    }


//    public Bitmap CreateBitmap_from_rgba_data(int width, int height, byte[] buf) { //very slow
//        int w = width;//1280;
//        int h = height;//1024;
//
//        Log.d("zhanwei", "buf.length    " + String.valueOf(buf.length));
//
//        Bitmap bmp = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
//
//        for (int i = 0; i < height; i++) {
//            for (int j = 0; j < width; j++) {
//                int color = buf[i * width + j] + buf[i * width + j + 1] << 8 + buf[i * width + j + 2] << 16 + buf[i * width + j + 3] << 24;
//
//                bmp.setPixel(j, i, color);
//
//            }
//        }
//
//        return bmp;
//    }


    void set_evbususb_rights() {

        Process p;
        try {
            // Preform su to get root privledges
            p = Runtime.getRuntime().exec(new String[]{"su", "chmod -R 777 /dev/bus/usb/"});


            // Attempt to write a file to a root-only
            DataOutputStream os = new DataOutputStream(p.getOutputStream());


            os.writeBytes("echo \"Do I have root?\" >/system/sd/temporary.txt\n");
            os.writeBytes("chmod -R 777 /dev/bus/usb/   \n");
            //   os.writeBytes("setenforce 0\n");

            // Close the terminal
            os.writeBytes("exit\n");
            os.flush();
            try {
                p.waitFor();
                if (p.exitValue() != 255) {
                    // TODO Code to run on success
                    Log.d("zhanwei", "root");
                } else {
                    // TODO Code to run on unsuccessful
                    Log.d("zhanwei", "not root");
                }
            } catch (InterruptedException e) {
                // TODO Code to run in interrupted exception
                Log.d("zhanwei", "not root");
            }
        } catch (IOException e) {
            // TODO Code to run in input/output exception
            Log.d("zhanwei", "not root");
        }


    }


    // Example of a call to a native method
//    TextView tv = (TextView) findViewById(R.id.sample_text);
//    tv.setText(stringFromJNI());


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onDestroy() {

        ksjcam.UnInit();

        super.onDestroy();
    }
}
