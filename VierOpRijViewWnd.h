#pragma once

#include "VierOpRijVeld.h"
#include <vector>
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
	void				 SetVeld(const VierOpRijVeld& veld, bool leegHistorie = false){if(leegHistorie)m_Veld.clear(); m_Veld.push_back(veld); if(m_hWnd != NULL)Invalidate(FALSE);}
	void				 Pleur(int P_iPlek){VolgendeVeld().Pleur(P_iPlek); Invalidate(FALSE);}
	bool				 Undo(){if(m_Veld.size() <= 1) return false; m_Veld.resize(m_Veld.size() - 1); Invalidate(FALSE); return true;}
	const VierOpRijVeld& Veld()const{return m_Veld.back();}
	void				 SetScore(int plek, int score){m_Scores[plek].Set(score); Invalidate(FALSE);}
	void				 ResetScores(){for(int i=0; i<VierOpRijVeld::Sm_Breedte; ++i)m_Scores[i].m_bBekend = false; Invalidate(FALSE);}
	void				 SetStats(const std::wstring& stats){/*if(m_Stats == stats)return;*/ m_Stats = stats; Invalidate(FALSE);}
	void				 SetProgress(const std::wstring& progress){/*if(m_Stats == stats)return;*/ m_Progress = progress; Invalidate(FALSE);}

private:
	class Score
	{
	public:
		Score():m_bBekend(false){}
		bool m_bBekend;
		int  m_Waarde;

		void Set(int waarde){m_bBekend = true; m_Waarde = waarde;}
	};

	VierOpRijVeld&		VolgendeVeld(){m_Veld.push_back(m_Veld.back()); return m_Veld.back();}

	std::vector<VierOpRijVeld>	m_Veld;
	Score						m_Scores[VierOpRijVeld::Sm_Breedte];
	std::wstring				m_Stats;
	std::wstring				m_Progress;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


