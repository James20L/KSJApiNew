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
	if (nFileSize - sizeof(CAL_MAP_FILE_HEADER) != FileHeader.uiMapXBytesNum*2+FileHeader.uiMapYBytesNum*2)//文件大小与对应的图像宽高不符
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