
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

	class CMijnZetBedenker : public CZetBedenker 
	{
	public:
		CMijnZetBedenker(const VierOpRijVeld& veld, CVierOpRijDlg* dlg):CZetBedenker(veld),m_Dlg(dlg){}

		virtual void ScoreBepaald(int plek, int score);

		CVierOpRijDlg* m_Dlg;

	};

	struct SScoreBepaald
	{
		CMijnZetBedenker* pBedenker;
		int plek;
		int score;
	};

// Dialog Data
	enum { IDD = IDD_VIEROPRIJ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void StopBedenker();
	void Pleur(int plek);

	void StartBedenker();
	void AsyncBedenk(CMijnZetBedenker* pBedenker);
	void BedenkResultaat(CMijnZetBedenker* pBedenker);
	void ScoreBepaald(SScoreBepaald params);


// Implementation
protected:
	HICON m_hIcon;

	CVierOpRijViewWnd m_VierOpRijWnd;

	CMijnZetBedenker* m_BezigeBedenkerPtr;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
