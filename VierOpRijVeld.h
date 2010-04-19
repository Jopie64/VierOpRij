#pragma once

#include <vector>

class VierOpRijVeld
{
public:
	static const int Sm_Breedte = 7;
	static const int Sm_Hoogte = 6;
	static const int Sm_Spelers = 2;

	VierOpRijVeld(void);
	~VierOpRijVeld(void);

	void Pleur(int plek);
	char Win()const;
	char Wie(int x, int y)const;
	int	 Waarde(char speler)const;
	int  Waarde()const{return Waarde(m_Beurt);}

	inline char Beurt()const{return m_Beurt;}

	inline bool Vol()const{return m_Aantal == Sm_Breedte * Sm_Hoogte;}
	void		BeurtOverslaan(){VolgendeBeurt();}

protected:
	int PleurUnchecked(int plek);
	int UnpleurUnchecked(int plek);
	inline void PlaatsUnchecked(int speler, int x, int y);
	inline char WieUnchecked(int x, int y)const{return m_Veld[x][y];}
	inline char VolgendeBeurt();
	inline char VorigeBeurt();
	char Win(int xHint, int yHint);

	char m_Veld[Sm_Breedte][Sm_Hoogte];
	char m_Hoogte[Sm_Breedte];
	char m_Beurt;
	char m_Win;
	int  m_Aantal;
	int	 m_SpelerWeegschaal[Sm_Spelers];

	friend class CZetBedenker;

};

class CZetBedenker
{
public:
	class DiepteProgress
	{
	public:
		DiepteProgress():volgorde(0), plek(0){}
		int volgorde;
		int plek;
	};
	typedef std::vector<DiepteProgress> DiepteProgressLijst;

	static const int Sm_PlusMax = 1000000000;
	static const int Sm_MinMax  = -Sm_PlusMax;

	CZetBedenker(const VierOpRijVeld& veld)
	:	m_Veld(veld),m_Zet(-1),m_bAbort(false),m_bWinst(false),
		m_statEvals(0),m_statPleurs(0), m_statWins(0), m_ZoekDiepte(0),m_Timestamp_Begin(CTime::GetCurrentTime()){}
	int BedenkZet(int zoekDiepte);

	int  Zet()const{return m_Zet;}
	bool Winst()const{return m_bWinst;}
	void Abort(){m_bAbort = true;}

	virtual void ScoreBepaald(int plek, int score){}

private:
//	char SpeelWillekeurigSpel(VierOpRijVeld& veld, int diepte);
	int Evalueer(const VierOpRijVeld& veld);
	int BepaalScore(VierOpRijVeld& veld, int zoekDiepte, int alpha, int beta, int* pZet);
	void BepaalVolgorde(const VierOpRijVeld& veld, int (& volgorde)[VierOpRijVeld::Sm_Breedte], int zoekDiepte);

	VierOpRijVeld	m_Veld;
	int				m_Zet;
	bool			m_bWinst;
	bool			m_bAbort;


public:
	CTime				m_Timestamp_Begin;
	DiepteProgressLijst m_DiepteProgressLijst;
	int					m_statEvals;
	int					m_statPleurs;
	int					m_statWins;
	int					m_ZoekDiepte;
};