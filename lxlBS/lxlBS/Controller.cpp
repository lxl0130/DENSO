//机械臂控制器实现文件

#include "StdAfx.h"
#include "Controller.h"
#include"Robot.h"
//#include "HalconVision.h"
#include "Kalman.h"
/*
注：浅蓝色为类名，浅灰色为类对象，中灰色为函数
*/

//cvCreateMat分配矩阵空间,主要用来转换成矩阵形式,CV_8UC1 表示8位无符号单通道矩阵, CV_32SC2表示32位有符号双通道矩阵.
CvMat* Controller::JacobCurrent = cvCreateMat(4,3,CV_64FC1);	//为当前雅克比矩阵分配矩阵空间4×3
int Controller::Switch_PRQ_flag = 0 ;  

Controller::Controller(void)
{
}

Controller::~Controller(void)
{
}

void Controller::Init(CHalconVision& m_vision1,CHalconVision& m_vision2,double *desirefeature)        //初始化
{
	CvMat* JacobInit= cvCreateMat(4,3,CV_64FC1);	//声明一个雅克比矩阵，分配矩阵空间4×3

	for (int i =0;i<4;i++)
	{
		DesireFeature[i]=desirefeature[i];	//输入的期望特征赋值给一个变量数组
	}

	InitJacob(m_vision1,m_vision2,JacobInit);	//雅可比矩阵初始值

	/*cvCloneImage与cvCloneMat是在赋值的同时会开辟一个新的空间给定义的变量，
	cvCopyImage与cvCopyMat只复制值，并不会分配一个空间给赋值对象，
	因此cvCloneImage与cvCloneMat只适合用于变量开始定义，千万不要用在算法处理中间，
	否则会产生一个新的地址空间，会将赋值对象的指针地址改变，
	在程序中间进行复制时候建议使用cvCopyImage与cvCopyMat。*/
	JacobCurrent = cvCloneMat(JacobInit);	//赋值且开辟空间，定义当前雅克比矩阵

	int k=0;
	for (int i = 0; i < JacobInit->rows; i++) 
	{
		for (int j = 0; j < JacobInit->cols; j++) 
		{	//第(i, j)个元素赋值给一个数组元素，即提取出矩阵中的元素组成一个一维数组。
			kalmanJacobInit[k++] = cvmGet(JacobInit,i,j);
		}
	}
};

void Controller::InitJacob(CHalconVision& m_vision1,CHalconVision& m_vision2,CvMat *JacobInit)	//雅可比矩阵初始值
{
	Robot DensoRob;	//声明一个robot对象
	CStringArray *CurrentPos = new CStringArray;	//MFC CStringArray 字符串数组类，定义当前位置。 C++中new运算符用于动态分配和撤销内存的运算符。
	CString DesPos;		//声明期望位置
	CHalconVision CamVision;	//声明一个相机对象
	
	double PosStartArray[7] = {0};	//工具末端起始位置
	double PosDelta[7] = {0};	//工具末端位置偏差数组
	double delta_pos = 10;	//位置增量

	double FeatureCurrent[4] = {0};	//定义一个当前特征数组
	double FeatureNext[4] = {0};	//下一次特征
	double DeltaFeature[4][3] = {0};	//特征偏差
	
	double DeltaPos[3][3] = {0};	//位置偏差矩阵
	double PosStartArrayCopy[7] = {0};	//起始位置复制数组

	DensoRob.Init();	//robot对象初始化，即设备连接是否正常
	DensoRob.pIRobot->Move(2,CComVariant("@0 P0"),CComBSTR(L""));    //移动到起始位置
	DensoRob.pIRobot->Speed(-1,20);   //设置运动速度
	Sleep(500);	//延时
	
	DensoRob.GetCurrentToolPos(PosStartArray);	//获得工具坐标系末端开始位置
	DensoRob.GetCurrentToolPos(PosStartArrayCopy);	//复制保存工具坐标系末端最开始位置
	
	//采集图像特征与末端位置
	m_vision1.CalcPos(FeatureCurrent[0],FeatureCurrent[1]);//相机1获取的当前特征，  //计算坐标处位置。
	m_vision2.CalcPos(FeatureCurrent[2],FeatureCurrent[3]);//相机1获取的当前特征，  //计算坐标处位置。
	
	for(int i = 0; i<3;i++)
	{
		PosStartArray[i] += delta_pos;//工具末端位置不断变化，delta_pos = 10
		DensoRob.SetToolPos(PosStartArray);//工具末端位置转换为机械臂末端位置(坐标变换)
		DensoRob.ArrayToPos(PosStartArray,DesPos);//工具末端当期位置与期望位姿数组

		DensoRob.pIRobot->Move(2,CComVariant(DesPos),CComBSTR(L""));

		//采集图像特征与末端位置
		m_vision1.CalcPos(FeatureNext[0],FeatureNext[1]);//相机1获取移动delta_pos后的当前特征
		m_vision2.CalcPos(FeatureNext[2],FeatureNext[3]);//相机1获取移动delta_pos后的当前特征
		for (int j=0;j<4;j++)
		{
			DeltaFeature[j][i] = FeatureNext[j] - FeatureCurrent[j];//特征偏差
		}
		DeltaPos[i][i] = delta_pos;
        //运动原位置
		DensoRob.SetToolPos(PosStartArrayCopy);//工具末端位置转换为机械臂末端位置(坐标变换)
		DensoRob.ArrayToPos(PosStartArrayCopy,DesPos);//工具末端当期位置与期望位姿数组
		DensoRob.pIRobot->Move(2,CComVariant(DesPos),CComBSTR(L""));
		/*
		CComVariant类是ATL对VARIANT数据类型的一个包装，
		它内部维护了一个VARIANT数据结构。
		所以我们可以很方便的在大多数情况下用CComVariant来替代VARIANT。
		CComBSTR 提供对 BSTR数据类型的包装。
		标准BSTR是一个有长度前缀和null结束符的OLECHAR数组。
		BSTR的前4字节是一个表示字符串长度的前缀。
		BSTR长度域的值是字符串的字节数，并且不包括0结束符。
		*/
		//还原位置值
		DensoRob.GetCurrentToolPos(PosStartArrayCopy);	//工具坐标系末端位置
		DensoRob.GetCurrentToolPos(PosStartArray);//得到移动delta_pos后工具末端的位置
	}

	//分配矩阵空间,主要用来转换成矩阵形式,CV_8UC1 表示8位无符号单通道矩阵, CV_32SC2表示32位有符号双通道矩阵.
	//计算图像雅克比矩阵
	CvMat* DeltaFeatureM = cvCreateMat(4,3,CV_64FC1);    //分配矩阵空间,主要用来转换成矩阵形式,CV_8UC1 表示8位无符号单通道矩阵, CV_32SC2表示32位有符号双通道矩阵.
	CvMat* DeltaPosM = cvCreateMat(3,3,CV_64FC1);    //分配矩阵空间,主要用来转换成矩阵形式,CV_8UC1 表示8位无符号单通道矩阵, CV_32SC2表示32位有符号双通道矩阵.
	CvMat* DeltaPosInvM = cvCreateMat(3,3,CV_64FC1);//DeltaPosM矩阵的逆矩阵    //分配矩阵空间,主要用来转换成矩阵形式,CV_8UC1 表示8位无符号单通道矩阵, CV_32SC2表示32位有符号双通道矩阵.

	for (int i = 0; i < DeltaFeatureM->rows; i++) //行
	{
		for (int j = 0; j < DeltaFeatureM->cols; j++) //列
		{//第(i, j)个元素赋值给一个数组元素，即提取出矩阵中的元素组成一个一维数组。
			cvmSet(DeltaFeatureM,i,j,DeltaFeature[i][j] );	//特征偏差
		}
	}
	for (int i = 0; i < DeltaPosM->rows; i++) 
	{
		for (int j = 0; j < DeltaPosM->cols; j++) 
		{
			cvmSet(DeltaPosM,i,j,DeltaPos[i][j] );	//位置偏差矩阵，cvmSet把一维或多维数组转换成另一大小矩阵
		}
	}
	cvInvert(DeltaPosM, DeltaPosInvM, CV_SVD);	//cvInvert矩阵取逆，CV_SVD为逆运算方法中的一种—奇异值分解
	cvmMul(DeltaFeatureM, DeltaPosInvM, JacobInit);
    DensoRob.pIRobot->Speed(-1,50);	//设置运动速度
	delete CurrentPos;	//一般来说使用new分配的对象，用delete来释放。用new[] 分配的内存用delete [] 来逐个释放。
};

//雅可比矩阵初始值
//跟上面的函数一样，即复制过来的，函数名不同而已
void Controller::InitJacob1(CHalconVision& m_vision1,CHalconVision& m_vision2,CvMat *JacobInit)
{	//估计雅克比矩阵的处置，结构与InitJacob函数一样
	Robot DensoRob;
	CStringArray *CurrentPos = new CStringArray;
	CString DesPos;
	CHalconVision CamVision;
	double PosStartArray[7] = {0};
	double PosDelta[7] = {0};
	double delta_pos = 5;   //位置增量
	double FeatureCurrent[4] = {0};   //当前特征
	double FeatureNext[4] = {0};   //下一刻特征
	double DeltaFeature[4][3] = {0};   //特征偏差
	double DeltaPos[3][3] = {0};   //位置偏差
	double PosStartArrayCopy[7] = {0};
	DensoRob.Init();
	DensoRob.pIRobot->Move(2,CComVariant("@0 P0"),CComBSTR(L""));
	DensoRob.pIRobot->Speed(-1,20);
	Sleep(500);
	DensoRob.GetCurrentToolPos(PosStartArray);//保存工具坐标系末端当前位置
	DensoRob.GetCurrentToolPos(PosStartArrayCopy);

	//当前图像特征
	m_vision1.CalcPos(FeatureCurrent[0],FeatureCurrent[1]);
	m_vision2.CalcPos(FeatureCurrent[2],FeatureCurrent[3]);
	
	for(int i = 0; i<3;i++)
	{
		PosStartArray[i] += delta_pos;   //delta_pos = 5  //位置增量
		DensoRob.SetToolPos(PosStartArray);    //设置机械臂末端位置
		DensoRob.ArrayToPos(PosStartArray,DesPos);   //得到机械臂末端将要移动到的位置数组
		DensoRob.pIRobot->Move(2,CComVariant(DesPos),CComBSTR(L""));

		//下一刻图像特征
		m_vision1.CalcPos(FeatureNext[0],FeatureNext[1]);
		m_vision2.CalcPos(FeatureNext[2],FeatureNext[3]);

		for (int j=0;j<4;j++)
		{
			DeltaFeature[j][i] = FeatureNext[j] - FeatureCurrent[j] ;   //特征偏差
		}
		DeltaPos[i][i] = delta_pos;     //位置偏差
        //运动原位置
		DensoRob.SetToolPos(PosStartArrayCopy);    //设置机械臂末端位置
		DensoRob.ArrayToPos(PosStartArrayCopy,DesPos);   //得到机械臂末端将要移动到的位置数组
		DensoRob.pIRobot->Move(2,CComVariant(DesPos),CComBSTR(L""));
		//还原位置值
		DensoRob.GetCurrentToolPos(PosStartArrayCopy);   //得到工具末端当前位置
		DensoRob.GetCurrentToolPos(PosStartArray);   //得到工具末端当前位置
	}

	//计算图像雅克比矩阵
	CvMat* DeltaFeatureM = cvCreateMat(4,3,CV_64FC1);    //分配矩阵空间,主要用来转换成矩阵形式,CV_8UC1 表示8位无符号单通道矩阵, CV_32SC2表示32位有符号双通道矩阵.
	CvMat* DeltaPosM = cvCreateMat(3,3,CV_64FC1);    //分配矩阵空间,主要用来转换成矩阵形式,CV_8UC1 表示8位无符号单通道矩阵, CV_32SC2表示32位有符号双通道矩阵.
	CvMat* DeltaPosInvM = cvCreateMat(3,3,CV_64FC1);    //分配矩阵空间,主要用来转换成矩阵形式,CV_8UC1 表示8位无符号单通道矩阵, CV_32SC2表示32位有符号双通道矩阵.

	for (int i = 0; i < DeltaFeatureM->rows; i++) 
	{
		for (int j = 0; j < DeltaFeatureM->cols; j++) 
		{
			cvmSet(DeltaFeatureM,i,j,DeltaFeature[i][j] );	//特征偏差矩阵，cvmSet把一维或多维数组转换成另一大小矩阵
		}
	}
		for (int i = 0; i < DeltaPosM->rows; i++) 
	{
		for (int j = 0; j < DeltaPosM->cols; j++) 
		{
			cvmSet(DeltaPosM,i,j,DeltaPos[i][j] );	//位置偏差矩阵，cvmSet把一维或多维数组转换成另一大小矩阵
		}
	}
	cvInvert(DeltaPosM, DeltaPosInvM, CV_SVD);	//cvInvert矩阵取逆，CV_SVD为逆运算方法中的一种—奇异值分解
	cvmMul(DeltaFeatureM, DeltaPosInvM, JacobInit);
    DensoRob.pIRobot->Speed(-1,50);	//设置运动速度
	delete CurrentPos;    //前面有new
};

void Controller::CalDesirePos(CHalconVision& m_vision1,CHalconVision& m_vision2,double* PosToolArray)
{

	Robot DensoRob;		//声明一个robot对象
	CStringArray *CurrentPos = new CStringArray;	//MFC CStringArray 字符串数组类，定义当前位置。 C++中new运算符用于动态分配和撤销内存的运算符。
	double FeatureCurrent[4] = {0};//当前特征
	double DeltaFeature[4] = {0};//特征偏差
	//double PosToolArray[7] = {0};
	double PosxyzCurrent[3] = {0};//当前位置
	double DeltaPosxyz[3] = {0};//位置偏差
	double DeltaU[7] = {0};
	
	static double PosxyzLast[3] = {0};//上一时刻的位置
	static double FeatureLast[4] = {0};//上一时刻特征

	static int flagFeature = 0;//特征标志位
	static int flagPosxyz = 0;//位置标志位
	static int flagJacobInit = 0;//雅可比矩阵标志位
	CvMat* JcobNext = cvCreateMat(4,3,CV_64FC1);    //下一时刻雅可比矩阵
    CvMat* Jcob_filter = cvCreateMat(12,1,CV_64FC1);  //Kalman滤波后雅可比矩阵

	//采集图像特征偏差量与末端位置偏差量
	m_vision1.CalcPos(FeatureCurrent[0],FeatureCurrent[1]);
	m_vision2.CalcPos(FeatureCurrent[2],FeatureCurrent[3]);

	if (flagFeature == 0)
	{//为了第一次执行此函数时，保证上一时刻的特征不为0，即赋值为当前时刻的特征
		for (int i=0;i<4;i++)
		{
			FeatureLast[i] = FeatureCurrent[i];
		}
		flagFeature = 1;
	}

	for (int i = 0;i<4;i++)
	{
		DeltaFeature[i] = FeatureCurrent[i] - FeatureLast[i];	//求特征偏差
	}
	for (int i=0;i<4;i++)
	{
		FeatureLast[i] = FeatureCurrent[i];//保存为上一时刻的特征
	}

	DensoRob.Init(); //robot对象初始化
	DensoRob.GetCurrentToolPos(PosToolArray);//保存工具末端当前位置
	for(int i = 0;i<3;i++)
	{
		PosxyzCurrent[i] = PosToolArray[i];//当前位置
	}
	if (flagPosxyz == 0)
	{

		for (int i=0;i<3;i++)
		{
			PosxyzLast[i] = PosxyzCurrent[i];//跟上一个if作用类似
		}
		flagPosxyz = 1;
	}
	for (int i = 0;i<3;i++)
	{
		DeltaPosxyz[i] = PosxyzCurrent[i] - PosxyzLast[i];//位置偏差
	}

	for (int i = 0;i<3;i++)
	{
		PosxyzLast[i] = PosxyzCurrent[i] ;//保存为上一位置
	}

	//计算图像雅克比矩阵
	static double DeltaLastPosxyz[3] = {0};//上一次计算出的位置偏差
	static int flagdeltapos = 0;//位置偏差标志位
	if (flagdeltapos == 0)
	{
		for (int i = 0;i<3;i++)
		{
			DeltaLastPosxyz[i] = DeltaPosxyz[i];//跟前面if功能类似
		}
		flagdeltapos = 1;
	}
	for(int i = 0;i<7;i++)
	{
		if (i<3)
			DeltaU[i] = DeltaLastPosxyz[i];//前面3个元素为上一次的位置偏差
		if (i>=3)
			DeltaU[i] = DeltaFeature[i-3];//后4个元素为特征偏差
	}
	for (int i = 0;i<3;i++)
	{
		DeltaLastPosxyz[i] = DeltaPosxyz[i];//保存为上一次位置偏差
	}

    Kalman JcobKalman;//定义一个Kalman对象
	if (flagJacobInit == 0)
	{
		JcobKalman.Init(kalmanJacobInit);  //跟前面if功能类似，Kalman对象初始化
		flagJacobInit = 1;
	}

	*Jcob_filter = JcobKalman.kalmanFilter(DeltaU);    //Kalman滤波后雅可比矩阵
	int k=0;
    for (int i = 0; i < JcobNext->rows; i++) 
	{
		for (int j = 0; j < JcobNext->cols; j++) 
		{
			cvmSet(JcobNext,i,j,cvmGet(Jcob_filter, k++,0) );
		}
	}

	// 计算特征偏量与期望位置
	for (int i=0;i<4;i++)
	{
		DeltaFeature[i] = DesireFeature[i] - FeatureCurrent[i];
	}

	//视觉控制器设计
   double lamda[4] = {0.09,0.09,0.09,0.09};
	//double lamda[4] = {0.055,0.055,0.055,0.055};
	if ((abs(DeltaFeature[0])<=1)&&(abs(DeltaFeature[1])<=1)&&(abs(DeltaFeature[2])<=1)&&(abs(DeltaFeature[3])<=1))//stop control
	{//stop control
		for (int i=0;i<4;i++)
			lamda[i] = 0;
	}

	//Kalman parameter P Q R switch flag//Kalman参数P-Q-R开关标志
	if ((abs(DeltaFeature[0])<=10)&&(abs(DeltaFeature[1])<=10)&&(abs(DeltaFeature[2])<=10)&&(abs(DeltaFeature[3])<=10))
	{    
		for(int i=0;i<4;i++)
		lamda[i] = 0.05;
		Switch_PRQ_flag = 1;
	}

	for (int i=0;i<4;i++)
	{
		DeltaFeature[i] *= lamda[i];//特征偏差乘以某系数
	}

	CvMat* DeltaFeatureM = cvCreateMat(4,1,CV_64FC1);
	CvMat* PosVelM = cvCreateMat(3,1,CV_64FC1);
	CvMat* LamdaPosVelM = cvCreateMat(3,1,CV_64FC1);
	//图像特征偏差矩阵
	for (int i = 0; i < DeltaFeatureM->rows; i++) 
	{
		for (int j = 0; j < DeltaFeatureM->cols; j++) 
		{
			cvmSet(DeltaFeatureM,i,j,DeltaFeature[i] );
		}
	}

	CvMat* JacobCurrent_inverse = cvCreateMat( 3, 4, CV_64FC1 );//逆雅可比矩阵
    cvInvert(JacobCurrent, JacobCurrent_inverse, CV_SVD);//求逆雅可比矩阵

	cvmMul(JacobCurrent_inverse,DeltaFeatureM, PosVelM); 
	//cvmMul(JacobCurrent_inverse,DeltaFeatureM, LamdaPosVelM);
	//cvmMul(JacobCurrent,DeltaFeatureM, PosVelM);  
	cvConvertScale(PosVelM,LamdaPosVelM, 1.5, 0 ); //期望工具末端运动速度

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
		PosToolArray[i] += PosVel[i];//期望工具末端位置
	}
	//DensoRob.SetToolPos(PosToolArray);//转换为机械臂末端位置
	JacobCurrent = cvCloneMat(JcobNext);//复制矩阵
	delete CurrentPos;
};
