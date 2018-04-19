#include "KSJRemap.h"
#include "version.h"
#include <fstream>
using namespace std;

bool KSJ_ReadMapfromFile(
	const char * strFileName,
	int &nWidth,
	int &nHeight,
	int &nFracPrecision,
	void* &pMapX,
	void* &pMapY
	)
{
	fstream file;
	file.open(strFileName, ios::in | ios::binary);
	//打开文件失败
	if (!file)
	{
		return false;
	}
	//判断文件大小是否正确
	file.seekg(0, file.end);
	int nFileSize = file.tellg();
	if (nFileSize < sizeof(CAL_MAP_FILE_HEADER))//文件大小小于文件头
	{
		file.close();
		return false;
	}

	file.seekg(0, file.beg);
	CAL_MAP_FILE_HEADER FileHeader;
	file.read((char*)&FileHeader, sizeof(CAL_MAP_FILE_HEADER));
	if (FileHeader.usType != (('M' << 8) | 'C') || FileHeader.usVersion != ((KSJ_CM_VERSION_MAJ << 8) | KSJ_CM_VERSION_MIN))//版本不正确
	{
		file.close();
		return false;
	}
	if (nFileSize - sizeof(CAL_MAP_FILE_HEADER) != FileHeader.uiMapXBytesNum * 2 + FileHeader.uiMapYBytesNum * 2)//文件大小与对应的图像宽高不符
	{
		file.close();
		return false;
	}

	nWidth = FileHeader.uiWidth;
	nHeight = FileHeader.uiHeight;
	nFracPrecision = FileHeader.uiFracPrecision;
	pMapX = (void *) new char[FileHeader.uiMapXBytesNum * 2];
	pMapY = (void *) new char[FileHeader.uiMapYBytesNum * 2];
	file.read((char *)pMapX, FileHeader.uiMapXBytesNum * 2);
	file.read((char *)pMapY, FileHeader.uiMapYBytesNum * 2);
	return true;
}

void KSJ_Remap(
	unsigned char* pImgInput,
	int            nBytesPerPixel,
	int            nWidth,
	int            nHeight,
	int            nFracPrecision,
	unsigned char* pImgOutput,
	void*          pMapX,
	void*          pMapY
	)
{
	unsigned short *mapx = (unsigned short *)pMapX;
	unsigned short *mapy = (unsigned short *)pMapY;
	unsigned short* mapFracX = (unsigned short*)((unsigned char *)pMapX + nWidth*nHeight*sizeof(unsigned short));
	unsigned short* mapFracY = (unsigned short*)((unsigned char *)pMapY + nWidth*nHeight*sizeof(unsigned short));
	unsigned char *imgOut = pImgOutput;

	int nPixelNum = nHeight * nWidth;
	int nOffsetAtoLast = (nWidth*(nHeight - 1) - 2)*nBytesPerPixel;
	for (int i = 0; i < nPixelNum; ++i)
	{
		int  x = (*mapx);
		int  y = (*mapy);
		int deltaX = (*mapFracX);
		int deltaY = (*mapFracY);
		//float deltaX = (float)*mapFracX / 1024;
		//float deltaY = (float)*mapFracY / 1024;;

		int nOffset = (y*nWidth + x)*nBytesPerPixel;

		if (nOffset<0 || nOffset>nOffsetAtoLast)//用于插值的四个点中若有超出范围的补黑边
		{
			for (int k = 0; k < nBytesPerPixel; k++)
			{
				*imgOut = 0;
				++imgOut;
			}
		}
		else
		{
			//float deltaX = x - (int)x;
			//float deltaY = y - (int)y;
			//int deltaX = (int)((x - (int)x) * 1024);//利用定点计算
			//int deltaY = (int)((y - (int)y) * 1024);
			unsigned char*a = pImgInput + nOffset;
			unsigned char*b = a + nBytesPerPixel;
			unsigned char*c = a + nWidth*nBytesPerPixel;
			unsigned char*d = c + nBytesPerPixel;

			for (int k = 0; k < nBytesPerPixel; k++)
			{
				//*imgOut = (int)(deltaX*deltaY*(*a - *b - *c + *d) + deltaY*(*c - *a) + deltaX*(*b - *a) + *a);
				*imgOut = (int)(deltaX*deltaY*(*a - *b - *c + *d) + ((deltaY*(*c - *a)) << nFracPrecision) + ((deltaX*(*b - *a)) << nFracPrecision) + (*a << (nFracPrecision << 1))) >> (nFracPrecision << 1);
				++a;
				++b;
				++c; 
				++d;
				++imgOut;
			}
		}
		++mapx;
		++mapy;
		++mapFracX;
		++mapFracY;
	}
}


bool MakeCalibrationMap(
	float fCoefficient[14],
	unsigned int uiWidth,
	unsigned int uiHeight,
	unsigned int uiFracPrecision, //小数精度
	unsigned short *pusMapX,
	unsigned short *pusMapDeltaX,
	unsigned short *pusMapY,
	unsigned short *pusMapDeltaY
	)
{
	//A矩阵表示的是相机的内建参数矩阵
	double A[3][3] = { fCoefficient[0], fCoefficient[1], fCoefficient[2], fCoefficient[3], fCoefficient[4], fCoefficient[5], fCoefficient[6], fCoefficient[7], fCoefficient[8] };
	//5个畸变系数
	//double dist[5] = { fCoefficient[9], fCoefficient[10], fCoefficient[11], fCoefficient[12], fCoefficient[13] };

	//iR表示A的逆。其实是原函数中newCameraMatrix的逆
	//计算A的行列式
	double d = A[0][0] * (A[1][1] * A[2][2] - A[2][1] * A[1][2]) -
		A[0][1] * (A[1][0] * A[2][2] - A[2][0] * A[1][2]) +
		A[0][2] * (A[1][0] * A[2][1] - A[2][0] * A[1][1]);

	if (d<0.0000001 && d>0.0000001) return false;//行列式为0，求逆失败
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

	double k1 = fCoefficient[9];//dist[0];
	double k2 = fCoefficient[10];//dist[1];
	double p1 = fCoefficient[11];//dist[2];
	double p2 = fCoefficient[12];//dist[3];
	double k3 = fCoefficient[13];//dist[4];

	for (int i = 0; i < uiHeight; i++)
	{
		double _x = i*iR[0][1] + iR[0][2], _y = i*iR[1][1] + iR[1][2], _w = i*iR[2][1] + iR[2][2];

		for (int j = 0; j < uiWidth; j++, _x += iR[0][0], _y += iR[1][0], _w += iR[2][0])
		{
			double w = 1. / _w, x = _x*w, y = _y*w;
			double x2 = x*x, y2 = y*y;
			double r2 = x2 + y2, _2xy = 2 * x*y;
			double kr = 1 + ((k3*r2 + k2)*r2 + k1)*r2;
			double xd = x*kr + p1*_2xy + p2*(r2 + 2 * x2);
			double yd = y*kr + p1*(r2 + 2 * y2) + p2*_2xy;
			double u = fx * xd + u0;
			double v = fy * yd + v0;

			*pusMapX = (unsigned short)u;
			*pusMapDeltaX = (unsigned short)((u - (int)u)*(1 << uiFracPrecision));
			*pusMapY = (unsigned short)v;
			*pusMapDeltaY = (unsigned short)((v - (int)v)*(1 << uiFracPrecision));

			++pusMapX;
			++pusMapDeltaX;
			++pusMapY;
			++pusMapDeltaY;
		}
	}
	return true;
}