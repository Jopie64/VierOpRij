#pragma once

class VierOpRijVeld
{
public:
	static const int Sm_Breedte = 7;
	static const int Sm_Hoogte = 6;

	VierOpRijVeld(void);
	~VierOpRijVeld(void);

	void Pleur(int plek);
	char Win()const;
	char Wie(int x, int y)const;

	char Beurt()const{return m_Beurt;}

private:
	int PleurUnchecked(int plek);
	char VolgendeBeurt();
	char Win(int xHint, int yHint);

	char m_Veld[Sm_Breedte][Sm_Hoogte];
	char m_Beurt;
	char m_Win;

	friend class CZetBedenker;

};


class CZetBedenker
{
public:
	static const int Sm_PlusMax = 1000000000;
	static const int Sm_MinMax  = -Sm_PlusMax;

	CZetBedenker(const VierOpRijVeld& veld):m_Veld(veld),m_Zet(-1),m_bAbort(false),m_bWinst(false){}
	int BedenkZet(int zoekDiepte);

	int  Zet()const{return m_Zet;}
	bool Winst()const{return m_bWinst;}
	void Abort(){m_bAbort = true;}

	virtual void ScoreBepaald(int plek, int score){}

private:
	int BepaalScore(const VierOpRijVeld& veld, int zoekDiepte, int alpha, int beta, int* pZet);

	VierOpRijVeld	m_Veld;
	int				m_Zet;
	bool			m_bWinst;
	bool			m_bAbort;
};