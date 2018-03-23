#include "../../github.opencv310-in-vs/opencv.inc/opencv.h"
#include "../../github.opencv310-in-vs/opencv.inc/opencv_link.h"
#include <vector>
using namespace cv;
using namespace std;


//����һ��ͼ������̸�ĳߴ磬��Ӧ��⵽��ͼ���������������ᱻ����������vector��
//�����ö��Ų�ͬ��ͼ�����̸���ͼ���ϵĲ�ͬλ�ã�
//������������������Ѷ�Ӧ�����궼�洢������vector�У���߱궨У����Ч��
bool KSJ_FindConners(
	 Mat& imgInput,								//����ͼ��
	 Size BoardSize,							//�������̸�ĳߴ磨�ߴ�Ϊ����ĸ�������Ҫ��������Χ��һȦ��
     vector<vector<Point2f> > &vecImagePts,		//�������м�⵽�Ľǵ��ͼ������,����һ������/����������¼����������̵�ͼ���������л�pushback�����������
     vector<vector<Point3f> > &vecWorldPts		//�������м�⵽�Ľǵ����������,����һ������/����������¼����������̵������������л�pushback�����������
	 );

//�����ϸ������õ�����������vector������X,Y�����ӳ�����
bool KSJ_CalRemapMat(
     vector<vector<Point2f> > &vecImagePts,
     vector<vector<Point3f> > &vecWorldPts,
	 Size  imgSize,								//ͼ��ĳߴ�,Ҳ��MapX��MapY�ĳߴ�
	 Mat&  MapX,								//���������У����X���󣬾����С��imgInput��ͬ
	 Mat&  MapY									//���������У����Y���󣬾����С��imgInput��ͬ
	 );

//��MapX��MapY���������Զ����Ƹ�ʽд���ļ���
void KSJ_WriteMaptoFile(
	const char * strFileName,
	Mat& MapX,
	Mat& MapY
	);
