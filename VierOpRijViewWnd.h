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
	void				 SetVeld(const VierOpRijVeld& veld){m_Veld = veld; Invalidate(FALSE);}
	void				 Pleur(int P_iPlek){m_Veld.Pleur(P_iPlek); Invalidate(FALSE);}
	const VierOpRijVeld& Veld()const{return m_Veld;}
private:
	VierOpRijVeld m_Veld;
};


