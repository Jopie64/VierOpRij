
// VierOpRijDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VierOpRij.h"
#include "VierOpRijDlg.h"
#include <sstream>
#include <iomanip>
#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVierOpRijDlg dialog

const int denkDiepte = -1;


CVierOpRijDlg::CVierOpRijDlg(CWnd* pParent /*=NULL*/)
:	CDialog(CVierOpRijDlg::IDD, pParent),
	m_BezigeBedenkerPtr(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVierOpRijDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVierOpRijDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CVierOpRijDlg message handlers

BOOL CVierOpRijDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CRect rectClient;
	GetClientRect(&rectClient);
	rectClient.DeflateRect(10,10);
	m_VierOpRijWnd.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rectClient, this, 0xFF);

	SetDlgItemText(IDC_EDIT_LEVEL, L"-1");
	SetTimer(0, 100, NULL);

	RelocateControls();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVierOpRijDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVierOpRijDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVierOpRijDlg::AsyncBedenk(CMijnZetBedenker* pBedenker)
{
	try
	{
		//pBedenker->BedenkZet(denkDiepte);
		CString level;
		GetDlgItemText(IDC_EDIT_LEVEL, level);
		int diepte = _wtol(level);
		pBedenker->BedenkZet(diepte);
	}
	catch(std::exception& C_e)
	{
		AfxMessageBox(L"Oeps, maak even een denkfout... " + CString(C_e.what()));
	}
	CMainTd::I().PostCallback(std::tr1::bind(&CVierOpRijDlg::BedenkResultaat, this, pBedenker));
}

void CVierOpRijDlg::CMijnZetBedenker::ScoreBepaald(int plek, int score)
{
	SScoreBepaald params;
	params.pBedenker	= this;
	params.plek			= plek;
	params.score		= score;
	CMainTd::I().PostCallback(std::tr1::bind(&CVierOpRijDlg::ScoreBepaald, m_Dlg, params));
}


void CVierOpRijDlg::ScoreBepaald(SScoreBepaald params)
{
	if(m_BezigeBedenkerPtr != params.pBedenker)
		return; //Niet deze bedenker meer...
	m_VierOpRijWnd.SetScore(params.plek, params.score);
	StatsBijwerken();
}

void CVierOpRijDlg::StatsBijwerken()
{
	if(m_BezigeBedenkerPtr == NULL)
		return; //Niet deze bedenker meer...
	{
		clock_t tijd = clock() - m_BezigeBedenkerPtr->m_Timestamp_Begin;

		std::wstringstream stats;
		stats 
			<< "Diepte: " << m_BezigeBedenkerPtr->m_ZoekDiepte
			<< "  Ev: " << m_BezigeBedenkerPtr->m_statEvals
			<< "  Pl: " << m_BezigeBedenkerPtr->m_statPleurs
			<< "  W: " << m_BezigeBedenkerPtr->m_statWins
			<< "  NP: " << m_BezigeBedenkerPtr->GetPleursDieNietHoeven()
			<< "  V: " << m_BezigeBedenkerPtr->GetVulling() << "%"
			<< "  Tijd: " << (int)(tijd / 1000) << "." << std::setfill(L'0') << std::setw(3) << (int)(tijd % 1000);
		m_VierOpRijWnd.SetStats(stats.str());
	}

	{
		std::wstringstream stats;
		int tel = 0;
		for(CZetBedenker::DiepteProgressLijst::reverse_iterator i = m_BezigeBedenkerPtr->m_DiepteProgressLijst.rbegin(); i != m_BezigeBedenkerPtr->m_DiepteProgressLijst.rend(); ++i)
		{
			stats
				<< std::setw(2) << tel++ << " " << i->volgorde << " " << i->plek;
			if(i->beste >= 0)
				stats << " b:" << i->beste << "|" << i->besteWaarde;
			stats << std::endl;		
		}
		m_VierOpRijWnd.SetProgress(stats.str());
	}

}


void CVierOpRijDlg::BedenkResultaat(CMijnZetBedenker* pBedenker)
{
	if(m_BezigeBedenkerPtr == pBedenker)
	{
		bool W_bWinst = m_BezigeBedenkerPtr->Winst();
		if(m_BezigeBedenkerPtr->Zet() < 0)
			AfxMessageBox(L"Ik weet nix.. Verzin jij maar wat voor mij.");
		else
			Pleur(m_BezigeBedenkerPtr->Zet());
		if(W_bWinst)
			AfxMessageBox(L"Geef maar op! Ik win toch!");
		StopBedenker();
	}

	delete pBedenker;
}

void CVierOpRijDlg::StopBedenker()
{
	if(m_BezigeBedenkerPtr == NULL)
		return;
	m_BezigeBedenkerPtr->Abort();
	m_BezigeBedenkerPtr = NULL;
}

void CVierOpRijDlg::StartBedenker()
{
	JStd::Threading::ExecAsync(std::tr1::bind(&CVierOpRijDlg::AsyncBedenk, this, 
				m_BezigeBedenkerPtr = new CMijnZetBedenker(m_VierOpRijWnd.Veld(), this)));
	m_VierOpRijWnd.ResetScores();
}

void CVierOpRijDlg::Pleur(int plek)
{
	try
	{
		if(plek >= 0)
		{
			m_VierOpRijWnd.Pleur(plek);
			StopBedenker();
			switch(m_VierOpRijWnd.Veld().Win())
			{
			case 1: AfxMessageBox(L"Rood heeft gewonnen. Blij."); break;
			case 2: AfxMessageBox(L"Geel heeft gewonnen. Blij."); break;
			}
		}
//		if(m_VierOpRijWnd.Veld().Beurt() == 2 || plek < 0)
		if(plek < 0)
			StartBedenker();
	}
	catch(std::exception&)
	{
		AfxMessageBox(L"Hee joh, dat kan helemaal niet!");
	}
}


BOOL CVierOpRijDlg::PreTranslateMessage(MSG* pMsg)
{
	if(GetFocus() == GetDlgItem(IDC_EDIT_LEVEL))
		return __super::PreTranslateMessage(pMsg);

	try
	{
		switch(pMsg->message)
		{
		case WM_CHAR:
			switch(pMsg->wParam)
			{
			case 'r':
				m_VierOpRijWnd.SetVeld(VierOpRijVeld(), true);
				return TRUE;
			case 'u':
				if(m_VierOpRijWnd.Undo())
					return TRUE;
				break;
			case 'b':
				Pleur(-1);
				return TRUE;
			case 'o':
				{
					VierOpRijVeld veld = m_VierOpRijWnd.Veld();
					veld.BeurtOverslaan();
					m_VierOpRijWnd.SetVeld(veld);
				}
				return TRUE;
			default:
				{
					char plek = pMsg->wParam - '0';
					if(plek >= 1 && plek <= 7)
					{
						Pleur(plek-1);
						return TRUE;
					}
				}
				break; //default
			}//switch
			break;//WM_CHAR
		}//switch
	}
	catch(std::exception& C_e)
	{
		AfxMessageBox(L"Foutje: " + CString(C_e.what()));
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CVierOpRijDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(!::IsWindow(m_VierOpRijWnd.m_hWnd))
		return;
	RelocateControls();
}

void CVierOpRijDlg::RelocateControls()
{
	CRect rectLevel;
	GetDlgItem(IDC_EDIT_LEVEL)->GetWindowRect(rectLevel);

	ScreenToClient(rectLevel);

	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.DeflateRect(10,10);

	rectClient.top = rectLevel.bottom + 1;

	m_VierOpRijWnd.MoveWindow(rectClient);

}

void CVierOpRijDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
	switch(nIDEvent)
	{
	case 0:
		StatsBijwerken();
		break;
	}
}
