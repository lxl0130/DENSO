#ifndef _LXL_H_
#define _LXL_H_

#pragma once

//#include "CAO.h"
#include <atlbase.h>
#include <afxtempl.h>
#include <afxcoll.h>
#include <atlcomcli.h>

class lxl
{
public:
	lxl(void);//���캯��
	~lxl(void);//��������

	//CComPtr���ڹ��� COM �ӿ�ָ�������ָ���ࡣ��COM �ӿ�pointer��
	//CComPtr<ICaoEngine>         m_pIEng;
	//CComPtr<ICaoWorkspaces>		m_pIWss;
	//CComPtr<ICaoWorkspace>		m_pIWs;
	//CComPtr<ICaoControllers>	m_pICtrls;
	//CComPtr<ICaoController>	    pICtrl;
	CComPtr<ICaoRobot>	        pIRobot;
	/*
	CComPtr����Ϊ����ָ�룬��ATL�ṩ��һ��ģ���࣬�ܹ����﷨���Զ����AddRef��Release��
	��Դ������atlbase.h�У�
	CComPtr���÷��ܼ򵥣���IHello*Ϊ����
	�����������нӿ�ָ�����ͣ����˲���������ʹ��CComPtr<IHello> ���漴�ɡ�
	�������г��˲���֮�⣬��Ҳ��Ҫʹ��IHello*��ȫ����CComPtr<IHello>���档
	*/

//
//	bool Init(void);	//robot�����ʼ��
//	bool Release(void);		//�ͷ��ڴ�
//
//	void GetCurPos(double* pdPos);	//	��õ�ǰλ��
//
//	void Twist(int pTarget, int time);	//ĩ��toolת��
//	void TwistTest();	////ĩ��toolת������
//
//
//private:
//public:
//	void GetIOStatus(int IO_Index, VARIANT_BOOL& IO_Status);//��ȡIO״̬
//
//
//	bool IsRobSlaveRunning(void);//�������Ƿ���������
//	CComPtr<ICaoRobot> GetRobotObject(void);//��û����˶���
//public:
//	void Unload(int pTarget, int time);//ж����ж��
//	void WellOpend(int pTarget);
//	void TakeTool(int pTarget);//���𹤾ߣ�Я������
//	void OnApproach(int pTarget, double length);//���ڿ���
//	void ReplaceTool(int pTarget);//��������
//	void GetCurrentPosition(CStringArray* CurrentPos);//�õ���ǰλ��
//	void PosToArray(CStringArray* Current_pos, double* pos);//λ�õ�����
//	void ArrayToPos(double* pos, CString& Des_pos);//���е�λ��
//	void OnMoveH(double* Relate_pos);
//	void OnDepart(double length);//���ڷ���
//	void OnDepartEx(double Length);
//	void SetIO(int IO_Index, bool IO_Status);//����IO
//
//	static double ClosePos[7];
//	static double OpenPos[7];
//	static double StartPos[7];
//	static double Start_Pos_well1[7];  //close well1
//	static double Close_Pos_well1[7];
//	static double Screw1Pos[7];            //save the position of Screw1 (Big) 
//	static double SmallScrew1Pos[7];        //save the position of Screw1 (small) 
//
//	void WellClose(void);
//	void GetCurrentTrans(CStringArray* CurrentPos);//��õ�ǰת��ʽ
//	void TransToArray(CStringArray* Current_pos, double* pos);//ת��������
//	void GetVec(double* Current_pos, CString& Current_vec);//�������
//
//	void Well1Close(void);
//	//small radiation rotate use
//	void RotateArrayToPos(double* pos, CString& Des_pos);//��ת����λ��
//	void GetCurrentToolPos(double* position);	//��ù�������ϵĩ�˵�ǰλ��
//	void SetToolPos(double* positon);//����ĩ��λ��ת��Ϊ��е��ĩ��λ��(����任)
//
//	//5DOF 
//	void GetCurrentJnt(double* Jnt);
//	void JntToArray(CStringArray* Current_Jnt, double* Jnt);
//	void ArrayToJnt(double* Jnt, CString& Des_Jnt);



	//��С��
	void SetToolPos(double* position);
	void ArrayToPos(double* pos, CString& Des_pos);
	void GetCurrentTrans(CStringArray* CurrentPos)








};







#endif