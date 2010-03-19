#include "StdAfx.h"
#include "VierOpRijVeld.h"
#include <algorithm>

VierOpRijVeld::VierOpRijVeld(void)
:	m_Beurt(1),
	m_Win(0),
	m_Aantal(0)
{
	memset(m_Veld,0,sizeof(m_Veld));
}

VierOpRijVeld::~VierOpRijVeld(void)
{
}

char VierOpRijVeld::VolgendeBeurt()
{
	char huidigeBeurt = m_Beurt;
	++m_Beurt;
	if(m_Beurt > 2)
		m_Beurt = 1;
	return huidigeBeurt;
}

void VierOpRijVeld::Pleur(int plek)
{
	if(plek < 0 || plek >= Sm_Breedte)
		throw std::runtime_error("Foute plek l*l");
	if(PleurUnchecked(plek) < 0)
		throw std::runtime_error("Hier zit ie vol...");
}

char VierOpRijVeld::Wie(int x, int y) const
{
	if(x < 0 || x >= Sm_Breedte || y < 0 || y >= Sm_Hoogte)
		throw std::runtime_error("Die valt dus buiten het veld. Niet zo slim.");

	return WieUnchecked(x, y);
}

int VierOpRijVeld::PleurUnchecked(int plek)
{
	for(int i=0; i<Sm_Hoogte; ++i)
		if(m_Veld[plek][i] == 0)
		{
			PlaatsUnchecked(VolgendeBeurt(), plek, i);
			return i;
		}
	return -1;

}

void VierOpRijVeld::PlaatsUnchecked(int speler, int x, int y)
{
	m_Veld[x][y] = speler;
	++m_Aantal;
	m_Win = Win(x, y);
}

char VierOpRijVeld::Win() const
{
	return m_Win;
}

char VierOpRijVeld::Win(int xHint, int yHint)
{
	char beurt = m_Veld[xHint][yHint];
//	int linker = __min(xHint - 3, 0);
//	int beneden = 
	int gelijk = 1;
	int x;
	int y;
	
	//Horizontaal
	x = xHint - 1;
	while(x >= 0 && gelijk < 4 && m_Veld[x][yHint] == beurt)
	{
		--x;
		++gelijk;
	}

	x = xHint + 1;
	while(x < Sm_Breedte && gelijk < 4 && m_Veld[x][yHint] == beurt)
	{
		++x;
		++gelijk;
	}
	if(gelijk >= 4)
		return beurt;

	//Verticaal
	gelijk = 1;
	y = yHint - 1;
	while(y >= 0 && gelijk < 4 && m_Veld[xHint][y] == beurt)
	{
		--y;
		++gelijk;
	}
	
	y = yHint + 1;
	while(y < Sm_Hoogte && gelijk < 4 && m_Veld[xHint][y] == beurt)
	{
		++y;
		++gelijk;
	}
	if(gelijk >= 4)
		return beurt;

	//Diagonaal /
	gelijk = 1;
	x = xHint - 1;
	y = yHint - 1;
	while(x >= 0 && y >= 0 && gelijk < 4 && m_Veld[x][y] == beurt)
	{
		--x;
		--y;
		++gelijk;
	}
	
	x = xHint + 1;
	y = yHint + 1;
	while(x < Sm_Breedte && y < Sm_Hoogte && gelijk < 4 && m_Veld[x][y] == beurt)
	{
		++x;
		++y;
		++gelijk;
	}
	if(gelijk >= 4)
		return beurt;

	//Diagonaal \ 
	gelijk = 1;
	x = xHint - 1;
	y = yHint + 1;
	while(x >= 0 && y < Sm_Hoogte && gelijk < 4 && m_Veld[x][y] == beurt)
	{
		--x;
		++y;
		++gelijk;
	}
	
	x = xHint + 1;
	y = yHint - 1;
	while(x < Sm_Breedte && y >= 0 && gelijk < 4 && m_Veld[x][y] == beurt)
	{
		++x;
		--y;
		++gelijk;
	}
	if(gelijk >= 4)
		return beurt;

	return 0; //Niemand gewonnen
}


/*
int CZetBedenker::Evalueer(const VierOpRijVeld& veld)
{
	int waarde;
	VierOpRijVeld werkVeld(veld);
	waarde = Evalueer(werkVeld, 2, true);
//	waarde += BepaalScore(veld, 1, Sm_MinMax, Sm_PlusMax, NULL);
	werkVeld.VolgendeBeurt();
	waarde -= Evalueer(werkVeld, 2, false);
	return waarde;
}

int CZetBedenker::Evalueer(const VierOpRijVeld& veld, int diepte, bool eerste)
{
	if(veld.Vol() || diepte == 0)
		return 0;


	int waarde = 0;
	for(int y = 0; y < VierOpRijVeld::Sm_Hoogte; ++y)
		for(int x = 0; x < VierOpRijVeld::Sm_Breedte; ++x)
		{
			if(veld.WieUnchecked(x, y) != 0)
				continue;
			VierOpRijVeld werkVeld(veld);
			werkVeld.PlaatsUnchecked(werkVeld.Beurt(), x, y);
			if(werkVeld.Win())
			{
				if(eerste && (y == 0 || veld.WieUnchecked(x, y - 1) != 0) )
					return Sm_PlusMax; //Wint gelijk. Dus hoef niet meer verder te zoeken hè.
				waarde += 1 + Evalueer(werkVeld, diepte - 1, false) * 10;
			}
		}

	return waarde;
}
*/
int CZetBedenker::Evalueer(const VierOpRijVeld& veld)
{
	++m_statEvals;
	int waarde = 0;
	char beurt = veld.Beurt();
	for(int i = 0; i < 0; ++i)
	{
		VierOpRijVeld werkVeld(veld);
		char win = SpeelWillekeurigSpel(werkVeld, 5);
		if(win == beurt)
			++waarde;
		else if(win != 0)
			--waarde;
	}
	return waarde;
}

char CZetBedenker::SpeelWillekeurigSpel(VierOpRijVeld& veld, int diepte)
{
	if(veld.Win() != 0)
		return veld.Win();
	if(veld.Vol() || diepte == 0)
		return 0;
	veld.PleurUnchecked(rand() % 7);
	return SpeelWillekeurigSpel(veld, diepte - 1);	
}

const int zetVolgorde[] = {3, 2, 4, 1, 5, 0, 6};

int PakVolgordePlek(const int* volgorde, int plek)
{
	return volgorde[plek % VierOpRijVeld::Sm_Breedte];
}

/*class CVolgordeBedenker : public CZetBedenker
{
public:
	CVolgordeBedenker(const VierOpRijVeld& veld):CZetBedenker(veld){}//{memset(&m_Score, 0, sizeof(m_Score));}

	virtual void ScoreBepaald(int plek, int score)
	{
		m_Score[plek] = score;
	}

	int m_Score[VierOpRijVeld::Sm_Breedte];
};

void CZetBedenker::BepaalVolgorde(const VierOpRijVeld& veld, int (& volgorde)[VierOpRijVeld::Sm_Breedte])
{
	CVolgordeBedenker volgordeBedenker(veld);
	int plek = volgordeBedenker.BedenkZet(1);
	memcpy(volgorde, zetVolgorde, sizeof(zetVolgorde));
	for(int x = 0; x < VierOpRijVeld::Sm_Breedte; ++x)
		if(volgorde[x] == plek)
		{
			if(x == 0)
				return;
			std::swap(volgorde[x], volgorde[0]);
			return;
		}
}
*/
struct PlekScore
{
	int plek;
	int score;
};

bool PlekScoreSort(const PlekScore& left, const PlekScore& right)
{
	return right.score < left.score;
}

void CZetBedenker::BepaalVolgorde(const VierOpRijVeld& veld, int (& volgorde)[VierOpRijVeld::Sm_Breedte])
{
	PlekScore plekScore[VierOpRijVeld::Sm_Breedte];
	for(int i = 0; i < VierOpRijVeld::Sm_Breedte; ++i)
	{
		int plek = PakVolgordePlek(volgorde, i);
//		int plek = i;
		plekScore[i].plek = plek;
		VierOpRijVeld werkVeld(veld);
		if(werkVeld.PleurUnchecked(plek) < 0)
			plekScore[i].score = Sm_MinMax;
		else
			plekScore[i].score = BepaalScore(werkVeld, 0, Sm_MinMax, Sm_PlusMax, NULL);
//		if(plekScore[i].score != 0)
//			printf("hier");
	}

	std::sort(plekScore, plekScore + VierOpRijVeld::Sm_Breedte, PlekScoreSort);
	for(int i = 0; i < VierOpRijVeld::Sm_Breedte; ++i)
		volgorde[i] = plekScore[i].plek;

}

int CZetBedenker::BepaalScore(const VierOpRijVeld& veld, int zoekDiepte, int alpha, int beta, int* pZet)
{
	if(veld.Win() != 0)
	{
		++m_statWins;
		return Sm_MinMax;   //Pff, vorige speler heeft al gewonnen. Lekker eerlijk.
	}
	if(	m_bAbort ||			//Stoppen maar. Gebruiker heeft geen geduld.
		veld.m_Aantal >= VierOpRijVeld::Sm_Breedte * VierOpRijVeld::Sm_Hoogte) //Veld zit vol. Heeft ook niet veel zin meer hè...
		return 0;
	if( zoekDiepte == 0)  //Niet meer verder zoeken (anders duurt 't beetje lang hè)
		return Evalueer(veld);//Eventueel evalueren...

	int volgorde[VierOpRijVeld::Sm_Breedte];
	for(int i = 0; i < VierOpRijVeld::Sm_Breedte; ++i)
		volgorde[i] = PakVolgordePlek(zetVolgorde, i + veld.m_Aantal);
//	memcpy(volgorde, zetVolgorde, sizeof(volgorde));
//	if(zoekDiepte > 4)
//		BepaalVolgorde(veld, volgorde);

	VierOpRijVeld veldMetZet(veld);
	for(int i = 0; i < VierOpRijVeld::Sm_Breedte; ++i)
	{
		int plek = PakVolgordePlek(volgorde, i);
//		int plek = PakVolgordePlek(zetVolgorde, i + veld.m_Aantal);
		if(veldMetZet.PleurUnchecked(plek) < 0)
			continue;//Kan niet hier...
		++m_statPleurs;
		int zetScore = -BepaalScore(veldMetZet, zoekDiepte - 1, -beta, -alpha, NULL);
		if(pZet == NULL)
			alpha = __max(alpha, zetScore);
		else
		{
			ScoreBepaald(plek, zetScore);
			if(zetScore > alpha)
			{
				alpha = zetScore;
				*pZet = plek;
			}
			if(zetScore == Sm_PlusMax)
				m_bWinst = true;
		}

		if(beta <= alpha)
			break; //Jay! Beta cutoff. Scheelt weer wat tijd.
		veldMetZet = veld; //Weer even terug... (Hmm gebeurt vaak.. Misschien iets als undo maken ofzo)
	}
	return alpha;
}

int CZetBedenker::BedenkZet(int zoekDiepte)
{
	m_bWinst = false;
	if(zoekDiepte < 0)
		zoekDiepte = max(16, m_Veld.m_Aantal);
	m_ZoekDiepte = zoekDiepte;
	BepaalScore(m_Veld, zoekDiepte, Sm_MinMax, Sm_PlusMax, &m_Zet);
	return m_Zet;
}

