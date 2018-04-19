#ifndef H_FSJ_CAPTURE
#define H_FSJ_CAPTURE
/*
功能:初始化康士佳摄像头

返回值：成功初始化话摄像头数量
*/
int KSJInitCapture();

/*
功能:设置某个相机工作
*/
void KSJSetCamsWork(int nIndex);


/*
功能:设置某个相机停止工作
*/
void KSJUnSetCamsWork(int nIndex);
/*
功能:	注销相机
*/
void KSJUnInitCapture(int nCamCount);

/*
功能:相机循环工作线程
参数:T_CameraInfo* 相机对象
*/
void* KSJCaptureWork(void* loopargs);

/*
功能:相机工作测试函数
参数:T_CameraInfo* 相机对象
*/

int testCaputreWork() ;

#endif