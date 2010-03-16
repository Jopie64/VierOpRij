
// VierOpRijDlg.h : header file
//

#pragma once

#include "VierOpRijViewWnd.h"

// CVierOpRijDlg dialog
class CVierOpRijDlg : public CDialog
{
// Construction
public:
	CVierOpRijDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VIEROPRIJ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	CVierOpRijViewWnd m_VierOpRijWnd;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
