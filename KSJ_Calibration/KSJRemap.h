//���ļ��ж�ȡRemap�õĲ�������
//����������ɺ���pfMapX��pfMapY�����ڴ棬�����Ӧ��delete
bool KSJ_ReadMapfromFile(
	 const char * strFileName,		//�ļ���
	 int &nWidth,                   //ͼ��Ŀ�
	 int &nHeight,                  //ͼ��ĸ�
	 int &nFracPrecision,           //С������
	 void* &pMapX,                //������ӳ���X����
	 void* &pMapY                 //������ӳ���Y����
	 );

//��ӳ�亯��
void KSJ_Remap(
	unsigned char* pImgInput,      //����ͼ��������
	int            nBytesPerPixel, //һ������ռ�����ֽ�
	int            nWidth,         //ͼ��Ŀ�
	int            nHeight,        //ͼ��ĸ�
	int            nFracPrecision, //С������
	unsigned char* pImgOutput,     //���ͼ���������
	void*          pMapX,          //���ļ��ж�ȡ��X����
	void*          pMapY           //���ļ��ж�ȡ��X����
	);