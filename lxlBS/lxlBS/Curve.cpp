// Curve.cpp : implementation file
//MFC界面实现文件

#include "stdafx.h"
#include "Curve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/*
当在debug模式下时，我们分配内存时的new被替换成DEBUG_NEW，而这个DEBUG_NEW不仅要传入内存块的大小，还要传入源文件名和行号，这就有个好处，即当发生内存泄漏时，我们可以在调试模式下定位到该问题代码处。
若删掉该句，就不能进行定位了。而在release版本下的new就是简单的new，并不会传入文件名和行号。
如果定义了_DEBUG，表示在调试状态下编译，因此相应修改了两个符号的定义THIS_FILE是一个char数组全局变量，字符串值为当前文件的全路径，这样在Debug版本中当程序出错时出错处理代码可用这个变量告诉你是哪个文件中的代码有问题。
*/


#define AXISWIDTH 8
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CCurve

CCurve::CCurve()
{
	m_nXCount = 2;
	m_nYCount = 2;
	m_nXMaxValue = 500;
	m_nYMaxValue = 1;
	m_bkBrush.CreateSolidBrush(RGB(255,0,0));//m_bkBrush为画刷类对象，CreateSolidBrush创建画刷，括号内为画刷颜色
	/*
	画刷类CBrush
	利用画笔可以画图形的边框，而用画刷就可以在图形内着色。
	大多数的GDI绘图函数既使用画笔又使用画刷，它们用画笔绘制各种图形的周边，
	而用画刷填充图形，因而可以用一种颜色和风格去设置画笔，
	而用另一种颜色和风格去设定画刷，通过一次函数调用就可以绘制出形状复杂的图形。
	*/
	m_CurvePanelBrush.CreateSolidBrush(RGB(230,230,230));//m_CurvePanelBrush为画刷类对象，CreateSolidBrush创建画刷，括号内为画刷颜色
	
    m_nSize = 0;
	//CFont类封装了一个Windows图形设备接口（GDI）字体，并为操作字体提供了成员函数。
	//CreateFont该函数创建一种有特殊性的逻辑字体。此逻辑字体能够在后面被不论什么设备选择。
	m_font.CreateFont(20,8,0,0,FW_ULTRABOLD,FALSE,FALSE,FALSE, 
		ANSI_CHARSET,OUT_TT_PRECIS ,CLIP_DEFAULT_PRECIS, 
		PROOF_QUALITY ,FF_SWISS , NULL);

	m_nDataCounts = 4;

	for(int i = 0; i < 4; i++)
	{
		m_pPoint[i] = new POINT[500];//分配空间
	}

	for(int i = 0; i < m_nDataCounts; ++i)
	{
		for(int j = 0; j < 200; j++)
		{
			(m_pPoint[i])[j].x = j;
		}
	}

    m_CurvePen1.CreatePen(PS_SOLID,1,RGB(255,0,0));//CreatePen是指定的样式、宽度和颜色创建画笔。
	m_CurvePen2.CreatePen(PS_SOLID,1,RGB(0,0,0));
	m_CurvePen3.CreatePen(PS_SOLID,1,RGB(0,0,255));
	m_CurvePen4.CreatePen(PS_SOLID,1,RGB(255,0,255));

	m_AxisPen.CreatePen(PS_SOLID,3,RGB(0,0,0));

}

CCurve::~CCurve()
{
	for(int i = 0; i < 4; i++)
	{
		delete[] m_pPoint[i];//释放空间
	}
}


BEGIN_MESSAGE_MAP(CCurve, CStatic)
//BEGIN_MESSAGE_MAP是宏定义的一种，在BEGIN_MESSAGE_MAP()和END_MESSAGE_MAP()之间添加你的消息响应函数, 为每个消息处理函数加入一个入口。
	//{{AFX_MSG_MAP(CCurve)
	ON_WM_PAINT()
	//这个宏是MFC用来构造消息映射表时用到的辅助宏，它埋在类识别宏或动态识别宏中。
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()
//DECLARE_MESSAGE_MAP、BEGIN_MESSAGE_MAP、END_MESSAGE_MAP以及ON_COMMAND、ON_MESSAGE等宏最终作用的结果是在你的类中生成了一个名为lpEntries的数组，该数组中将填入每个你关心的消息和此消息对应的处理函数。
/////////////////////////////////////////////////////////////////////////////


void CCurve::SetBkColor(COLORREF crColor)
{//setbkcolor，函数名。该函数用指定的颜色值来设置当前的背景色，
//如果指定的颜色值超出了当前设备的表示范围，则设置为最近似的、设备可以表示的颜色。
    m_bkBrush.Detach();//m_bkBrush为画刷类对象
	m_bkBrush.CreateSolidBrush(crColor);//创建实体画刷
}

void CCurve::SetXCount(int xCount)//设置X值
{
	m_nXCount = xCount;
	
}

void CCurve::SetYCount(int yCount)//设置Y值
{
    m_nYCount = yCount;
}

void CCurve::SetXMaxValue(int maxValue)//设置X最大值
{
    m_nXMaxValue = maxValue;
}

void CCurve::SetYMaxValue(int maxValue)//设置Y最大值
{
    m_nYMaxValue = maxValue;
}

int CCurve::ConvertValueToAxisX(double value)//将值转换为X轴
{
	int nReturn;
	int nCenterX  = (m_rectCurvePanel.left + m_rectCurvePanel.right) / 2;
	
	
	nReturn = (int)(value / m_nXMaxValue * m_rectCurvePanel.Width() + 30);
	
	return nReturn;
}

int CCurve::ConvertValueToAxisY(double value)//将值转换为Y轴
{
	int nReturn;
	
	
	int nCenterY  = (m_rectCurvePanel.top + m_rectCurvePanel.bottom) / 2;
	
	
	nReturn = (int)(m_rectCurvePanel.Height() - value / m_nYMaxValue * m_rectCurvePanel.Height() + 10);
	
	
	return nReturn;
}

void CCurve::OnPaint()
{
	CPaintDC dc(this); // device context for painting//用于绘制设备的背景， MFC自动生成的OnPaint函数都会定义一个CPaintDC类型的变量。
	//CPaintDC是一个特殊的设备环境封闭类，它主要处理windows的wm_paint消息。
	// CPaintDC一般是用在OnPaint（）函数里的。CPaintDC对象一构造，消息队列中的WM_PAINT会被取走，
	GetClientRect(&m_rect);//该函数获取窗口客户区的大小。注意一下：窗口的客户区为窗口中除标题栏、菜单栏之外的地方。
	m_rectCurvePanel = m_rect;
	m_rectCurvePanel.InflateRect(-30,-10,0,-18);//InflateRect函数增大或减小指定矩形的宽和高。InflateRect函数在矩形的左和右增加dx,在矩形的上下增加dy。 dx和dy参数是有符号值。正数增加宽和高，负数减小。
	
	//DC就是设备描述表（Device Context, DC），bai可以把它理解成“du画布”。
	//CPaintDC的父类就是CDC，换句话说两者底层使用同一种方法进行绘图。
	//MFC是通过消息泵信息处理的。CPaintDC内部发出绘图消息给系统。所以描画画面时，最好使用CPaintDC。
	//CDC只是HDC的包装，不包含绘图消息的发送。所以，创建图片文件（不需要刷新画面）等处理，需要使用CDC。
	CDC MemDC;
	CBitmap MemBitmap;//类CBitmap封装了Windows图形设备接口（GDI）中的位图，并且提供了操纵位图的成员函数。使用CBitmap对象之前要先构造CBitmap对象，调用其中的一个初始化成员函数设置位图对象的句柄。此后就可以调用其它成员函数了。

	MemDC.CreateCompatibleDC(NULL);//该函数创建一个与指定设备兼容的内存设备上下文环境（DC）。
	MemBitmap.CreateCompatibleBitmap(&dc,m_rect.Width(),m_rect.Height());//该函数用于创建与指定的设备环境相关的设备兼容的位图。

	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);//SelectObject，计算机编程语言函数，该函数选择一对象到指定的设备上下文环境中，该新对象替换先前的相同类型的对象。

	MemDC.FillRect(&m_rect,&m_bkBrush);//FillRect函数用指定的画刷填充矩形，此函数包括矩形的左上边界，但不包括矩形的右下边界。
	
	MemDC.Rectangle(&m_rectCurvePanel);//Rectangle是一个函数，使用该函数画一个矩形，可以用当前的画笔画矩形轮廓，用当前画刷进行填充。
   // MemDC.FillRect(&m_rectCurvePanel,&m_CurvePanelBrush);

	DrawAxis(&MemDC);//画轴
	DrawCurve(&MemDC);//画曲线


	dc.BitBlt(0,0,m_rect.Width(),m_rect.Height(),&MemDC,0,0,SRCCOPY); //BitBlt是一个计算机函数，该函数对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	
	MemBitmap.DeleteObject();//DeleteObject，该函数删除一个逻辑笔、画笔、字体、位图、区域或者调色板，释放所有与该对象有关的系统资源，在对象被删除之后，指定的句柄也就失效了。
	MemDC.DeleteDC();//该函数删除指定的设备上下文环境（Dc）。
	// Do not call CStatic::OnPaint() for painting messages
}

void CCurve::DrawAxis(CDC *pDC)
{
	CString str;
	
	CPen *pOldPen;
    pOldPen = pDC->SelectObject(&m_AxisPen);
	
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextAlign(TA_RIGHT|TA_BASELINE);
	
	int dimX = m_rectCurvePanel.Width() / m_nXCount;
	
	int x = m_rectCurvePanel.left ;
	
	int nDimX = m_nXMaxValue / m_nXCount;
	
	//Begin drawing X axis
	for(int i = 0; i<= m_nXCount; i++) 
	{
			
		str.Format(_T("%3d"),i*nDimX);
	
		pDC->TextOut(x,m_rectCurvePanel.bottom + 16,str);
		
		x += dimX;
		
		pDC->MoveTo(x,m_rectCurvePanel.bottom - AXISWIDTH);
		pDC->LineTo(x,m_rectCurvePanel.bottom);
	}
	
	//Begin drawing Y axix
	int dimY = m_rectCurvePanel.Height() / m_nYCount;
	int y = m_rectCurvePanel.bottom ;
	
	int nDimY =  m_nYMaxValue / m_nYCount;
	
	
	pDC->SetTextAlign(TA_LEFT|TA_BASELINE);
	
	
	for(int i = 0; i <= m_nYCount; i++ )
	{
		str.Format(_T("%3d"),i*nDimY - m_nYMaxValue / 2);
		
		pDC->TextOut(m_rectCurvePanel.left - 30,y,str);
		
		y -= dimY;
		pDC->MoveTo(m_rectCurvePanel.left + AXISWIDTH,y);
		pDC->LineTo(m_rectCurvePanel.left,y);
		
	}

	if(m_nDataCounts == 4)
	{
		pDC->SetTextAlign(TA_RIGHT|TA_BASELINE);

	//	CFont *pOldFont = dc.SelectObject(&m_font);
        //du1 
		pDC->SelectObject(&m_CurvePen1);
		pDC->SetBkMode(TRANSPARENT);
	    pDC->MoveTo(m_rectCurvePanel.right - 85,m_rectCurvePanel.top + 8);
		pDC->LineTo(m_rectCurvePanel.right - 110,m_rectCurvePanel.top + 8);	

	//	dc.SetTextColor(RGB(255,0,0));
		pDC->TextOut(m_rectCurvePanel.right - 120,m_rectCurvePanel.top + 13,_T("du1"));

		//dv1 
		pDC->SelectObject(&m_CurvePen2);
        pDC->MoveTo(m_rectCurvePanel.right - 35,m_rectCurvePanel.top + 8);
		pDC->LineTo(m_rectCurvePanel.right - 5,m_rectCurvePanel.top + 8);	
	
	
	//	dc.SetTextColor(RGB(0,255,0));
		pDC->TextOut(m_rectCurvePanel.right - 40,m_rectCurvePanel.top + 13,_T("dv1"));

        //du2 
		pOldPen = pDC->SelectObject(&m_CurvePen3);
		pDC->MoveTo(m_rectCurvePanel.right - 85,m_rectCurvePanel.top + 23);
		pDC->LineTo(m_rectCurvePanel.right - 110,m_rectCurvePanel.top + 23);	
	
		
	//	dc.SetTextColor(RGB(0,0,255));
		pDC->TextOut(m_rectCurvePanel.right - 120,m_rectCurvePanel.top + 28,_T("du2"));

		//dv2 
		pDC->SelectObject(&m_CurvePen4);
		pDC->MoveTo(m_rectCurvePanel.right - 35,m_rectCurvePanel.top + 23);
		pDC->LineTo(m_rectCurvePanel.right - 5,m_rectCurvePanel.top + 23);

		
	//	dc.SetTextColor(RGB(255,0,255));
		pDC->TextOut(m_rectCurvePanel.right - 40,m_rectCurvePanel.top + 28,_T("dv2"));

	//	dc.SelectObject(pOldFont);
	}

	else if(m_nDataCounts == 2)
	{
		pDC->SetTextAlign(TA_RIGHT|TA_BASELINE);

	//	CFont *pOldFont = dc.SelectObject(&m_font);
        //dAngle1 
		pDC->SelectObject(&m_CurvePen1);
        pDC->SetBkMode(TRANSPARENT);
		pDC->MoveTo(m_rectCurvePanel.right - 85,m_rectCurvePanel.top + 12);
		pDC->LineTo(m_rectCurvePanel.right - 110,m_rectCurvePanel.top + 12);	
		
	//	dc.SetTextColor(RGB(255,0,0));
		pDC->TextOut(m_rectCurvePanel.right - 120,m_rectCurvePanel.top + 17,_T("dθ1"));

		//dAngle2 
		pDC->SelectObject(&m_CurvePen2);
        pDC->MoveTo(m_rectCurvePanel.right - 35,m_rectCurvePanel.top + 12);
		pDC->LineTo(m_rectCurvePanel.right - 5,m_rectCurvePanel.top + 12);	

	
	//	dc.SetTextColor(RGB(0,255,0));
		pDC->TextOut(m_rectCurvePanel.right - 40,m_rectCurvePanel.top + 17,_T("dθ2"));

	}
	pDC->SelectObject(pOldPen);
	
    
}

void CCurve::DrawCurve(CDC *pDC)
{	
	int nDC;
    nDC = pDC->SaveDC();
	
	pDC->SetMapMode(MM_ANISOTROPIC);
    pDC->SetWindowExt(m_nXMaxValue,-m_nYMaxValue);
	pDC->SetViewportExt(m_rectCurvePanel.Width(),m_rectCurvePanel.Height());
	
	pDC->SetWindowOrg(0,0);
	pDC->SetViewportOrg(m_rectCurvePanel.left,m_rectCurvePanel.bottom);

	CPen *pOldPen;
	pOldPen = pDC->SelectObject(&m_CurvePen1);
	//TRACE("size: %d\n",m_listY1.size());
	if(m_listY1.size() > 0)
	{
		pDC->Polyline(m_pPoint[0],m_listY1.size());
	}

	//for(int i = 0; i < m_listY1.size(); ++i)
	//{
	//TRACE("point0: %d,%d size: %d\n",(m_pPoint[0])[i].x,(m_pPoint[0])[i].y,m_listY1.size());
	//}

	if(m_listY2.size() > 0)
	{
		pDC->SelectObject(&m_CurvePen2);
		pDC->Polyline(m_pPoint[1],m_listY2.size());
	}


	if(m_nDataCounts > 2)
	{
		if(m_listY3.size() > 0)
		{
			pDC->SelectObject(&m_CurvePen3);
			pDC->Polyline(m_pPoint[2],m_listY3.size());
		}

		if(m_listY4.size() > 0)
		{
			pDC->SelectObject(&m_CurvePen4);
			pDC->Polyline(m_pPoint[3],m_listY4.size());
		}
	}

	pDC->SelectObject(pOldPen);

	pDC->RestoreDC(nDC);	
}

void CCurve::Show()
{
    OnPaint();
}


double CCurve::RangeChange(double dValue)
{
    return (dValue + (double)m_nYMaxValue / 2);
}

void CCurve::Clear()
{
	m_listY1.clear();
	m_listY2.clear();
	m_listY3.clear();
	m_listY4.clear();
	InvalidateRect(m_rectCurvePanel); //该函数向指定的窗体更新区域添加一个矩形，然后窗体跟新区域的这一部分将被重新绘制。
}

void CCurve::Update()
{
	int i = 0;
	list<double>::iterator yValue_it1 = m_listY1.begin();
	while(yValue_it1 != m_listY1.end())
	{
		(m_pPoint[0])[i].y = (LONG)RangeChange(*yValue_it1);
		++yValue_it1;
		++i;
	}

	list<double>::iterator yValue_it2 = m_listY2.begin();
	i = 0;
	while(yValue_it2 != m_listY2.end())
	{
		(m_pPoint[1])[i].y = (LONG)RangeChange(*yValue_it2);
		++yValue_it2;
		++i;
	}
	if(m_nDataCounts > 2)
	{
		list<double>::iterator yValue_it3 = m_listY3.begin();
		i = 0;
		while(yValue_it3 != m_listY3.end())
		{
			(m_pPoint[2])[i].y = (LONG)RangeChange(*yValue_it3);
			++yValue_it3;
			++i;
		}

		list<double>::iterator yValue_it4 = m_listY4.begin();
		i = 0;
		while(yValue_it4 != m_listY4.end())
		{
			(m_pPoint[3])[i].y = (LONG)RangeChange(*yValue_it4);
			++yValue_it4;
			++i;
		}
	}
	InvalidateRect(m_rectCurvePanel);//该函数向指定的窗体更新区域添加一个矩形，然后窗体跟新区域的这一部分将被重新绘制。
}

//我新加的函数
void CCurve::AddValue(double *jointerr)
	{
	m_listY1.push_back(jointerr[0]);
	if(m_listY1.size() > m_nXMaxValue)
	{
		m_listY1.pop_front();//通过pop_front()删除第一个元素,通过pop_back删除最后一个元素
	}
	m_listY2.push_back(jointerr[1]);//其中push_back()从list的末端插入，而 push_front()实现的从list的头部插入一个元素
	if(m_listY2.size() > m_nXMaxValue)
	{
		m_listY2.pop_front();//通过pop_front()删除第一个元素,通过pop_back删除最后一个元素
	}
	m_listY3.push_back(jointerr[2]);//其中push_back()从list的末端插入，而 push_front()实现的从list的头部插入一个元素
	if(m_listY3.size() > m_nXMaxValue)
	{
		m_listY3.pop_front();//通过pop_front()删除第一个元素,通过pop_back删除最后一个元素
	}
	m_listY4.push_back(jointerr[3]);//其中push_back()从list的末端插入，而 push_front()实现的从list的头部插入一个元素
	if(m_listY4.size() > m_nXMaxValue)
	{
		m_listY4.pop_front();//通过pop_front()删除第一个元素,通过pop_back删除最后一个元素
	}
	m_listY5.push_back(jointerr[4]);//其中push_back()从list的末端插入，而 push_front()实现的从list的头部插入一个元素
	if(m_listY5.size() > m_nXMaxValue)
	{
		m_listY5.pop_front();//通过pop_front()删除第一个元素,通过pop_back删除最后一个元素
	}
	m_listY6.push_back(jointerr[5]);//其中push_back()从list的末端插入，而 push_front()实现的从list的头部插入一个元素
	if(m_listY6.size() > m_nXMaxValue)
	{
		m_listY6.pop_front();//通过pop_front()删除第一个元素,通过pop_back删除最后一个元素
	}
}
void CCurve::AddValue(double dYValue1,double dYValue2,double dYValue3,double dYValue4)
{
	m_listY1.push_back(dYValue1);//其中push_back()从list的末端插入，而 push_front()实现的从list的头部插入一个元素
	if(m_listY1.size() > m_nXMaxValue)
	{
		m_listY1.pop_front();//通过pop_front()删除第一个元素,通过pop_back删除最后一个元素
	}

	m_listY2.push_back(dYValue2);//其中push_back()从list的末端插入，而 push_front()实现的从list的头部插入一个元素
	if(m_listY2.size() > m_nXMaxValue)
	{
		m_listY2.pop_front();//通过pop_front()删除第一个元素,通过pop_back删除最后一个元素
	}

	m_listY3.push_back(dYValue3);//其中push_back()从list的末端插入，而 push_front()实现的从list的头部插入一个元素
	if(m_listY3.size() > m_nXMaxValue)
	{
		m_listY3.pop_front();//通过pop_front()删除第一个元素,通过pop_back删除最后一个元素
	}

	m_listY4.push_back(dYValue4);//其中push_back()从list的末端插入，而 push_front()实现的从list的头部插入一个元素
	if(m_listY4.size() > m_nXMaxValue)
	{
		m_listY4.pop_front();//通过pop_front()删除第一个元素,通过pop_back删除最后一个元素
	}

	int i = 0;
	list<double>::iterator yValue_it1 = m_listY1.begin();
	while(yValue_it1 != m_listY1.end())
	{
		(m_pPoint[0])[i].y = (LONG)RangeChange(*yValue_it1);
		++yValue_it1;
		++i;
	}

	list<double>::iterator yValue_it2 = m_listY2.begin();
	i = 0;
	while(yValue_it2 != m_listY2.end())
	{
		(m_pPoint[1])[i].y = (LONG)RangeChange(*yValue_it2);
		++yValue_it2;
		++i;
	}

	list<double>::iterator yValue_it3 = m_listY3.begin();
	i = 0;
	while(yValue_it3 != m_listY3.end())
	{
		(m_pPoint[2])[i].y = (LONG)RangeChange(*yValue_it3);
		++yValue_it3;
		++i;
	}

	list<double>::iterator yValue_it4 = m_listY4.begin();
	i = 0;
	while(yValue_it4 != m_listY4.end())
	{
		(m_pPoint[3])[i].y = (LONG)RangeChange(*yValue_it4);
		++yValue_it4;
		++i;
	}

	InvalidateRect(m_rectCurvePanel);
}

void CCurve::AddValue(double dYValue1,double dYValue2)
{
	m_listY1.push_back(dYValue1);
	if(m_listY1.size() > m_nXMaxValue)//m_nXMaxValue=500
	{
		m_listY1.pop_front(); //通过pop_front()删除第一个元素,通过pop_back删除最后一个元素
	}

	m_listY2.push_back(dYValue2);//其中push_back()从list的末端插入，而 push_front()实现的从list的头部插入一个元素
	if(m_listY2.size() > m_nXMaxValue)
	{
		m_listY2.pop_front();
	}
}


void CCurve::SetCurvePenColor(COLORREF crPen)
{
	m_CurvePen1.Detach();
    m_CurvePen1.CreatePen(PS_SOLID,2,crPen);//CreatePen是指定的样式、宽度和颜色创建画笔。
}

void CCurve::SetDataCounts(int nDataCounts)
{
	m_nDataCounts = nDataCounts;
}