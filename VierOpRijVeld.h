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

	inline char Beurt()const{return m_Beurt;}

	inline bool Vol()const{return m_Aantal == Sm_Breedte * Sm_Hoogte;}
	void		BeurtOverslaan(){VolgendeBeurt();}

private:
	int PleurUnchecked(int plek);
	inline void PlaatsUnchecked(int speler, int x, int y);
	inline char WieUnchecked(int x, int y)const{return m_Veld[x][y];}
	char VolgendeBeurt();
	char Win(int xHint, int yHint);

	char m_Veld[Sm_Breedte][Sm_Hoogte];
	char m_Beurt;
	char m_Win;
	int  m_Aantal;

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
	char SpeelWillekeurigSpel(VierOpRijVeld& veld, int diepte);
	int Evalueer(const VierOpRijVeld& veld);
	int BepaalScore(const VierOpRijVeld& veld, int zoekDiepte, int alpha, int beta, int* pZet);
	void BepaalVolgorde(const VierOpRijVeld& veld, int (& volgorde)[VierOpRijVeld::Sm_Breedte]);

	VierOpRijVeld	m_Veld;
	int				m_Zet;
	bool			m_bWinst;
	bool			m_bAbort;
};