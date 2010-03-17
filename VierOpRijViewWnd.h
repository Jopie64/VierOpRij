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
	void				 SetScore(int plek, int score){m_Scores[plek].Set(score); Invalidate(FALSE);}
	void				 ResetScores(){for(int i=0; i<VierOpRijVeld::Sm_Breedte; ++i)m_Scores[i].m_bBekend = false; Invalidate(FALSE);}

private:
	class Score
	{
	public:
		Score():m_bBekend(false){}
		bool m_bBekend;
		int  m_Waarde;

		void Set(int waarde){m_bBekend = true; m_Waarde = waarde;}
	};

	VierOpRijVeld	m_Veld;
	Score			m_Scores[VierOpRijVeld::Sm_Breedte];
};


