//#include "Controller.h"
//#include "Curve.h"
//#include "denso.h"
//#include "densoDlg.h"
//#include "HalconVision.h"
//#include "Kalman.h"
//#include "MyGroupBox.h"
//#include "robot.h"
//#include "stdafx.h"
//
//#include "CAO_i.c"
//
//#include "Robot.h"
//#include "StdAfx.h"

//#include "fstream"
#include "iostream"



#include <synchapi.h>
#include <atlcomcli.h>
#include <afxstr.h>
#include <afxwin.h>


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "lxl.h"

using namespace std;


void lxl::GetCurrentTrans(CStringArray* CurrentPos)
{

	long                    longIndex;
	long			        longlBound, longUBound;
	long                    i;
	CComPtr<ICaoRobots>	    pIRobots;
	CComPtr<ICaoVariable>	pIVar;
	CComPtr<ICaoVariables>	pIVars;
	CComVariant				varValue;
	HRESULT					hr;
	CString			        strWork;
	//	CStringArray	        *arrayResult;
	//	arrayResult = new CStringArray;

	SAFEARRAY* psa;

	//set io[27]

	hr = pIRobot->AddVariable(CComBSTR("@CURRENT_TRANS"), CComBSTR(L""), &pIVar);
	if (FAILED(hr)) {
		AfxMessageBox(_T("wang 11111Error : AddVariable\n"));
		return;
	}

	hr = pIVar->get_Value(&varValue);
	if (FAILED(hr)) {
		AfxMessageBox(_T("Error : get_Value\n"));
		return;
	}
	psa = varValue.parray;
	SafeArrayGetLBound(psa, 1, &longlBound);
	SafeArrayGetUBound(psa, 1, &longUBound);
	hr = SafeArrayLock(psa);
	if (SUCCEEDED(hr))
	{
		for (i = longlBound; i <= longUBound; i++)
		{
			strWork = "";
			float	fltResult;
			SafeArrayGetElement(psa, &i, &fltResult);
			strWork.Format(_T("%.3f"), fltResult);
			CurrentPos->Add(strWork);
		}
	}
	SafeArrayUnlock(psa);

	hr = pIVar->get_Index(&longIndex);
	if (SUCCEEDED(hr))
	{
		hr = pIRobot->get_Variables(&pIVars);
		if (FAILED(hr)) {
			AfxMessageBox(_T("Error : get_variables_Value\n"));
			return;
		}

		hr = pIVars->Remove(CComVariant(longIndex));
		if (FAILED(hr)) {
			AfxMessageBox(_T("Error : release_Value\n"));
			return;
		}
	}

	//	delete arrayResult;
	return;
}


void lxl::SetToolPos(double* position)
{
	int lTool = 166;
	CStringArray* CurrentTrans = new CStringArray;
	GetCurrentTrans(CurrentTrans);
	double x_tool;
	double y_tool;
	double z_tool;
	x_tool = _tcstod(CurrentTrans->GetAt(6), 0);
	y_tool = _tcstod(CurrentTrans->GetAt(7), 0);
	z_tool = _tcstod(CurrentTrans->GetAt(8), 0);
	position[0] -= lTool * x_tool;//x
	position[1] -= lTool * y_tool;//y
	position[2] -= lTool * z_tool;//z

	delete CurrentTrans;
}

//得到机械臂末端将要移动到的位置数组
void lxl::ArrayToPos(double* pos, CString& Des_pos)
{
	int i = 0;
	CString point;
	Des_pos = _T("P(");    //_T("")是一个宏,他的作用是让你的程序支持Unicode编码，因为Windows使用两种字符集ANSI和UNICODE，
	for (i = 0; i < 6; i++)
	{
		point.Format(_T("%.3f"), pos[i]);
		Des_pos += point;
		Des_pos += ",";
	}
	point.Format(_T("%.3f"), pos[6]);
	Des_pos += point;
	Des_pos += _T(")");
}




int main(){
	printf("Hello, World! \n");



	double PosStartArray[7] = { 0 };	//工具末端起始位置
	int i;
	CString DesPos;		//声明期望位置
	int lTool = 166;

	lxl DensoRobot;	//声明一个robot对象
	DensoRobot.pIRobot->Move(2, CComVariant("@0 P0"), CComBSTR(L""));    //移动到起始位置
	DensoRobot.pIRobot->Speed(-1, 20);   //设置运动速度
	Sleep(5000);	//延时

	PosStartArray[i] += 10;//工具末端位置不断变化，delta_pos = 10
	DensoRobot.SetToolPos(PosStartArray);//工具末端位置转换为机械臂末端位置(坐标变换)
	DensoRobot.ArrayToPos(PosStartArray, DesPos);//工具末端当期位置与期望位姿数组


}



//halconcpp.lib
//cv.lib
//cvaux.lib
//cxcore.lib