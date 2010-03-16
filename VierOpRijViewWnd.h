#pragma once

#include "VierOpRijVeld.h"
// CVierOpRijViewWnd

class CVierOpRijViewWnd : public CWnd
{
	DECLARE_DYNAMIC(CVierOpRijViewWnd)

public:
	CVierOpRijViewWnd();
	virtual ~CVierOpRijViewWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

public:
	void SetVeld(const VierOpRijVeld& veld){m_Veld = veld; Invalidate(FALSE);}
private:
	VierOpRijVeld m_Veld;
};


