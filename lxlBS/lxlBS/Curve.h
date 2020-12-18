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
	CFont m_font;//CFont���װ��һ��Windowsͼ���豸�ӿڣ�GDI�����壬��Ϊ���������ṩ�˳�Ա������
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
	//������Ǵ�tagRECT�ṹ���������ġ���tagRECT��RECT�ṹ�Ĳ�̫���õı�������
	//����ζ��RECT�ṹ�����ݳ�Ա��left��top��right����bottom��Ҳ��CRect�Ŀɷ������ݳ�Ա��
	//left���Ͻ�X���꣬top���Ͻ�Y���꣬right���½�X���꣬bottom���½�Y���ꡣ
	CRect m_rectCurvePanel;

	void SetBkColor(COLORREF crColor);
	CBrush m_CurvePanelBrush;//��ˢ��CBrush
	CBrush m_bkBrush;//��ˢ��CBrush
	/*
	���û��ʿ��Ի�ͼ�εı߿򣬶��û�ˢ�Ϳ�����ͼ������ɫ��
	�������GDI��ͼ������ʹ�û�����ʹ�û�ˢ�������û��ʻ��Ƹ���ͼ�ε��ܱߣ�
	���û�ˢ���ͼ�Σ����������һ����ɫ�ͷ��ȥ���û��ʣ�
	������һ����ɫ�ͷ��ȥ�趨��ˢ��ͨ��һ�κ������þͿ��Ի��Ƴ���״���ӵ�ͼ�Ρ�
	*/
	void Update();
	

    //CPen�����ǻ�ͼ�Ļ������ߣ���MFC�л�����CPen��Ķ�����������DC����ɻ������������񡣳�ʼʱ����Ĭ����̬�����ǿ��Ը��ݸ���ϲ�ý��г���ı�д�Ӷ��ı�����̬��
	CPen m_CurvePen1;
	CPen m_CurvePen2;
	CPen m_CurvePen3;
	CPen m_CurvePen4;
	CPen m_AxisPen;
//COLORREF��color��reference����д����ʾ��ɫֵ��ʵ������unsigned long���͡��������RGB()�������á�
	COLORREF m_LineColor;
	//����Point�࣬������x,y����˽�г�Ա����;��Point�����ء�+������ӣ�����-����������͡�==������ȣ��������ʵ�ֶ�����ĸı䣬Ҫ������Ԫ�����ͳ�Ա�������ַ���ʵ�֡�
	POINT* m_pPoint[4];

    list<double> m_listY1;//��������
	list<double> m_listY2;
	list<double> m_listY3;
	list<double> m_listY4;
	list<double> m_listY5;
	list<double> m_listY6;

	double RangeChange(double dValue);
	void Clear();

	virtual ~CCurve();

	// Generated message map functions//���ɵ���Ϣӳ�亯��
protected:
	//{{AFX_MSG(CCurve)
	afx_msg void OnPaint();//��ע�ͺ�֮���������VC�л�ɫ��ʾ��afx_msg���ʾ��������һ����Ϣ��Ӧ����.afx_msgΪ��Ϣ��־������ϵͳ����������Ϣӳ�䵽����ʵ���壻

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURVE_H__4C711C33_5C01_4F7E_8723_82B388ADCE64__INCLUDED_)
