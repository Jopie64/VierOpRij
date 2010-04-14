#include "StdAfx.h"
#include "VierOpRijVeld.h"
#include <algorithm>
#include <vector>

class CVierOpRijWeegschaal : public VierOpRijVeld
{
public:
	CVierOpRijWeegschaal(){BepaalWeegschaal();}

	void BepaalWeegschaal();

};

CVierOpRijWeegschaal G_Weegschaal;

VierOpRijVeld::VierOpRijVeld(void)
:	m_Beurt(1),
	m_Win(0),
	m_Aantal(0)
{
	memset(m_Veld,0,sizeof(m_Veld));
	memset(m_Hoogte,0,sizeof(m_Hoogte));
	memset(m_SpelerWeegschaal, 0, sizeof(m_SpelerWeegschaal));
}

VierOpRijVeld::~VierOpRijVeld(void)
{
}

char VierOpRijVeld::VolgendeBeurt()
{
	char huidigeBeurt = m_Beurt;
	++m_Beurt;
	if(m_Beurt > Sm_Spelers)
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
	char& hoogte = m_Hoogte[plek];
	if(hoogte >= Sm_Hoogte)
		return -1;
	
	PlaatsUnchecked(VolgendeBeurt(), plek, hoogte);
//	PlaatsUnchecked(m_Beurt, plek, hoogte);
//	m_Beurt ^= 0x3;
	return hoogte++;
}

void VierOpRijVeld::PlaatsUnchecked(int speler, int x, int y)
{
	m_Veld[x][y] = speler;
	++m_Aantal;
	m_SpelerWeegschaal[speler - 1] += G_Weegschaal.m_Veld[x][y];
	m_Win = Win(x, y);
}

char VierOpRijVeld::Win() const
{
	return m_Win;
}

char VierOpRijVeld::Win(int xHint, int yHint)
{
	char* kolomX = m_Veld[xHint];
	char beurt = kolomX[yHint];
//	int linker = __min(xHint - 3, 0);
//	int beneden = 
	int gelijk;
	int x;
	int y;
	
	//Verticaal
	gelijk = 1;
	y = yHint - 1;
	while(y >= 0 && gelijk < 4 && kolomX[y] == beurt)
	{
		--y;
		++gelijk;
	}
	
	y = yHint + 1;
	while(y < Sm_Hoogte && gelijk < 4 && kolomX[y] == beurt)
	{
		++y;
		++gelijk;
	}
	if(gelijk >= 4)
		return beurt;

	//Horizontaal
	gelijk = 1;
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

int VierOpRijVeld::Waarde(char speler)const
{
	--speler; //speler is altijd 1 of 2. Moet hier 0 of 1 zijn.
	int waarde = 0;
	for(int i=0; i<Sm_Spelers; ++i)
		if(speler == i)
			waarde += m_SpelerWeegschaal[i];
		else
			waarde -= m_SpelerWeegschaal[i];
	return waarde;
}


void CVierOpRijWeegschaal::BepaalWeegschaal()
{
	for(int x = 0; x < Sm_Breedte; ++x)
		for(int y = 0; y < Sm_Hoogte; ++y)
		{
			// _ en /
			if(x <= Sm_Breedte - 4)
			{
				for(int i = 0; i < 4; ++i)
					++m_Veld[x + i][y];
				// /
				if(y <= Sm_Hoogte - 4)
					for(int i = 0; i < 4; ++i)
						++m_Veld[x + i][y + i];
			}

			// | en \ dus
			if(y <= Sm_Hoogte - 4)
			{
				for(int i = 0; i < 4; ++i)
					++m_Veld[x][y + i];
				if(x >= 3)
					for(int i = 0; i < 4; ++i)
						++m_Veld[x - i][y + i];
			}
		}
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

/* //Willekeurig spel
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

*/

// Weegschaal
int CZetBedenker::Evalueer(const VierOpRijVeld& veld)
{
	++m_statEvals;
	return veld.Waarde();
}

const int zetVolgorde[] = {3, 3, 3, 3, 2, 2, 4, 4, 1, 5, 0, 6};
//const int zetVolgorde[] = {3, 3, 3, 3, 2, 4, 2, 4, 2, 4, 1, 5, 1, 5, 0, 6};

int PakVolgordePlek(int nummer)
{
	return zetVolgorde[nummer % (sizeof(zetVolgorde) / sizeof(int))];
}

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

void CZetBedenker::BepaalVolgorde(const VierOpRijVeld& veld, int (& volgorde)[VierOpRijVeld::Sm_Breedte], int zoekDiepte)
{
	zoekDiepte -= 2; //4 niveaus minder doorzoeken.
	if(zoekDiepte <= 0)
		return; // Volgorde is niet boeiend.

//	return;
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
//			plekScore[i].score = BepaalScore(werkVeld, zoekDiepte, Sm_MinMax, Sm_PlusMax, NULL);
			plekScore[i].score = -werkVeld.Waarde();
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
//		return Sm_MinMax - zoekDiepte;   //Pff, vorige speler heeft al gewonnen. Lekker eerlijk. (Oja, het is nog erger als de diepte nog hoog is.)
		return Sm_MinMax;
	}
	if(	m_bAbort ||			//Stoppen maar. Gebruiker heeft geen geduld.
		veld.m_Aantal >= VierOpRijVeld::Sm_Breedte * VierOpRijVeld::Sm_Hoogte) //Veld zit vol. Heeft ook niet veel zin meer hè...
		return 0;
	if( zoekDiepte == 0)  //Niet meer verder zoeken (anders duurt 't beetje lang hè)
		return Evalueer(veld);//Eventueel evalueren...

	bool plekGebruikt[VierOpRijVeld::Sm_Breedte];
	memset(plekGebruikt, 0, sizeof(plekGebruikt));
	int volgorde[VierOpRijVeld::Sm_Breedte];
	int j = 0;
	for(int i = 0; j < VierOpRijVeld::Sm_Breedte; ++i)
	{
		int plek = PakVolgordePlek(i + veld.m_Aantal);
		if(plekGebruikt[plek])
			continue;
		plekGebruikt[plek] = true;
		volgorde[j++] = plek;
	}

	//BepaalVolgorde(veld, volgorde, zoekDiepte);

	std::vector<VierOpRijVeld> veldenMetZet(VierOpRijVeld::Sm_Breedte, veld);
	//for(int i = 0; i < VierOpRijVeld::Sm_Breedte; ++i)
	int tel = 0;
	int tel2 = 0;
	for(std::vector<VierOpRijVeld>::iterator i = veldenMetZet.begin(); i != veldenMetZet.end();)
	{
		VierOpRijVeld& veldMetZet = *i;
		int plek = PakVolgordePlek(volgorde, tel++);
//		int plek = PakVolgordePlek(zetVolgorde, i + veld.m_Aantal);
		if(veldMetZet.PleurUnchecked(plek) < 0)
		{
			i = veldenMetZet.erase(i);
			continue;//Kan niet hier...
		}
		++m_statPleurs;
		if(veldMetZet.Win() != 0)
		{
			++m_statWins;
			return Sm_PlusMax;
		}
		volgorde[tel2++] = plek;//Volgorde wordt opnieuw bepaald, zonder de plekken waar geen steen kan
		++i;
	}

	tel = 0;
	for(std::vector<VierOpRijVeld>::iterator i = veldenMetZet.begin(); i != veldenMetZet.end(); ++i)
	{
		VierOpRijVeld& veldMetZet = *i;
		int zetScore = -BepaalScore(veldMetZet, zoekDiepte - 1, -beta, -alpha, NULL);
		if(pZet == NULL)
		{
			if(zetScore > alpha)
				alpha = zetScore;
		}
//			alpha = __max(alpha, zetScore);
		else
		{
			int plek = volgorde[tel++];
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
//		veldMetZet = veld; //Weer even terug... (Hmm gebeurt vaak.. Misschien iets als undo maken ofzo)
	}
	return alpha;
}

int CZetBedenker::BedenkZet(int zoekDiepte)
{
	m_bWinst = false;
	if(zoekDiepte < 0)
		zoekDiepte = max(14, m_Veld.m_Aantal);
	m_ZoekDiepte = zoekDiepte;
	BepaalScore(m_Veld, zoekDiepte, Sm_MinMax, Sm_PlusMax, &m_Zet);
	return m_Zet;
}

