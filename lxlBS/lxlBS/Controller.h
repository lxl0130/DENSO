#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#pragma once
#include "HalconVision.h"
#include "cv.h"

class Controller
{
public:
	double kalmanJacobInit[12];
	double DesireFeature[4];   //��������
	static CvMat* JacobCurrent;   //��ǰ�ſ˱Ⱦ���
	void Init(CHalconVision& m_vision1,CHalconVision& m_vision2,double *desirefeature);   //��ʼ��
	Controller(void);
	~Controller(void);
	void InitJacob(CHalconVision& m_vision1,CHalconVision& m_vision2,CvMat *JacobInit);   	//�����ſ˱Ⱦ���Ĵ���
	void CalDesirePos(CHalconVision& m_vision1,CHalconVision& m_vision2,double *PosToolArray);
	void InitJacob1(CHalconVision& m_vision1,CHalconVision& m_vision2,CvMat *JacobInit);    	//�����ſ˱Ⱦ���Ĵ���
	static int Switch_PRQ_flag ;    //��̬����
};

#endif
