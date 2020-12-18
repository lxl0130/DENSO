#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#pragma once
#include "HalconVision.h"
#include "cv.h"

class Controller
{
public:
	double kalmanJacobInit[12];
	double DesireFeature[4];   //期望特征
	static CvMat* JacobCurrent;   //当前雅克比矩阵
	void Init(CHalconVision& m_vision1,CHalconVision& m_vision2,double *desirefeature);   //初始化
	Controller(void);
	~Controller(void);
	void InitJacob(CHalconVision& m_vision1,CHalconVision& m_vision2,CvMat *JacobInit);   	//估计雅克比矩阵的处置
	void CalDesirePos(CHalconVision& m_vision1,CHalconVision& m_vision2,double *PosToolArray);
	void InitJacob1(CHalconVision& m_vision1,CHalconVision& m_vision2,CvMat *JacobInit);    	//估计雅克比矩阵的处置
	static int Switch_PRQ_flag ;    //静态变量
};

#endif
