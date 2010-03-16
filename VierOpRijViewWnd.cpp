// VierOpRijViewWnd.cpp : implementation file
//

#include "stdafx.h"
#include "VierOpRij.h"
#include "VierOpRijViewWnd.h"


// CVierOpRijViewWnd

IMPLEMENT_DYNAMIC(CVierOpRijViewWnd, CWnd)

CVierOpRijViewWnd::CVierOpRijViewWnd()
{

}

CVierOpRijViewWnd::~CVierOpRijViewWnd()
{
}


BEGIN_MESSAGE_MAP(CVierOpRijViewWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CVierOpRijViewWnd message handlers



void CVierOpRijViewWnd::OnPaint()
{
	CPaintDC dc(this);
	CRect rectClient;
	GetClientRect(&rectClient);
	dc.FillSolidRect(rectClient, RGB(255,255,255));

	for(int x = 0; x < VierOpRijVeld::Sm_Breedte; ++x)
		for(int y = 0; y < VierOpRijVeld::Sm_Hoogte; ++y)
		{
			CRect rectVakje(
				(int)(rectClient.left   + x       * (1.0 * rectClient.Width() / VierOpRijVeld::Sm_Breedte)),
				(int)(rectClient.bottom - (y + 1) * (1.0 * rectClient.Height() / VierOpRijVeld::Sm_Hoogte)),

				(int)(rectClient.left   + (x + 1) * (1.0 * rectClient.Width() / VierOpRijVeld::Sm_Breedte)),
				(int)(rectClient.bottom - y       * (1.0 * rectClient.Height() / VierOpRijVeld::Sm_Hoogte)));
			
			rectVakje.DeflateRect(1,1);
			COLORREF kleur = RGB(150, 150, 150);
			switch(m_Veld.Wie(x, y))
			{
			case 1: kleur = RGB(255,0,0); break;
			case 2: kleur = RGB(255,255,0); break;
			}
			dc.FillSolidRect(rectVakje, kleur);
		}
}
