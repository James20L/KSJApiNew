//从文件中读取Remap用的参数矩阵
//函数调用完成后会给pfMapX和pfMapY分配内存，用完后应该delete
bool KSJ_ReadMapfromFile(
	 const char * strFileName,		//文件名
	 int &nWidth,                   //图像的宽
	 int &nHeight,                  //图像的高
	 int &nFracPrecision,           //小数精度
	 void* &pMapX,                //用于重映射的X矩阵
	 void* &pMapY                 //用于重映射的Y矩阵
	 );

//重映射函数
void KSJ_Remap(
	unsigned char* pImgInput,      //输入图像数据流
	int            nBytesPerPixel, //一个像素占几个字节
	int            nWidth,         //图像的宽
	int            nHeight,        //图像的高
	int            nFracPrecision, //小数精度
	unsigned char* pImgOutput,     //输出图像的数据流
	void*          pMapX,          //从文件中读取的X矩阵
	void*          pMapY           //从文件中读取的X矩阵
	);