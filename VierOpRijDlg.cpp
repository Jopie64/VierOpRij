
// VierOpRijDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VierOpRij.h"
#include "VierOpRijDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVierOpRijDlg dialog




CVierOpRijDlg::CVierOpRijDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVierOpRijDlg::IDD, pParent)
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


void CVierOpRijDlg::Pleur(int plek)
{
	m_VierOpRijWnd.Pleur(plek);
	switch(m_VierOpRijWnd.Veld().Win())
	{
	case 1: AfxMessageBox(L"Rood heeft gewonnen. Blij."); break;
	case 2: AfxMessageBox(L"Geel heeft gewonnen. Blij."); break;
	}
	if(m_VierOpRijWnd.Veld().Beurt() == 2)
	{
		int besteZet = m_VierOpRijWnd.Veld().BesteZet(10);
		if(besteZet < 0)
			AfxMessageBox(L"Ik weet nix.. Verzin jij maar wat voor mij.");
		else
			Pleur(besteZet);
	}
}


BOOL CVierOpRijDlg::PreTranslateMessage(MSG* pMsg)
{
	try
	{
		switch(pMsg->message)
		{
		case WM_CHAR:
			switch(pMsg->wParam)
			{
			case 'r':
				m_VierOpRijWnd.SetVeld(VierOpRijVeld());
				break;
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
		//AfxMessageBox(C_e.what());
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
