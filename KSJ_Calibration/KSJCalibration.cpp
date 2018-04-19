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
	//initUndistortRectifyMap_Self(cameraMatrix, distCoeffs,imgSize, MapX, MapY);
	return true;
}

bool KSJ_CalRemapFloat14(
    vector<vector<Point2f> > &vecImagePts,
    vector<vector<Point3f> > &vecWorldPts,
	Size  imgSize,
	float fCoefficient[14]
	)
{
	Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* 摄像机内参数矩阵 */
	Mat distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0)); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
	vector<Mat> tvecsMat;  /* 每幅图像的旋转向量 */
	vector<Mat> rvecsMat; /* 每幅图像的平移向量 */

	/* 开始标定 */
	calibrateCamera(vecWorldPts, vecImagePts, imgSize, cameraMatrix, distCoeffs, rvecsMat, tvecsMat, 0);
	fCoefficient[0] = cameraMatrix.at<double>(0, 0);
	fCoefficient[1] = cameraMatrix.at<double>(0, 1);
	fCoefficient[2] = cameraMatrix.at<double>(0, 2);
	fCoefficient[3] = cameraMatrix.at<double>(1, 0);
	fCoefficient[4] = cameraMatrix.at<double>(1, 1);
	fCoefficient[5] = cameraMatrix.at<double>(1, 2);
	fCoefficient[6] = cameraMatrix.at<double>(2, 0);
	fCoefficient[7] = cameraMatrix.at<double>(2, 1);
	fCoefficient[8] = cameraMatrix.at<double>(2, 2);
	fCoefficient[9] = distCoeffs.at<double>(0, 0);
	fCoefficient[10] = distCoeffs.at<double>(0, 1);
	fCoefficient[11] = distCoeffs.at<double>(0, 2);
	fCoefficient[12] = distCoeffs.at<double>(0, 3);
	fCoefficient[13] = distCoeffs.at<double>(0, 4);

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

void initUndistortRectifyMap_Self(Mat& cameraMatrix, Mat& distCoeffs, Size size, Mat& map1, Mat& map2)
{
	//A矩阵表示的是相机的内建参数矩阵
	double A[3][3] = { cameraMatrix.at<double>(0, 0), cameraMatrix.at<double>(0, 1), cameraMatrix.at<double>(0, 2),
		cameraMatrix.at<double>(1, 0), cameraMatrix.at<double>(1, 1), cameraMatrix.at<double>(1, 2),
		cameraMatrix.at<double>(2, 0), cameraMatrix.at<double>(2, 1), cameraMatrix.at<double>(2, 2) };
	//5个畸变系数
	double dist[5] = { distCoeffs.at<double>(0, 0), distCoeffs.at<double>(0, 1), distCoeffs.at<double>(0, 2), distCoeffs.at<double>(0, 3), distCoeffs.at<double>(0, 4) };

	//iR表示A的逆。其实是原函数中newCameraMatrix的逆
	//计算A的行列式
	double d = A[0][0] * (A[1][1] * A[2][2] - A[2][1] * A[1][2]) -
		A[0][1] * (A[1][0] * A[2][2] - A[2][0] * A[1][2]) +
		A[0][2] * (A[1][0] * A[2][1] - A[2][0] * A[1][1]);

	if (d<0.0000001 && d>0.0000001) return;
	d = 1 / d;
	double iR[3][3] = {
		(A[1][1] * A[2][2] - A[1][2] * A[2][1]) * d,
		(A[0][2] * A[2][1] - A[0][1] * A[2][2]) * d,
		(A[0][1] * A[1][2] - A[0][2] * A[1][1]) * d,
		(A[1][2] * A[2][0] - A[1][0] * A[2][2]) * d,
		(A[0][0] * A[2][2] - A[0][2] * A[2][0]) * d,
		(A[0][2] * A[1][0] - A[0][0] * A[1][2]) * d,
		(A[1][0] * A[2][1] - A[1][1] * A[2][0]) * d,
		(A[0][1] * A[2][0] - A[0][0] * A[2][1]) * d,
		(A[0][0] * A[1][1] - A[0][1] * A[1][0]) * d
	};

	double u0 = A[0][2], v0 = A[1][2];
	double fx = A[0][0], fy = A[1][1];

	double k1 = dist[0];
	double k2 = dist[1];
	double p1 = dist[2];
	double p2 = dist[3];
	double k3 = dist[4];

	for (int i = 0; i < size.height; i++)
	{
		float* m1f = map1.ptr<float>(i);
		float* m2f = map2.ptr<float>(i);
		double _x = i*iR[0][1] + iR[0][2], _y = i*iR[1][1] + iR[1][2], _w = i*iR[2][1] + iR[2][2];

		for (int j = 0; j < size.width; j++, _x += iR[0][0], _y += iR[1][0], _w += iR[2][0])
		{
			double w = 1. / _w, x = _x*w, y = _y*w;
			double x2 = x*x, y2 = y*y;
			double r2 = x2 + y2, _2xy = 2 * x*y;
			double kr = 1 + ((k3*r2 + k2)*r2 + k1)*r2;
			double xd = x*kr + p1*_2xy + p2*(r2 + 2 * x2);
			double yd = y*kr + p1*(r2 + 2 * y2) + p2*_2xy;
			//cv::Vec3d vecTilt = matTilt*cv::Vec3d(xd, yd, 1);
			//double invProj = vecTilt(2) ? 1. / vecTilt(2) : 1;
			double u = fx * xd + u0;
			double v = fy * yd + v0;
			m1f[j] = (float)u;
			m2f[j] = (float)v;
		}
	}
}
