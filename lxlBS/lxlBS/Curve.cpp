// Curve.cpp : implementation file
//MFC����ʵ���ļ�

#include "stdafx.h"
#include "Curve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/*
����debugģʽ��ʱ�����Ƿ����ڴ�ʱ��new���滻��DEBUG_NEW�������DEBUG_NEW����Ҫ�����ڴ��Ĵ�С����Ҫ����Դ�ļ������кţ�����и��ô������������ڴ�й©ʱ�����ǿ����ڵ���ģʽ�¶�λ����������봦��
��ɾ���þ䣬�Ͳ��ܽ��ж�λ�ˡ�����release�汾�µ�new���Ǽ򵥵�new�������ᴫ���ļ������кš�
���������_DEBUG����ʾ�ڵ���״̬�±��룬�����Ӧ�޸����������ŵĶ���THIS_FILE��һ��char����ȫ�ֱ������ַ���ֵΪ��ǰ�ļ���ȫ·����������Debug�汾�е��������ʱ����������������������������ĸ��ļ��еĴ��������⡣
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
	m_bkBrush.CreateSolidBrush(RGB(255,0,0));//m_bkBrushΪ��ˢ�����CreateSolidBrush������ˢ��������Ϊ��ˢ��ɫ
	/*
	��ˢ��CBrush
	���û��ʿ��Ի�ͼ�εı߿򣬶��û�ˢ�Ϳ�����ͼ������ɫ��
	�������GDI��ͼ������ʹ�û�����ʹ�û�ˢ�������û��ʻ��Ƹ���ͼ�ε��ܱߣ�
	���û�ˢ���ͼ�Σ����������һ����ɫ�ͷ��ȥ���û��ʣ�
	������һ����ɫ�ͷ��ȥ�趨��ˢ��ͨ��һ�κ������þͿ��Ի��Ƴ���״���ӵ�ͼ�Ρ�
	*/
	m_CurvePanelBrush.CreateSolidBrush(RGB(230,230,230));//m_CurvePanelBrushΪ��ˢ�����CreateSolidBrush������ˢ��������Ϊ��ˢ��ɫ
	
    m_nSize = 0;
	//CFont���װ��һ��Windowsͼ���豸�ӿڣ�GDI�����壬��Ϊ���������ṩ�˳�Ա������
	//CreateFont�ú�������һ���������Ե��߼����塣���߼������ܹ��ں��汻����ʲô�豸ѡ��
	m_font.CreateFont(20,8,0,0,FW_ULTRABOLD,FALSE,FALSE,FALSE, 
		ANSI_CHARSET,OUT_TT_PRECIS ,CLIP_DEFAULT_PRECIS, 
		PROOF_QUALITY ,FF_SWISS , NULL);

	m_nDataCounts = 4;

	for(int i = 0; i < 4; i++)
	{
		m_pPoint[i] = new POINT[500];//����ռ�
	}

	for(int i = 0; i < m_nDataCounts; ++i)
	{
		for(int j = 0; j < 200; j++)
		{
			(m_pPoint[i])[j].x = j;
		}
	}

    m_CurvePen1.CreatePen(PS_SOLID,1,RGB(255,0,0));//CreatePen��ָ������ʽ����Ⱥ���ɫ�������ʡ�
	m_CurvePen2.CreatePen(PS_SOLID,1,RGB(0,0,0));
	m_CurvePen3.CreatePen(PS_SOLID,1,RGB(0,0,255));
	m_CurvePen4.CreatePen(PS_SOLID,1,RGB(255,0,255));

	m_AxisPen.CreatePen(PS_SOLID,3,RGB(0,0,0));

}

CCurve::~CCurve()
{
	for(int i = 0; i < 4; i++)
	{
		delete[] m_pPoint[i];//�ͷſռ�
	}
}


BEGIN_MESSAGE_MAP(CCurve, CStatic)
//BEGIN_MESSAGE_MAP�Ǻ궨���һ�֣���BEGIN_MESSAGE_MAP()��END_MESSAGE_MAP()֮����������Ϣ��Ӧ����, Ϊÿ����Ϣ����������һ����ڡ�
	//{{AFX_MSG_MAP(CCurve)
	ON_WM_PAINT()
	//�������MFC����������Ϣӳ���ʱ�õ��ĸ����꣬��������ʶ����̬ʶ����С�
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()
//DECLARE_MESSAGE_MAP��BEGIN_MESSAGE_MAP��END_MESSAGE_MAP�Լ�ON_COMMAND��ON_MESSAGE�Ⱥ��������õĽ�������������������һ����ΪlpEntries�����飬�������н�����ÿ������ĵ���Ϣ�ʹ���Ϣ��Ӧ�Ĵ�������
/////////////////////////////////////////////////////////////////////////////


void CCurve::SetBkColor(COLORREF crColor)
{//setbkcolor�����������ú�����ָ������ɫֵ�����õ�ǰ�ı���ɫ��
//���ָ������ɫֵ�����˵�ǰ�豸�ı�ʾ��Χ��������Ϊ����Ƶġ��豸���Ա�ʾ����ɫ��
    m_bkBrush.Detach();//m_bkBrushΪ��ˢ�����
	m_bkBrush.CreateSolidBrush(crColor);//����ʵ�廭ˢ
}

void CCurve::SetXCount(int xCount)//����Xֵ
{
	m_nXCount = xCount;
	
}

void CCurve::SetYCount(int yCount)//����Yֵ
{
    m_nYCount = yCount;
}

void CCurve::SetXMaxValue(int maxValue)//����X���ֵ
{
    m_nXMaxValue = maxValue;
}

void CCurve::SetYMaxValue(int maxValue)//����Y���ֵ
{
    m_nYMaxValue = maxValue;
}

int CCurve::ConvertValueToAxisX(double value)//��ֵת��ΪX��
{
	int nReturn;
	int nCenterX  = (m_rectCurvePanel.left + m_rectCurvePanel.right) / 2;
	
	
	nReturn = (int)(value / m_nXMaxValue * m_rectCurvePanel.Width() + 30);
	
	return nReturn;
}

int CCurve::ConvertValueToAxisY(double value)//��ֵת��ΪY��
{
	int nReturn;
	
	
	int nCenterY  = (m_rectCurvePanel.top + m_rectCurvePanel.bottom) / 2;
	
	
	nReturn = (int)(m_rectCurvePanel.Height() - value / m_nYMaxValue * m_rectCurvePanel.Height() + 10);
	
	
	return nReturn;
}

void CCurve::OnPaint()
{
	CPaintDC dc(this); // device context for painting//���ڻ����豸�ı����� MFC�Զ����ɵ�OnPaint�������ᶨ��һ��CPaintDC���͵ı�����
	//CPaintDC��һ��������豸��������࣬����Ҫ����windows��wm_paint��Ϣ��
	// CPaintDCһ��������OnPaint����������ġ�CPaintDC����һ���죬��Ϣ�����е�WM_PAINT�ᱻȡ�ߣ�
	GetClientRect(&m_rect);//�ú�����ȡ���ڿͻ����Ĵ�С��ע��һ�£����ڵĿͻ���Ϊ�����г����������˵���֮��ĵط���
	m_rectCurvePanel = m_rect;
	m_rectCurvePanel.InflateRect(-30,-10,0,-18);//InflateRect����������Сָ�����εĿ�͸ߡ�InflateRect�����ھ��ε����������dx,�ھ��ε���������dy�� dx��dy�������з���ֵ���������ӿ�͸ߣ�������С��
	
	//DC�����豸������Device Context, DC����bai���԰������ɡ�du��������
	//CPaintDC�ĸ������CDC�����仰˵���ߵײ�ʹ��ͬһ�ַ������л�ͼ��
	//MFC��ͨ����Ϣ����Ϣ����ġ�CPaintDC�ڲ�������ͼ��Ϣ��ϵͳ�������軭����ʱ�����ʹ��CPaintDC��
	//CDCֻ��HDC�İ�װ����������ͼ��Ϣ�ķ��͡����ԣ�����ͼƬ�ļ�������Ҫˢ�»��棩�ȴ�����Ҫʹ��CDC��
	CDC MemDC;
	CBitmap MemBitmap;//��CBitmap��װ��Windowsͼ���豸�ӿڣ�GDI���е�λͼ�������ṩ�˲���λͼ�ĳ�Ա������ʹ��CBitmap����֮ǰҪ�ȹ���CBitmap���󣬵������е�һ����ʼ����Ա��������λͼ����ľ�����˺�Ϳ��Ե���������Ա�����ˡ�

	MemDC.CreateCompatibleDC(NULL);//�ú�������һ����ָ���豸���ݵ��ڴ��豸�����Ļ�����DC����
	MemBitmap.CreateCompatibleBitmap(&dc,m_rect.Width(),m_rect.Height());//�ú������ڴ�����ָ�����豸������ص��豸���ݵ�λͼ��

	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);//SelectObject�������������Ժ������ú���ѡ��һ����ָ�����豸�����Ļ����У����¶����滻��ǰ����ͬ���͵Ķ���

	MemDC.FillRect(&m_rect,&m_bkBrush);//FillRect������ָ���Ļ�ˢ�����Σ��˺����������ε����ϱ߽磬�����������ε����±߽硣
	
	MemDC.Rectangle(&m_rectCurvePanel);//Rectangle��һ��������ʹ�øú�����һ�����Σ������õ�ǰ�Ļ��ʻ������������õ�ǰ��ˢ������䡣
   // MemDC.FillRect(&m_rectCurvePanel,&m_CurvePanelBrush);

	DrawAxis(&MemDC);//����
	DrawCurve(&MemDC);//������


	dc.BitBlt(0,0,m_rect.Width(),m_rect.Height(),&MemDC,0,0,SRCCOPY); //BitBlt��һ��������������ú�����ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	
	MemBitmap.DeleteObject();//DeleteObject���ú���ɾ��һ���߼��ʡ����ʡ����塢λͼ��������ߵ�ɫ�壬�ͷ�������ö����йص�ϵͳ��Դ���ڶ���ɾ��֮��ָ���ľ��Ҳ��ʧЧ�ˡ�
	MemDC.DeleteDC();//�ú���ɾ��ָ�����豸�����Ļ�����Dc����
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
		pDC->TextOut(m_rectCurvePanel.right - 120,m_rectCurvePanel.top + 17,_T("d��1"));

		//dAngle2 
		pDC->SelectObject(&m_CurvePen2);
        pDC->MoveTo(m_rectCurvePanel.right - 35,m_rectCurvePanel.top + 12);
		pDC->LineTo(m_rectCurvePanel.right - 5,m_rectCurvePanel.top + 12);	

	
	//	dc.SetTextColor(RGB(0,255,0));
		pDC->TextOut(m_rectCurvePanel.right - 40,m_rectCurvePanel.top + 17,_T("d��2"));

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
	InvalidateRect(m_rectCurvePanel); //�ú�����ָ���Ĵ�������������һ�����Σ�Ȼ��������������һ���ֽ������»��ơ�
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
	InvalidateRect(m_rectCurvePanel);//�ú�����ָ���Ĵ�������������һ�����Σ�Ȼ��������������һ���ֽ������»��ơ�
}

//���¼ӵĺ���
void CCurve::AddValue(double *jointerr)
	{
	m_listY1.push_back(jointerr[0]);
	if(m_listY1.size() > m_nXMaxValue)
	{
		m_listY1.pop_front();//ͨ��pop_front()ɾ����һ��Ԫ��,ͨ��pop_backɾ�����һ��Ԫ��
	}
	m_listY2.push_back(jointerr[1]);//����push_back()��list��ĩ�˲��룬�� push_front()ʵ�ֵĴ�list��ͷ������һ��Ԫ��
	if(m_listY2.size() > m_nXMaxValue)
	{
		m_listY2.pop_front();//ͨ��pop_front()ɾ����һ��Ԫ��,ͨ��pop_backɾ�����һ��Ԫ��
	}
	m_listY3.push_back(jointerr[2]);//����push_back()��list��ĩ�˲��룬�� push_front()ʵ�ֵĴ�list��ͷ������һ��Ԫ��
	if(m_listY3.size() > m_nXMaxValue)
	{
		m_listY3.pop_front();//ͨ��pop_front()ɾ����һ��Ԫ��,ͨ��pop_backɾ�����һ��Ԫ��
	}
	m_listY4.push_back(jointerr[3]);//����push_back()��list��ĩ�˲��룬�� push_front()ʵ�ֵĴ�list��ͷ������һ��Ԫ��
	if(m_listY4.size() > m_nXMaxValue)
	{
		m_listY4.pop_front();//ͨ��pop_front()ɾ����һ��Ԫ��,ͨ��pop_backɾ�����һ��Ԫ��
	}
	m_listY5.push_back(jointerr[4]);//����push_back()��list��ĩ�˲��룬�� push_front()ʵ�ֵĴ�list��ͷ������һ��Ԫ��
	if(m_listY5.size() > m_nXMaxValue)
	{
		m_listY5.pop_front();//ͨ��pop_front()ɾ����һ��Ԫ��,ͨ��pop_backɾ�����һ��Ԫ��
	}
	m_listY6.push_back(jointerr[5]);//����push_back()��list��ĩ�˲��룬�� push_front()ʵ�ֵĴ�list��ͷ������һ��Ԫ��
	if(m_listY6.size() > m_nXMaxValue)
	{
		m_listY6.pop_front();//ͨ��pop_front()ɾ����һ��Ԫ��,ͨ��pop_backɾ�����һ��Ԫ��
	}
}
void CCurve::AddValue(double dYValue1,double dYValue2,double dYValue3,double dYValue4)
{
	m_listY1.push_back(dYValue1);//����push_back()��list��ĩ�˲��룬�� push_front()ʵ�ֵĴ�list��ͷ������һ��Ԫ��
	if(m_listY1.size() > m_nXMaxValue)
	{
		m_listY1.pop_front();//ͨ��pop_front()ɾ����һ��Ԫ��,ͨ��pop_backɾ�����һ��Ԫ��
	}

	m_listY2.push_back(dYValue2);//����push_back()��list��ĩ�˲��룬�� push_front()ʵ�ֵĴ�list��ͷ������һ��Ԫ��
	if(m_listY2.size() > m_nXMaxValue)
	{
		m_listY2.pop_front();//ͨ��pop_front()ɾ����һ��Ԫ��,ͨ��pop_backɾ�����һ��Ԫ��
	}

	m_listY3.push_back(dYValue3);//����push_back()��list��ĩ�˲��룬�� push_front()ʵ�ֵĴ�list��ͷ������һ��Ԫ��
	if(m_listY3.size() > m_nXMaxValue)
	{
		m_listY3.pop_front();//ͨ��pop_front()ɾ����һ��Ԫ��,ͨ��pop_backɾ�����һ��Ԫ��
	}

	m_listY4.push_back(dYValue4);//����push_back()��list��ĩ�˲��룬�� push_front()ʵ�ֵĴ�list��ͷ������һ��Ԫ��
	if(m_listY4.size() > m_nXMaxValue)
	{
		m_listY4.pop_front();//ͨ��pop_front()ɾ����һ��Ԫ��,ͨ��pop_backɾ�����һ��Ԫ��
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
		m_listY1.pop_front(); //ͨ��pop_front()ɾ����һ��Ԫ��,ͨ��pop_backɾ�����һ��Ԫ��
	}

	m_listY2.push_back(dYValue2);//����push_back()��list��ĩ�˲��룬�� push_front()ʵ�ֵĴ�list��ͷ������һ��Ԫ��
	if(m_listY2.size() > m_nXMaxValue)
	{
		m_listY2.pop_front();
	}
}


void CCurve::SetCurvePenColor(COLORREF crPen)
{
	m_CurvePen1.Detach();
    m_CurvePen1.CreatePen(PS_SOLID,2,crPen);//CreatePen��ָ������ʽ����Ⱥ���ɫ�������ʡ�
}

void CCurve::SetDataCounts(int nDataCounts)
{
	m_nDataCounts = nDataCounts;
}