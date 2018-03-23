#include "KSJCalibrartion.h"
#include "version.h"
#include <fstream>

bool KSJ_FindConners(
	 Mat& imgInput,
	 Size BoardSize,
     vector<vector<Point2f> > &vecImagePts,
     vector<vector<Point3f> > &vecWorldPts
	 )
{
	vector<Point2f> vecImagePtsBuf;  /* 缓存每幅图像上检测到的角点 */
	if (0 == findChessboardCorners(imgInput, BoardSize, vecImagePtsBuf))
	{
		return false;
	}

	Mat matGray;
	if (imgInput.type() == CV_8UC1)
	{
		matGray = imgInput;
	}
	else
	{
		cvtColor(imgInput, matGray, CV_RGB2GRAY);
	}
	
	/* 亚像素精确化 */
	find4QuadCornerSubpix(matGray, vecImagePtsBuf, Size(5, 5)); //对粗提取的角点进行精确化  
	vecImagePts.push_back(vecImagePtsBuf);  //保存亚像素角点  
	vector<Point3f> tempPointSet;
	for (int i = 0; i < BoardSize.height; i++)
	{
		for (int j = 0; j < BoardSize.width; j++)
		{
			Point3f realPoint;
			/* 假设标定板放在世界坐标系中z=0的平面上 */
			realPoint.x = i;
			realPoint.y = j;
			realPoint.z = 0;
			tempPointSet.push_back(realPoint);
		}
	}
	vecWorldPts.push_back(tempPointSet);
	return true;
}

bool KSJ_CalRemapMat(
     vector<vector<Point2f> > &vecImagePts,
     vector<vector<Point3f> > &vecWorldPts,
	 Size  imgSize,
	 Mat&  MapX,
	 Mat&  MapY
	 )
{
	Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* 摄像机内参数矩阵 */
	Mat distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0)); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
	vector<Mat> tvecsMat;  /* 每幅图像的旋转向量 */
	vector<Mat> rvecsMat; /* 每幅图像的平移向量 */
	
	/* 开始标定 */
	calibrateCamera(vecWorldPts, vecImagePts, imgSize, cameraMatrix, distCoeffs, rvecsMat, tvecsMat, 0);
	MapX = Mat(imgSize, CV_32FC1);
	MapY = Mat(imgSize, CV_32FC1);
	Mat R = Mat::eye(3, 3, CV_32F);
	initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, imgSize, CV_32FC1, MapX, MapY);

	return true;
}

void KSJ_WriteMaptoFile(
	const char * strFileName,
	Mat& MapX,
	Mat& MapY
	)
{
	fstream file;
	file.open(strFileName, ios::out | ios::binary|ios::trunc);

	CAL_MAP_FILE_HEADER FileHeader;
	FileHeader.usType = ('M' << 8) | 'C';
	FileHeader.usVersion = (KSJ_CM_VERSION_MAJ << 8) | KSJ_CM_VERSION_MIN;
	FileHeader.uiWidth = MapX.cols;
	FileHeader.uiHeight = MapX.rows;
	FileHeader.uiFracPrecision = 10;
	FileHeader.uiMapXBytesNum = FileHeader.uiWidth * FileHeader.uiHeight * sizeof(unsigned short);
	FileHeader.uiMapYBytesNum = FileHeader.uiWidth * FileHeader.uiHeight * sizeof(unsigned short);
	file.write((char *)&FileHeader, sizeof(CAL_MAP_FILE_HEADER));

	for (int i = FileHeader.uiHeight - 1; i >= 0; --i)
	{
		for (int j = 0; j < FileHeader.uiWidth; ++j)
		{
			unsigned short nPosX = (unsigned short)MapX.at<float>(i, j);
			file.write((char *)&nPosX, sizeof(unsigned short));
		}
	}

	for (int i = FileHeader.uiHeight - 1; i >= 0; --i)
	{
		for (int j = 0; j < FileHeader.uiWidth; ++j)
		{
			unsigned short usPosXFraction = (unsigned short)(((MapX.at<float>(i, j) - (int)MapX.at<float>(i, j))*(1 << FileHeader.uiFracPrecision)));
			file.write((char *)&usPosXFraction, sizeof(unsigned short));
		}
	}

	for (int i = FileHeader.uiHeight - 1; i >= 0; --i)
	{
		for (int j = 0; j < FileHeader.uiWidth; ++j)
		{
			unsigned short nPosY = (unsigned short)(FileHeader.uiHeight - 1 - MapY.at<float>(i, j));
			file.write((char *)&nPosY, sizeof(unsigned short));
		}
	}

	for (int i = FileHeader.uiHeight - 1; i >= 0; --i)
	{
		for (int j = 0; j < FileHeader.uiWidth; ++j)
		{
			float fPosY = FileHeader.uiHeight - 1 - MapY.at<float>(i, j);
			unsigned short usPosYFraction = (unsigned short)(((fPosY - (int)fPosY)*(1 << FileHeader.uiFracPrecision)));
			file.write((char *)&usPosYFraction, sizeof(unsigned short));
		}
	}
}
