#pragma once

#include <vector>
#include <time.h>

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

	void		BeurtOverslaan(){VolgendeBeurt();}

	bool		IsZelfdeVeld(const VierOpRijVeld& veld)const;
	inline unsigned int  Hash()const;

	inline int	AantalZettenOver()const{return Sm_Breedte * Sm_Hoogte - m_Aantal;}
	inline bool	Vol()const{return AantalZettenOver() == 0;}
	inline int	GetAantal()const{return m_Aantal;}

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
    unsigned long long m_Hash;

	friend class CZetBedenker;

};

class CZetBedenker
{
public:
	class DiepteProgress
	{
	public:
		DiepteProgress():volgorde(0), plek(0), beste(-1), besteWaarde(0){}
		int volgorde;
		int plek;
		int beste;
		int besteWaarde;
	};
	typedef std::vector<DiepteProgress> DiepteProgressLijst;

	static const int Sm_PlusMax = 1000000000;
	static const int Sm_MinMax  = -Sm_PlusMax;

	CZetBedenker(const VierOpRijVeld& veld)
	:	m_Veld(veld),m_Zet(-1),m_bAbort(false),m_bWinst(false),
		m_statEvals(0),m_statPleurs(0), m_statWins(0), m_ZoekDiepte(0),m_Timestamp_Begin(clock()){}
	int BedenkZet(int zoekDiepte);

	int  Zet()const{return m_Zet;}
	bool Winst()const{return m_bWinst;}
	void Abort(){m_bAbort = true;}

	virtual void ScoreBepaald(int plek, int score){}

	static bool IsWinWaarde(int waarde){return abs(waarde) >= Sm_PlusMax - VierOpRijVeld::Sm_Breedte * VierOpRijVeld::Sm_Hoogte;}

private:
//	char SpeelWillekeurigSpel(VierOpRijVeld& veld, int diepte);
	int Evalueer(const VierOpRijVeld& veld);
	int Minimax(VierOpRijVeld& veld, int zoekDiepte);
	int BepaalScore(VierOpRijVeld& veld, int zoekDiepte, int alpha, int beta, int* pZet);
	void BepaalVolgorde(VierOpRijVeld& veld, int (& volgorde)[VierOpRijVeld::Sm_Breedte], int zoekDiepte);

	VierOpRijVeld	m_Veld;
	int				m_Zet;
	bool			m_bWinst;
	bool			m_bAbort;


public:
	int					GetPleursDieNietHoeven()const;
	int					GetVulling()const;

	clock_t				m_Timestamp_Begin;
	DiepteProgressLijst m_DiepteProgressLijst;
	int					m_statEvals;
	int					m_statPleurs;
	int					m_statWins;
	int					m_statPleursDieNietHoevenBegin;
	int					m_ZoekDiepte;
};
