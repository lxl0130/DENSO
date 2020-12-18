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
	lxl(void);//构造函数
	~lxl(void);//析构函数

	//CComPtr用于管理 COM 接口指针的智能指针类。（COM 接口pointer）
	//CComPtr<ICaoEngine>         m_pIEng;
	//CComPtr<ICaoWorkspaces>		m_pIWss;
	//CComPtr<ICaoWorkspace>		m_pIWs;
	//CComPtr<ICaoControllers>	m_pICtrls;
	//CComPtr<ICaoController>	    pICtrl;
	CComPtr<ICaoRobot>	        pIRobot;
	/*
	CComPtr被称为智能指针，是ATL提供的一个模版类，能够从语法上自动完成AddRef和Release。
	（源代码在atlbase.h中）
	CComPtr的用法很简单，以IHello*为例，
	将程序中所有接口指针类型（除了参数），都使用CComPtr<IHello> 代替即可。
	即程序中除了参数之外，再也不要使用IHello*，全部以CComPtr<IHello>代替。
	*/

//
//	bool Init(void);	//robot对象初始化
//	bool Release(void);		//释放内存
//
//	void GetCurPos(double* pdPos);	//	获得当前位置
//
//	void Twist(int pTarget, int time);	//末端tool转动
//	void TwistTest();	////末端tool转动测试
//
//
//private:
//public:
//	void GetIOStatus(int IO_Index, VARIANT_BOOL& IO_Status);//获取IO状态
//
//
//	bool IsRobSlaveRunning(void);//机器人是否正在运行
//	CComPtr<ICaoRobot> GetRobotObject(void);//获得机器人对象
//public:
//	void Unload(int pTarget, int time);//卸货，卸料
//	void WellOpend(int pTarget);
//	void TakeTool(int pTarget);//拿起工具，携带工具
//	void OnApproach(int pTarget, double length);//正在靠近
//	void ReplaceTool(int pTarget);//更换工具
//	void GetCurrentPosition(CStringArray* CurrentPos);//得到当前位置
//	void PosToArray(CStringArray* Current_pos, double* pos);//位置到排列
//	void ArrayToPos(double* pos, CString& Des_pos);//排列到位置
//	void OnMoveH(double* Relate_pos);
//	void OnDepart(double length);//正在分离
//	void OnDepartEx(double Length);
//	void SetIO(int IO_Index, bool IO_Status);//设置IO
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
//	void GetCurrentTrans(CStringArray* CurrentPos);//获得当前转化式
//	void TransToArray(CStringArray* Current_pos, double* pos);//转换到矩阵
//	void GetVec(double* Current_pos, CString& Current_vec);//获得数组
//
//	void Well1Close(void);
//	//small radiation rotate use
//	void RotateArrayToPos(double* pos, CString& Des_pos);//旋转矩阵到位置
//	void GetCurrentToolPos(double* position);	//获得工具坐标系末端当前位置
//	void SetToolPos(double* positon);//工具末端位置转换为机械臂末端位置(坐标变换)
//
//	//5DOF 
//	void GetCurrentJnt(double* Jnt);
//	void JntToArray(CStringArray* Current_Jnt, double* Jnt);
//	void ArrayToJnt(double* Jnt, CString& Des_Jnt);



	//李小龙
	void SetToolPos(double* position);
	void ArrayToPos(double* pos, CString& Des_pos);
	void GetCurrentTrans(CStringArray* CurrentPos)








};







#endif