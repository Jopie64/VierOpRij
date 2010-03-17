// VierOpRijViewWnd.cpp : implementation file
//

#include "stdafx.h"
#include "VierOpRij.h"
#include "VierOpRijViewWnd.h"
#include <sstream>

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

	CRect rectScores = rectClient;
	rectScores.bottom = rectClient.top + 20;

	dc.SetTextColor(RGB(0, 0, 0));
	dc.SetBkMode(TRANSPARENT);
	for(int x = 0; x < VierOpRijVeld::Sm_Breedte; ++x)
	{
		if(!m_Scores[x].m_bBekend)
			continue; //Score niet bekend. Dus maar niet tekenen...

		CRect rectVakje(
			(int)(rectScores.left   + x       * (1.0 * rectScores.Width() / VierOpRijVeld::Sm_Breedte)),
			rectScores.top,

			(int)(rectScores.left   + (x + 1) * (1.0 * rectScores.Width() / VierOpRijVeld::Sm_Breedte)),
			rectScores.bottom);

		std::wstringstream score;
		score << m_Scores[x].m_Waarde;
		dc.DrawText(score.str().c_str(), rectVakje, 0);
	}

	rectClient.top = rectScores.bottom + 1;


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
