#ifndef _CURVE_H_
#define _CURVE_H_

#endif // !_CURVE_H_

#if !defined(AFX_CURVE_H__4C711C33_5C01_4F7E_8723_82B388ADCE64__INCLUDED_)
#define AFX_CURVE_H__4C711C33_5C01_4F7E_8723_82B388ADCE64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <list>
#include <vector>
#include <iterator>

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CCurve window

class CCurve : public CStatic
{
// Construction
public:
	CCurve();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurve)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetCurvePenColor(COLORREF crPen);

	void AddValue(double *jointerr);
	void AddValue(double dYValue1,double dYValue2,double dYValue3,double dYValue4);
	void AddValue(double dYValue1,double dYValue2);
	int m_nSize;
	void Show(void);
	CFont m_font;//CFont类封装了一个Windows图形设备接口（GDI）字体，并为操作字体提供了成员函数。
	int m_nDataCounts;

	void SetXMaxValue(int maxValue);
	void SetYMaxValue(int maxValue);
	int m_nXMaxValue;
	int m_nYMaxValue;

	int ConvertValueToAxisX(double value);
	int ConvertValueToAxisY(double value);
	
    void SetDataCounts(int nDataCounts);

	void DrawCurve(CDC *pDC);
	void DrawAxis(CDC *pDC);


	void SetYCount(int yCount);
	void SetXCount(int xCount);
	int m_nYCount;
	int m_nXCount;
	CRect m_rect;
	//这个类是从tagRECT结构派生而来的。（tagRECT是RECT结构的不太常用的别名。）
	//这意味着RECT结构的数据成员（left，top，right，和bottom）也是CRect的可访问数据成员。
	//left左上角X坐标，top左上角Y坐标，right右下角X坐标，bottom右下角Y坐标。
	CRect m_rectCurvePanel;

	void SetBkColor(COLORREF crColor);
	CBrush m_CurvePanelBrush;//画刷类CBrush
	CBrush m_bkBrush;//画刷类CBrush
	/*
	利用画笔可以画图形的边框，而用画刷就可以在图形内着色。
	大多数的GDI绘图函数既使用画笔又使用画刷，它们用画笔绘制各种图形的周边，
	而用画刷填充图形，因而可以用一种颜色和风格去设置画笔，
	而用另一种颜色和风格去设定画刷，通过一次函数调用就可以绘制出形状复杂的图形。
	*/
	void Update();
	

    //CPen画笔是绘图的基本工具，在MFC中画笔是CPen类的对象，它用来在DC上完成绘制线条的任务。初始时会有默认形态，但是可以根据个人喜好进行程序的编写从而改变其形态。
	CPen m_CurvePen1;
	CPen m_CurvePen2;
	CPen m_CurvePen3;
	CPen m_CurvePen4;
	CPen m_AxisPen;
//COLORREF是color和reference的缩写，表示颜色值，实际上是unsigned long类型。可以配合RGB()宏来设置。
	COLORREF m_LineColor;
	//定义Point类，有坐标x,y两个私有成员变量;对Point类重载“+”（相加）、“-”（相减）和“==”（相等）运算符，实现对坐标的改变，要求用友元函数和成员函数两种方法实现。
	POINT* m_pPoint[4];

    list<double> m_listY1;//声明链表
	list<double> m_listY2;
	list<double> m_listY3;
	list<double> m_listY4;
	list<double> m_listY5;
	list<double> m_listY6;

	double RangeChange(double dValue);
	void Clear();

	virtual ~CCurve();

	// Generated message map functions//生成的消息映射函数
protected:
	//{{AFX_MSG(CCurve)
	afx_msg void OnPaint();//在注释宏之间的声明在VC中灰色显示。afx_msg宏表示声明的是一个消息响应函数.afx_msg为消息标志，它向系统声明：有消息映射到函数实现体；

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURVE_H__4C711C33_5C01_4F7E_8723_82B388ADCE64__INCLUDED_)
