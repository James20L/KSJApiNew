#include "../../github.opencv310-in-vs/opencv.inc/opencv.h"
#include "../../github.opencv310-in-vs/opencv.inc/opencv_link.h"
#include <vector>
using namespace cv;
using namespace std;


//输入一张图像和棋盘格的尺寸，对应检测到的图像坐标和世界坐标会被保存在两个vector中
//可以用多张不同的图像（棋盘格在图像上的不同位置）
//反复调用这个函数，把对应的坐标都存储在两个vector中，提高标定校正的效果
bool KSJ_FindConners(
	 Mat& imgInput,								//输入图像
	 Size BoardSize,							//输入棋盘格的尺寸（尺寸为顶点的个数，不要包括最外围的一圈）
     vector<vector<Point2f> > &vecImagePts,		//保存所有检测到的角点的图像坐标,这是一个输入/输出参数，新检测出来的棋盘点图像坐标序列会pushback到这个向量中
     vector<vector<Point3f> > &vecWorldPts		//保存所有检测到的角点的世界坐标,这是一个输入/输出参数，新检测出来的棋盘点世界坐标序列会pushback到这个向量中
	 );

//利用上个函数得到的两个坐标vector，计算X,Y方向的映射矩阵
bool KSJ_CalRemapMat(
     vector<vector<Point2f> > &vecImagePts,
     vector<vector<Point3f> > &vecWorldPts,
	 Size  imgSize,								//图像的尺寸,也是MapX和MapY的尺寸
	 Mat&  MapX,								//输出的用于校正的X矩阵，矩阵大小与imgInput相同
	 Mat&  MapY									//输出的用于校正的Y矩阵，矩阵大小与imgInput相同
	 );

//把MapX和MapY两个矩阵以二进制格式写到文件中
void KSJ_WriteMaptoFile(
	const char * strFileName,
	Mat& MapX,
	Mat& MapY
	);
