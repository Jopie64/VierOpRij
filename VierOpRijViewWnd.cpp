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
	SetVeld(VierOpRijVeld());
}

CVierOpRijViewWnd::~CVierOpRijViewWnd()
{
}


BEGIN_MESSAGE_MAP(CVierOpRijViewWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CVierOpRijViewWnd message handlers



void CVierOpRijViewWnd::OnPaint()
{
	static CFont S_font;
	if(S_font.m_hObject == NULL)
//		S_font.CreatePointFont(80, L"MS Sans Serif");
		S_font.CreatePointFont(70, L"Courier New");
	CPaintDC dc(this);
	CFont* pOldFont = dc.SelectObject(&S_font);
	CRect rectClient;
	GetClientRect(&rectClient);
	dc.FillSolidRect(rectClient, RGB(255,255,255));

	// **** Stats

	CRect rectStats  = rectClient;
	rectStats.bottom = rectClient.top + 20;
	rectClient.top   = rectStats.bottom + 1;
	dc.Rectangle(rectStats);
	rectStats.DeflateRect(2,2);
	//rectStats.left  += 2;
	dc.DrawText(m_Stats.c_str(), rectStats, DT_LEFT);

	// **** Progress

//	static CFont sCourier;
//	if(sCourier.m_hObject == NULL)
//		sCourier.CreatePointFont(60, L"Courier New", &dc);

//	CFont * pOldFont2 = dc.SelectObject(&sCourier);
	
	CRect rectProgress = rectClient;
	rectProgress.right = rectProgress.left + 80;
	rectClient.left    = rectProgress.right + 1;

	dc.Rectangle(rectProgress);
	rectProgress.DeflateRect(2,2);
	dc.DrawText(m_Progress.c_str(), rectProgress, DT_TOP | DT_LEFT);

//	dc.SelectObject(pOldFont2);

	// **** Scores
	CRect rectScores  = rectClient;
	rectScores.bottom = rectClient.top + 20;
	rectClient.top    = rectScores.bottom + 1;

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
		int waarde = m_Scores[x].m_Waarde;
		if     (waarde >= CZetBedenker::Sm_PlusMax) score << "Winst ("   << waarde - CZetBedenker::Sm_PlusMax << ")";
		else if(waarde <= CZetBedenker::Sm_MinMax)  score << "Verlies (" << CZetBedenker::Sm_MinMax - waarde << ")";
		else score << waarde;

//		switch(m_Scores[x].m_Waarde) 
//		{
//		case CZetBedenker::Sm_PlusMax:	score << "Winst"; break;
//		case CZetBedenker::Sm_MinMax:	score << "Verlies"; break;
//		default: score << m_Scores[x].m_Waarde;
//		}
		
		dc.DrawText(score.str().c_str(), rectVakje, DT_CENTER);
	}

	// *** Vakjes



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
			switch(Veld().Wie(x, y))
			{
			case 1: kleur = RGB(255,0,0); break;
			case 2: kleur = RGB(255,255,0); break;
			}
			dc.FillSolidRect(rectVakje, kleur);
		}

	dc.SelectObject(pOldFont);
}

void CVierOpRijViewWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetFocus();

	CWnd::OnLButtonUp(nFlags, point);
}
