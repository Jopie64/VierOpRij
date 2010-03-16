#pragma once

class VierOpRijVeld
{
public:

	VierOpRijVeld(void);
	~VierOpRijVeld(void);

	void Pleur(int plek);
	char Win();

private:
	int PleurUnchecked(int plek);
	char VolgendeBeurt();
	char Win(int xHint, int yHint);

	static const int Sm_Breedte = 7;
	static const int Sm_Hoogte = 6;

	char m_Veld[Sm_Breedte][Sm_Hoogte];
	char m_Beurt;
	char m_Win;


};
