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

//�õ���е��ĩ�˽�Ҫ�ƶ�����λ������
void lxl::ArrayToPos(double* pos, CString& Des_pos)
{
	int i = 0;
	CString point;
	Des_pos = _T("P(");    //_T("")��һ����,��������������ĳ���֧��Unicode���룬��ΪWindowsʹ�������ַ���ANSI��UNICODE��
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



	double PosStartArray[7] = { 0 };	//����ĩ����ʼλ��
	int i;
	CString DesPos;		//��������λ��
	int lTool = 166;

	lxl DensoRobot;	//����һ��robot����
	DensoRobot.pIRobot->Move(2, CComVariant("@0 P0"), CComBSTR(L""));    //�ƶ�����ʼλ��
	DensoRobot.pIRobot->Speed(-1, 20);   //�����˶��ٶ�
	Sleep(5000);	//��ʱ

	PosStartArray[i] += 10;//����ĩ��λ�ò��ϱ仯��delta_pos = 10
	DensoRobot.SetToolPos(PosStartArray);//����ĩ��λ��ת��Ϊ��е��ĩ��λ��(����任)
	DensoRobot.ArrayToPos(PosStartArray, DesPos);//����ĩ�˵���λ��������λ������


}



//halconcpp.lib
//cv.lib
//cvaux.lib
//cxcore.lib