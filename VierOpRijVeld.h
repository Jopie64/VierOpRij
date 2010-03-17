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
	int BedenkZet(const VierOpRijVeld& veld, int zoekDiepte);

private:
	int BepaalScore(const VierOpRijVeld& veld, int zoekDiepte, int alpha, int beta, int* pZet);
};