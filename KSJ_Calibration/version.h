//当前版本1.0
#define KSJ_CM_VERSION_MAJ   1
#define KSJ_CM_VERSION_MIN   0


typedef struct _tagCALMAPFILEHEADER
{
	unsigned short usType;           // ('M'<<8) | 'C')
	unsigned short usVersion;        // ( KSJ_CM_VERSION_MAJ << 8 ) | KSJ_CM_VERSION_MIN
	unsigned int   uiWidth;          // (像素）
	unsigned int   uiHeight;         // (像素）
	unsigned int   uiFracPrecision;  // 利用定点计算时的小数精度
	unsigned int   uiMapXBytesNum;   // 矩阵MapX的字节数，类型为void *
	unsigned int   uiMapYBytesNum;   // 矩阵MapY的字节数，类型为void *
}CAL_MAP_FILE_HEADER;
