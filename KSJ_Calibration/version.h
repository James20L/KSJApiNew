//��ǰ�汾1.0
#define KSJ_CM_VERSION_MAJ   1
#define KSJ_CM_VERSION_MIN   0


typedef struct _tagCALMAPFILEHEADER
{
	unsigned short usType;           // ('M'<<8) | 'C')
	unsigned short usVersion;        // ( KSJ_CM_VERSION_MAJ << 8 ) | KSJ_CM_VERSION_MIN
	unsigned int   uiWidth;          // (���أ�
	unsigned int   uiHeight;         // (���أ�
	unsigned int   uiFracPrecision;  // ���ö������ʱ��С������
	unsigned int   uiMapXBytesNum;   // ����MapX���ֽ���������Ϊvoid *
	unsigned int   uiMapYBytesNum;   // ����MapY���ֽ���������Ϊvoid *
}CAL_MAP_FILE_HEADER;
