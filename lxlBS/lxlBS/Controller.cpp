//��е�ۿ�����ʵ���ļ�

#include "StdAfx.h"
#include "Controller.h"
#include"Robot.h"
//#include "HalconVision.h"
#include "Kalman.h"
/*
ע��ǳ��ɫΪ������ǳ��ɫΪ������л�ɫΪ����
*/

//cvCreateMat�������ռ�,��Ҫ����ת���ɾ�����ʽ,CV_8UC1 ��ʾ8λ�޷��ŵ�ͨ������, CV_32SC2��ʾ32λ�з���˫ͨ������.
CvMat* Controller::JacobCurrent = cvCreateMat(4,3,CV_64FC1);	//Ϊ��ǰ�ſ˱Ⱦ���������ռ�4��3
int Controller::Switch_PRQ_flag = 0 ;  

Controller::Controller(void)
{
}

Controller::~Controller(void)
{
}

void Controller::Init(CHalconVision& m_vision1,CHalconVision& m_vision2,double *desirefeature)        //��ʼ��
{
	CvMat* JacobInit= cvCreateMat(4,3,CV_64FC1);	//����һ���ſ˱Ⱦ��󣬷������ռ�4��3

	for (int i =0;i<4;i++)
	{
		DesireFeature[i]=desirefeature[i];	//���������������ֵ��һ����������
	}

	InitJacob(m_vision1,m_vision2,JacobInit);	//�ſɱȾ����ʼֵ

	/*cvCloneImage��cvCloneMat���ڸ�ֵ��ͬʱ�Ὺ��һ���µĿռ������ı�����
	cvCopyImage��cvCopyMatֻ����ֵ�����������һ���ռ����ֵ����
	���cvCloneImage��cvCloneMatֻ�ʺ����ڱ�����ʼ���壬ǧ��Ҫ�����㷨�����м䣬
	��������һ���µĵ�ַ�ռ䣬�Ὣ��ֵ�����ָ���ַ�ı䣬
	�ڳ����м���и���ʱ����ʹ��cvCopyImage��cvCopyMat��*/
	JacobCurrent = cvCloneMat(JacobInit);	//��ֵ�ҿ��ٿռ䣬���嵱ǰ�ſ˱Ⱦ���

	int k=0;
	for (int i = 0; i < JacobInit->rows; i++) 
	{
		for (int j = 0; j < JacobInit->cols; j++) 
		{	//��(i, j)��Ԫ�ظ�ֵ��һ������Ԫ�أ�����ȡ�������е�Ԫ�����һ��һά���顣
			kalmanJacobInit[k++] = cvmGet(JacobInit,i,j);
		}
	}
};

void Controller::InitJacob(CHalconVision& m_vision1,CHalconVision& m_vision2,CvMat *JacobInit)	//�ſɱȾ����ʼֵ
{
	Robot DensoRob;	//����һ��robot����
	CStringArray *CurrentPos = new CStringArray;	//MFC CStringArray �ַ��������࣬���嵱ǰλ�á� C++��new��������ڶ�̬����ͳ����ڴ���������
	CString DesPos;		//��������λ��
	CHalconVision CamVision;	//����һ���������
	
	double PosStartArray[7] = {0};	//����ĩ����ʼλ��
	double PosDelta[7] = {0};	//����ĩ��λ��ƫ������
	double delta_pos = 10;	//λ������

	double FeatureCurrent[4] = {0};	//����һ����ǰ��������
	double FeatureNext[4] = {0};	//��һ������
	double DeltaFeature[4][3] = {0};	//����ƫ��
	
	double DeltaPos[3][3] = {0};	//λ��ƫ�����
	double PosStartArrayCopy[7] = {0};	//��ʼλ�ø�������

	DensoRob.Init();	//robot�����ʼ�������豸�����Ƿ�����
	DensoRob.pIRobot->Move(2,CComVariant("@0 P0"),CComBSTR(L""));    //�ƶ�����ʼλ��
	DensoRob.pIRobot->Speed(-1,20);   //�����˶��ٶ�
	Sleep(500);	//��ʱ
	
	DensoRob.GetCurrentToolPos(PosStartArray);	//��ù�������ϵĩ�˿�ʼλ��
	DensoRob.GetCurrentToolPos(PosStartArrayCopy);	//���Ʊ��湤������ϵĩ���ʼλ��
	
	//�ɼ�ͼ��������ĩ��λ��
	m_vision1.CalcPos(FeatureCurrent[0],FeatureCurrent[1]);//���1��ȡ�ĵ�ǰ������  //�������괦λ�á�
	m_vision2.CalcPos(FeatureCurrent[2],FeatureCurrent[3]);//���1��ȡ�ĵ�ǰ������  //�������괦λ�á�
	
	for(int i = 0; i<3;i++)
	{
		PosStartArray[i] += delta_pos;//����ĩ��λ�ò��ϱ仯��delta_pos = 10
		DensoRob.SetToolPos(PosStartArray);//����ĩ��λ��ת��Ϊ��е��ĩ��λ��(����任)
		DensoRob.ArrayToPos(PosStartArray,DesPos);//����ĩ�˵���λ��������λ������

		DensoRob.pIRobot->Move(2,CComVariant(DesPos),CComBSTR(L""));

		//�ɼ�ͼ��������ĩ��λ��
		m_vision1.CalcPos(FeatureNext[0],FeatureNext[1]);//���1��ȡ�ƶ�delta_pos��ĵ�ǰ����
		m_vision2.CalcPos(FeatureNext[2],FeatureNext[3]);//���1��ȡ�ƶ�delta_pos��ĵ�ǰ����
		for (int j=0;j<4;j++)
		{
			DeltaFeature[j][i] = FeatureNext[j] - FeatureCurrent[j];//����ƫ��
		}
		DeltaPos[i][i] = delta_pos;
        //�˶�ԭλ��
		DensoRob.SetToolPos(PosStartArrayCopy);//����ĩ��λ��ת��Ϊ��е��ĩ��λ��(����任)
		DensoRob.ArrayToPos(PosStartArrayCopy,DesPos);//����ĩ�˵���λ��������λ������
		DensoRob.pIRobot->Move(2,CComVariant(DesPos),CComBSTR(L""));
		/*
		CComVariant����ATL��VARIANT�������͵�һ����װ��
		���ڲ�ά����һ��VARIANT���ݽṹ��
		�������ǿ��Ժܷ�����ڴ�����������CComVariant�����VARIANT��
		CComBSTR �ṩ�� BSTR�������͵İ�װ��
		��׼BSTR��һ���г���ǰ׺��null��������OLECHAR���顣
		BSTR��ǰ4�ֽ���һ����ʾ�ַ������ȵ�ǰ׺��
		BSTR�������ֵ���ַ������ֽ��������Ҳ�����0��������
		*/
		//��ԭλ��ֵ
		DensoRob.GetCurrentToolPos(PosStartArrayCopy);	//��������ϵĩ��λ��
		DensoRob.GetCurrentToolPos(PosStartArray);//�õ��ƶ�delta_pos�󹤾�ĩ�˵�λ��
	}

	//�������ռ�,��Ҫ����ת���ɾ�����ʽ,CV_8UC1 ��ʾ8λ�޷��ŵ�ͨ������, CV_32SC2��ʾ32λ�з���˫ͨ������.
	//����ͼ���ſ˱Ⱦ���
	CvMat* DeltaFeatureM = cvCreateMat(4,3,CV_64FC1);    //�������ռ�,��Ҫ����ת���ɾ�����ʽ,CV_8UC1 ��ʾ8λ�޷��ŵ�ͨ������, CV_32SC2��ʾ32λ�з���˫ͨ������.
	CvMat* DeltaPosM = cvCreateMat(3,3,CV_64FC1);    //�������ռ�,��Ҫ����ת���ɾ�����ʽ,CV_8UC1 ��ʾ8λ�޷��ŵ�ͨ������, CV_32SC2��ʾ32λ�з���˫ͨ������.
	CvMat* DeltaPosInvM = cvCreateMat(3,3,CV_64FC1);//DeltaPosM����������    //�������ռ�,��Ҫ����ת���ɾ�����ʽ,CV_8UC1 ��ʾ8λ�޷��ŵ�ͨ������, CV_32SC2��ʾ32λ�з���˫ͨ������.

	for (int i = 0; i < DeltaFeatureM->rows; i++) //��
	{
		for (int j = 0; j < DeltaFeatureM->cols; j++) //��
		{//��(i, j)��Ԫ�ظ�ֵ��һ������Ԫ�أ�����ȡ�������е�Ԫ�����һ��һά���顣
			cvmSet(DeltaFeatureM,i,j,DeltaFeature[i][j] );	//����ƫ��
		}
	}
	for (int i = 0; i < DeltaPosM->rows; i++) 
	{
		for (int j = 0; j < DeltaPosM->cols; j++) 
		{
			cvmSet(DeltaPosM,i,j,DeltaPos[i][j] );	//λ��ƫ�����cvmSet��һά���ά����ת������һ��С����
		}
	}
	cvInvert(DeltaPosM, DeltaPosInvM, CV_SVD);	//cvInvert����ȡ�棬CV_SVDΪ�����㷽���е�һ�֡�����ֵ�ֽ�
	cvmMul(DeltaFeatureM, DeltaPosInvM, JacobInit);
    DensoRob.pIRobot->Speed(-1,50);	//�����˶��ٶ�
	delete CurrentPos;	//һ����˵ʹ��new����Ķ�����delete���ͷš���new[] ������ڴ���delete [] ������ͷš�
};

//�ſɱȾ����ʼֵ
//������ĺ���һ���������ƹ����ģ���������ͬ����
void Controller::InitJacob1(CHalconVision& m_vision1,CHalconVision& m_vision2,CvMat *JacobInit)
{	//�����ſ˱Ⱦ���Ĵ��ã��ṹ��InitJacob����һ��
	Robot DensoRob;
	CStringArray *CurrentPos = new CStringArray;
	CString DesPos;
	CHalconVision CamVision;
	double PosStartArray[7] = {0};
	double PosDelta[7] = {0};
	double delta_pos = 5;   //λ������
	double FeatureCurrent[4] = {0};   //��ǰ����
	double FeatureNext[4] = {0};   //��һ������
	double DeltaFeature[4][3] = {0};   //����ƫ��
	double DeltaPos[3][3] = {0};   //λ��ƫ��
	double PosStartArrayCopy[7] = {0};
	DensoRob.Init();
	DensoRob.pIRobot->Move(2,CComVariant("@0 P0"),CComBSTR(L""));
	DensoRob.pIRobot->Speed(-1,20);
	Sleep(500);
	DensoRob.GetCurrentToolPos(PosStartArray);//���湤������ϵĩ�˵�ǰλ��
	DensoRob.GetCurrentToolPos(PosStartArrayCopy);

	//��ǰͼ������
	m_vision1.CalcPos(FeatureCurrent[0],FeatureCurrent[1]);
	m_vision2.CalcPos(FeatureCurrent[2],FeatureCurrent[3]);
	
	for(int i = 0; i<3;i++)
	{
		PosStartArray[i] += delta_pos;   //delta_pos = 5  //λ������
		DensoRob.SetToolPos(PosStartArray);    //���û�е��ĩ��λ��
		DensoRob.ArrayToPos(PosStartArray,DesPos);   //�õ���е��ĩ�˽�Ҫ�ƶ�����λ������
		DensoRob.pIRobot->Move(2,CComVariant(DesPos),CComBSTR(L""));

		//��һ��ͼ������
		m_vision1.CalcPos(FeatureNext[0],FeatureNext[1]);
		m_vision2.CalcPos(FeatureNext[2],FeatureNext[3]);

		for (int j=0;j<4;j++)
		{
			DeltaFeature[j][i] = FeatureNext[j] - FeatureCurrent[j] ;   //����ƫ��
		}
		DeltaPos[i][i] = delta_pos;     //λ��ƫ��
        //�˶�ԭλ��
		DensoRob.SetToolPos(PosStartArrayCopy);    //���û�е��ĩ��λ��
		DensoRob.ArrayToPos(PosStartArrayCopy,DesPos);   //�õ���е��ĩ�˽�Ҫ�ƶ�����λ������
		DensoRob.pIRobot->Move(2,CComVariant(DesPos),CComBSTR(L""));
		//��ԭλ��ֵ
		DensoRob.GetCurrentToolPos(PosStartArrayCopy);   //�õ�����ĩ�˵�ǰλ��
		DensoRob.GetCurrentToolPos(PosStartArray);   //�õ�����ĩ�˵�ǰλ��
	}

	//����ͼ���ſ˱Ⱦ���
	CvMat* DeltaFeatureM = cvCreateMat(4,3,CV_64FC1);    //�������ռ�,��Ҫ����ת���ɾ�����ʽ,CV_8UC1 ��ʾ8λ�޷��ŵ�ͨ������, CV_32SC2��ʾ32λ�з���˫ͨ������.
	CvMat* DeltaPosM = cvCreateMat(3,3,CV_64FC1);    //�������ռ�,��Ҫ����ת���ɾ�����ʽ,CV_8UC1 ��ʾ8λ�޷��ŵ�ͨ������, CV_32SC2��ʾ32λ�з���˫ͨ������.
	CvMat* DeltaPosInvM = cvCreateMat(3,3,CV_64FC1);    //�������ռ�,��Ҫ����ת���ɾ�����ʽ,CV_8UC1 ��ʾ8λ�޷��ŵ�ͨ������, CV_32SC2��ʾ32λ�з���˫ͨ������.

	for (int i = 0; i < DeltaFeatureM->rows; i++) 
	{
		for (int j = 0; j < DeltaFeatureM->cols; j++) 
		{
			cvmSet(DeltaFeatureM,i,j,DeltaFeature[i][j] );	//����ƫ�����cvmSet��һά���ά����ת������һ��С����
		}
	}
		for (int i = 0; i < DeltaPosM->rows; i++) 
	{
		for (int j = 0; j < DeltaPosM->cols; j++) 
		{
			cvmSet(DeltaPosM,i,j,DeltaPos[i][j] );	//λ��ƫ�����cvmSet��һά���ά����ת������һ��С����
		}
	}
	cvInvert(DeltaPosM, DeltaPosInvM, CV_SVD);	//cvInvert����ȡ�棬CV_SVDΪ�����㷽���е�һ�֡�����ֵ�ֽ�
	cvmMul(DeltaFeatureM, DeltaPosInvM, JacobInit);
    DensoRob.pIRobot->Speed(-1,50);	//�����˶��ٶ�
	delete CurrentPos;    //ǰ����new
};

void Controller::CalDesirePos(CHalconVision& m_vision1,CHalconVision& m_vision2,double* PosToolArray)
{

	Robot DensoRob;		//����һ��robot����
	CStringArray *CurrentPos = new CStringArray;	//MFC CStringArray �ַ��������࣬���嵱ǰλ�á� C++��new��������ڶ�̬����ͳ����ڴ���������
	double FeatureCurrent[4] = {0};//��ǰ����
	double DeltaFeature[4] = {0};//����ƫ��
	//double PosToolArray[7] = {0};
	double PosxyzCurrent[3] = {0};//��ǰλ��
	double DeltaPosxyz[3] = {0};//λ��ƫ��
	double DeltaU[7] = {0};
	
	static double PosxyzLast[3] = {0};//��һʱ�̵�λ��
	static double FeatureLast[4] = {0};//��һʱ������

	static int flagFeature = 0;//������־λ
	static int flagPosxyz = 0;//λ�ñ�־λ
	static int flagJacobInit = 0;//�ſɱȾ����־λ
	CvMat* JcobNext = cvCreateMat(4,3,CV_64FC1);    //��һʱ���ſɱȾ���
    CvMat* Jcob_filter = cvCreateMat(12,1,CV_64FC1);  //Kalman�˲����ſɱȾ���

	//�ɼ�ͼ������ƫ������ĩ��λ��ƫ����
	m_vision1.CalcPos(FeatureCurrent[0],FeatureCurrent[1]);
	m_vision2.CalcPos(FeatureCurrent[2],FeatureCurrent[3]);

	if (flagFeature == 0)
	{//Ϊ�˵�һ��ִ�д˺���ʱ����֤��һʱ�̵�������Ϊ0������ֵΪ��ǰʱ�̵�����
		for (int i=0;i<4;i++)
		{
			FeatureLast[i] = FeatureCurrent[i];
		}
		flagFeature = 1;
	}

	for (int i = 0;i<4;i++)
	{
		DeltaFeature[i] = FeatureCurrent[i] - FeatureLast[i];	//������ƫ��
	}
	for (int i=0;i<4;i++)
	{
		FeatureLast[i] = FeatureCurrent[i];//����Ϊ��һʱ�̵�����
	}

	DensoRob.Init(); //robot�����ʼ��
	DensoRob.GetCurrentToolPos(PosToolArray);//���湤��ĩ�˵�ǰλ��
	for(int i = 0;i<3;i++)
	{
		PosxyzCurrent[i] = PosToolArray[i];//��ǰλ��
	}
	if (flagPosxyz == 0)
	{

		for (int i=0;i<3;i++)
		{
			PosxyzLast[i] = PosxyzCurrent[i];//����һ��if��������
		}
		flagPosxyz = 1;
	}
	for (int i = 0;i<3;i++)
	{
		DeltaPosxyz[i] = PosxyzCurrent[i] - PosxyzLast[i];//λ��ƫ��
	}

	for (int i = 0;i<3;i++)
	{
		PosxyzLast[i] = PosxyzCurrent[i] ;//����Ϊ��һλ��
	}

	//����ͼ���ſ˱Ⱦ���
	static double DeltaLastPosxyz[3] = {0};//��һ�μ������λ��ƫ��
	static int flagdeltapos = 0;//λ��ƫ���־λ
	if (flagdeltapos == 0)
	{
		for (int i = 0;i<3;i++)
		{
			DeltaLastPosxyz[i] = DeltaPosxyz[i];//��ǰ��if��������
		}
		flagdeltapos = 1;
	}
	for(int i = 0;i<7;i++)
	{
		if (i<3)
			DeltaU[i] = DeltaLastPosxyz[i];//ǰ��3��Ԫ��Ϊ��һ�ε�λ��ƫ��
		if (i>=3)
			DeltaU[i] = DeltaFeature[i-3];//��4��Ԫ��Ϊ����ƫ��
	}
	for (int i = 0;i<3;i++)
	{
		DeltaLastPosxyz[i] = DeltaPosxyz[i];//����Ϊ��һ��λ��ƫ��
	}

    Kalman JcobKalman;//����һ��Kalman����
	if (flagJacobInit == 0)
	{
		JcobKalman.Init(kalmanJacobInit);  //��ǰ��if�������ƣ�Kalman�����ʼ��
		flagJacobInit = 1;
	}

	*Jcob_filter = JcobKalman.kalmanFilter(DeltaU);    //Kalman�˲����ſɱȾ���
	int k=0;
    for (int i = 0; i < JcobNext->rows; i++) 
	{
		for (int j = 0; j < JcobNext->cols; j++) 
		{
			cvmSet(JcobNext,i,j,cvmGet(Jcob_filter, k++,0) );
		}
	}

	// ��������ƫ��������λ��
	for (int i=0;i<4;i++)
	{
		DeltaFeature[i] = DesireFeature[i] - FeatureCurrent[i];
	}

	//�Ӿ����������
   double lamda[4] = {0.09,0.09,0.09,0.09};
	//double lamda[4] = {0.055,0.055,0.055,0.055};
	if ((abs(DeltaFeature[0])<=1)&&(abs(DeltaFeature[1])<=1)&&(abs(DeltaFeature[2])<=1)&&(abs(DeltaFeature[3])<=1))//stop control
	{//stop control
		for (int i=0;i<4;i++)
			lamda[i] = 0;
	}

	//Kalman parameter P Q R switch flag//Kalman����P-Q-R���ر�־
	if ((abs(DeltaFeature[0])<=10)&&(abs(DeltaFeature[1])<=10)&&(abs(DeltaFeature[2])<=10)&&(abs(DeltaFeature[3])<=10))
	{    
		for(int i=0;i<4;i++)
		lamda[i] = 0.05;
		Switch_PRQ_flag = 1;
	}

	for (int i=0;i<4;i++)
	{
		DeltaFeature[i] *= lamda[i];//����ƫ�����ĳϵ��
	}

	CvMat* DeltaFeatureM = cvCreateMat(4,1,CV_64FC1);
	CvMat* PosVelM = cvCreateMat(3,1,CV_64FC1);
	CvMat* LamdaPosVelM = cvCreateMat(3,1,CV_64FC1);
	//ͼ������ƫ�����
	for (int i = 0; i < DeltaFeatureM->rows; i++) 
	{
		for (int j = 0; j < DeltaFeatureM->cols; j++) 
		{
			cvmSet(DeltaFeatureM,i,j,DeltaFeature[i] );
		}
	}

	CvMat* JacobCurrent_inverse = cvCreateMat( 3, 4, CV_64FC1 );//���ſɱȾ���
    cvInvert(JacobCurrent, JacobCurrent_inverse, CV_SVD);//�����ſɱȾ���

	cvmMul(JacobCurrent_inverse,DeltaFeatureM, PosVelM); 
	//cvmMul(JacobCurrent_inverse,DeltaFeatureM, LamdaPosVelM);
	//cvmMul(JacobCurrent,DeltaFeatureM, PosVelM);  
	cvConvertScale(PosVelM,LamdaPosVelM, 1.5, 0 ); //��������ĩ���˶��ٶ�

	double PosVel[3] = {0};
	for (int i = 0; i < LamdaPosVelM->rows; i++) 
	{
		for (int j = 0; j < LamdaPosVelM->cols; j++) 
		{
			PosVel[i]=cvmGet(LamdaPosVelM, i, j);
		}
	}
	
	for (int i=0;i<3;i++)
	{
		PosToolArray[i] += PosVel[i];//��������ĩ��λ��
	}
	//DensoRob.SetToolPos(PosToolArray);//ת��Ϊ��е��ĩ��λ��
	JacobCurrent = cvCloneMat(JcobNext);//���ƾ���
	delete CurrentPos;
};
