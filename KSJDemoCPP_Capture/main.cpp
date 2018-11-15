#include <stdio.h>
#include "stdlib.h"
#include <pthread.h>
#include <sys/types.h>

#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#include "KSJApi.h"
using namespace std;

typedef unsigned int        DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

typedef struct tagBITMAPFILEHEADER {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
}  __attribute__((packed)) BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
	DWORD      biSize;
	int       biWidth;
	int       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	int       biXPelsPerMeter;
	int       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
}  __attribute__((packed)) BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
	BYTE    rgbReserved;
} __attribute__((packed)) RGBQUAD;

typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER    bmiHeader;
	RGBQUAD             bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;


#define MAKEWIDTHBYTES( PIXELW, BITCOUNT )      (((( BITCOUNT * PIXELW ) + 31) / 32) * 4)     // 图像4Byte对齐

bool SaveToBmp(unsigned char *pData, int nWidth, int nHeight, int nBitCount, const char *pszFileName)
{
	if (pData == NULL || pszFileName == NULL)
	{
		return false;
	}

	if ((nBitCount != 24) && (nBitCount != 32))
	{
		return false;
	}

	int    nBmpInfoSize = sizeof(BITMAPINFOHEADER);
	int    nWidthBytes = MAKEWIDTHBYTES(nWidth, nBitCount);
	int    nImageSize = nWidthBytes * nHeight;

	FILE *pf = NULL;

	pf = fopen(pszFileName, "w+");

	if (NULL == pf)
	{
		return false;
	}

	BITMAPFILEHEADER *pBmpFileHeader = (BITMAPFILEHEADER*) new char[sizeof(BITMAPFILEHEADER)];

	pBmpFileHeader->bfType = 0x4d42; // "BM"
	pBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER)+nBmpInfoSize + nImageSize;
	pBmpFileHeader->bfReserved1 = 0;
	pBmpFileHeader->bfReserved2 = 0;
	pBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER)+nBmpInfoSize;

	BITMAPINFO *pBmpInfo = (BITMAPINFO*) new char[nBmpInfoSize];

	pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBmpInfo->bmiHeader.biPlanes = 1;
	pBmpInfo->bmiHeader.biCompression = 0; //BI_RGB;
	pBmpInfo->bmiHeader.biSizeImage = 0;
	pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	pBmpInfo->bmiHeader.biClrUsed = 0;
	pBmpInfo->bmiHeader.biClrImportant = 0;

	pBmpInfo->bmiHeader.biWidth = nWidth;
	pBmpInfo->bmiHeader.biHeight = -nHeight;      // top-down
	pBmpInfo->bmiHeader.biBitCount = 24;
	pBmpInfo->bmiHeader.biSizeImage = nImageSize;

	unsigned int dwNumberOfByteWritten;
	fwrite(pBmpFileHeader, 1, sizeof(BITMAPFILEHEADER), pf);
	fwrite(pBmpInfo, 1, nBmpInfoSize, pf);
	fwrite(pData, 1, nImageSize, pf);

	delete[] pBmpInfo;
	delete[] pBmpFileHeader;

	fflush(pf);
	fclose(pf);

	return true;
}

int main(int argc, void ** argv)
{
	int nRet = 0;

	nRet = KSJ_Init();

	printf("===== KSJ_Init nRet = %d =====\r\n", nRet);

	if (nRet != RET_SUCCESS)
	{
		printf("===== Init failed! exit... ===== \n");
		return 0;
	}

	int nCamCount = KSJ_DeviceGetCount();

	printf("===== KSJ_DeviceGetCount found number = %d! =====\n", nCamCount);

	if (nCamCount <= 0)
	{
		printf("===== KSJ_DeviceGetCount: cann't found any camare ! exit... ===== \n");
		nRet = KSJ_UnInit();
		exit(1);
	}

	nRet = KSJ_SetSerials(0, 0);
	printf("===== KSJ_SetSerials 0  ret = %d =====\r\n", nRet);

	nRet = KSJ_SetSerials(1, 1);
	printf("===== KSJ_SetSerials 1  ret = %d =====\r\n", nRet);

	unsigned short pusDeviceType;
	int            pnSerials;
	unsigned short pusFirmwareVersion;
	unsigned short pusFpgaVersion;


	nRet = KSJ_DeviceGetInformationEx(0, &pusDeviceType, &pnSerials, &pusFirmwareVersion, &pusFpgaVersion);
	if (nRet != RET_SUCCESS) printf("===== Device 0 Info: 0x%08X - 0x%08X - 0x%08X - 0x%08X =====\n", pusDeviceType, pnSerials, pusFirmwareVersion, pusFpgaVersion);
	
	nRet = KSJ_DeviceGetInformationEx(1, &pusDeviceType, &pnSerials, &pusFirmwareVersion, &pusFpgaVersion);
	if (nRet != RET_SUCCESS) printf("===== Device 1 Info: 0x%08X - 0x%08X - 0x%08X - 0x%08X =====\n", pusDeviceType, pnSerials, pusFirmwareVersion, pusFpgaVersion);

	char working_path[128];
	getcwd(working_path, sizeof(working_path));

	int nIndex = 0;

	int nWidth;
	int nHeight;
	int nBitCount;
	int nBufferSize = 0;
	unsigned char* pImageBuffer = NULL;

	nRet = KSJ_CaptureGetSizeEx(0, &nWidth, &nHeight, &nBitCount);

	if (nRet == RET_SUCCESS)
	{
		if (nBufferSize < (nWidth*nHeight*nBitCount / 8))
		{
			if (pImageBuffer != NULL)
			{
				delete[]pImageBuffer;
				pImageBuffer = NULL;
			}

			nBufferSize = nWidth*nHeight*nBitCount / 8;
			pImageBuffer = new unsigned char[nBufferSize];
		}

		printf("===== Warming up camare 1=====\n");

		nRet = KSJ_TriggerModeSet(0, KSJ_TRIGGER_INTERNAL);
		printf("===== KSJ_TriggerModeSet camare 1 as software, ret = %d =====\r\n", nRet);
		nRet = KSJ_SetParam(0, KSJ_MIRROR, true);
		printf("===== KSJ_SetParam camare 1 as mirror, ret = %d =====\r\n", nRet);
		for (int i = 0; i < 20; ++i)
		{
			nRet = KSJ_CaptureRgbData(0, pImageBuffer);
		}

		nRet = KSJ_SetParam(0, KSJ_MIRROR, false);
		printf("===== KSJ_SetParam camare 1 as no-mirror, ret = %d =====\r\n", nRet);
		for (int i = 0; i < 200; ++i)
		{
			nRet = KSJ_CaptureRgbData(0, pImageBuffer);
		}

		++nIndex;
	}

	nIndex = 0;

	nRet = KSJ_CaptureGetSizeEx(1, &nWidth, &nHeight, &nBitCount);

	if (nRet == RET_SUCCESS)
	{
		if (nBufferSize < (nWidth*nHeight*nBitCount / 8))
		{
			if (pImageBuffer != NULL)
			{
				delete[]pImageBuffer;
				pImageBuffer = NULL;
			}

			nBufferSize = nWidth*nHeight*nBitCount / 8;
			pImageBuffer = new unsigned char[nBufferSize];
		}

		printf("===== Warming up camare 2=====\n");

		nRet = KSJ_TriggerModeSet(1, KSJ_TRIGGER_INTERNAL);
		printf("===== KSJ_TriggerModeSet camare 2 as software, ret = %d =====\r\n", nRet);
		nRet = KSJ_SetParam(1, KSJ_MIRROR, true);
		printf("===== KSJ_SetParam camare 2 as mirror, ret = %d =====\r\n", nRet);
		for (int i = 0; i < 20; ++i)
		{
			nRet = KSJ_CaptureRgbData(1, pImageBuffer);
		}

		nRet = KSJ_SetParam(1, KSJ_MIRROR, false);
		printf("===== KSJ_SetParam camare 2 as no-mirror, ret = %d =====\r\n", nRet);
		for (int i = 0; i < 200; ++i)
		{
			nRet = KSJ_CaptureRgbData(1, pImageBuffer);
		}

		++nIndex;
	}

	nRet = KSJ_TriggerModeSet(0, KSJ_TRIGGER_FIXFRAMERATE);
	printf("===== KSJ_TriggerModeSet camare 1 as fixframerate, ret = %d =====\r\n", nRet);
	nRet = KSJ_SetFixedFrameRateEx(0, 8);
	printf("===== KSJ_TriggerModeSet fix frame rate of camare 1 to be 8, ret = %d =====\r\n", nRet);

	nRet = KSJ_TriggerModeSet(1, KSJ_TRIGGER_FIXFRAMERATE);
	printf("===== KSJ_TriggerModeSet camare 2 as fixframerate, ret = %d =====\r\n", nRet);
	nRet = KSJ_SetFixedFrameRateEx(1, 8);
	printf("===== KSJ_TriggerModeSet fix frame rate of camare 2 to be 8, ret = %d =====\r\n", nRet);

	printf("\r\n\r\n\r\n===== capture data=====\r\n\\rn");

	nIndex = 0;

	while (1)
	{
		nRet = KSJ_CaptureGetSizeEx(0, &nWidth, &nHeight, &nBitCount);
		if (nRet == RET_SUCCESS)
		{
			if (nBufferSize < (nWidth*nHeight*nBitCount / 8))
			{
				if (pImageBuffer != NULL)
				{
					delete[]pImageBuffer;
					pImageBuffer = NULL;
				}

				nBufferSize = nWidth*nHeight*nBitCount / 8;
				pImageBuffer = new unsigned char[nBufferSize];
			}

			nRet = KSJ_CaptureRgbData(0, pImageBuffer);

			if (nRet == RET_SUCCESS)
			{
				char szFilePath[260] = { 0 };

				sprintf(szFilePath, "%s/c1-img%02d.bmp", working_path, nIndex % 20);

				if (SaveToBmp(pImageBuffer, nWidth, nHeight, nBitCount, szFilePath))
				{
					printf("===== camare1 save image to %s ok =====\n", szFilePath);
				}
				else
				{
					printf("===== camare1 save image to %s failed =====\n", szFilePath);
				}

			}
			else
			{
				printf("===== KSJ_CaptureRgbData failed : ret = %d, index = %d =====\n", nRet, nIndex);

			}
		}

		nRet = KSJ_CaptureGetSizeEx(1, &nWidth, &nHeight, &nBitCount);
		if (nRet == RET_SUCCESS)
		{
			if (nBufferSize < (nWidth*nHeight*nBitCount / 8))
			{
				if (pImageBuffer != NULL)
				{
					delete[]pImageBuffer;
					pImageBuffer = NULL;
				}

				nBufferSize = nWidth*nHeight*nBitCount / 8;
				pImageBuffer = new unsigned char[nBufferSize];
			}

			nRet = KSJ_CaptureRgbData(1, pImageBuffer);

			if (nRet == RET_SUCCESS)
			{
				char szFilePath[260] = { 0 };

				sprintf(szFilePath, "%s/c2-img%02d.bmp", working_path, nIndex % 20);

				if (SaveToBmp(pImageBuffer, nWidth, nHeight, nBitCount, szFilePath))
				{
					printf("===== camare2 save image to %s ok =====\n", szFilePath);
				}
				else
				{
					printf("===== camare2 save image to %s failed =====\n", szFilePath);
				}

			}
			else
			{
				printf("===== KSJ_CaptureRgbData failed : ret = %d, index = %d =====\n", nRet, nIndex);
			}
		}

		++nIndex;
	}


	if (pImageBuffer != NULL)
	{
		delete[]pImageBuffer;
		pImageBuffer = NULL;
	}

	nRet = KSJ_UnInit();


	printf("===== exit app ... =====\n");

	return 0;
}



