#include "StdAfx.h"
#include "VierOpRijVeld.h"

VierOpRijVeld::VierOpRijVeld(void)
:	m_Beurt(1),
	m_Win(0)
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

	return m_Veld[x][y];
}

int VierOpRijVeld::PleurUnchecked(int plek)
{
	for(int i=0; i<Sm_Hoogte; ++i)
		if(m_Veld[plek][i] == 0)
		{
			m_Veld[plek][i] = VolgendeBeurt();
			m_Win = Win(plek, i);
			return i;
		}
	return -1;

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

const int zetVolgorde[] = {3, 2, 4, 1, 5, 0, 6};
const int plusMax = 1000000000;
const int minMax  = -plusMax;

class CAlphabeta
{
public:
	static int BepaalScore(const VierOpRijVeld& veld, int zoekDiepte, int alpha, int beta, int* pZet)
	{
		if(veld.Win() != 0)
			return minMax;
		if(zoekDiepte == 0)
			return 0;//Eventueel evalueren...

		VierOpRijVeld veldMetZet(veld);
		for(int i = 0; i < VierOpRijVeld::Sm_Breedte; ++i)
		{
			if(veldMetZet.PleurUnchecked(zetVolgorde[i]) < 0)
				continue;//Kan niet hier...
			int zetScore = -BepaalScore(veldMetZet, zoekDiepte - 1, -beta, -alpha, NULL);
			if(pZet == NULL)
				alpha = __max(alpha, zetScore);
			else
			{
				if(zetScore > alpha)
				{
					alpha = zetScore;
					*pZet = zetVolgorde[i];						
				}
			}

			if(beta < alpha)
				break; //Jay! Beta cutoff. Scheelt weer wat tijd.
			veldMetZet = veld; //Weer even terug... (Hmm gebeurt vaak.. Misschien iets als undo maken ofzo)
		}
		return alpha;
	}
};

int VierOpRijVeld::BesteZet(int zoekDiepte) const
{
	int besteZet = -1;
	CAlphabeta::BepaalScore(*this, zoekDiepte, minMax, plusMax, &besteZet);
	return besteZet;
}
