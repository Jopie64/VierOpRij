#include "stdafx.h"
#include "VierOpRijVeld.h"
#include <algorithm>
#include <cstring>

using namespace std;

class CVierOpRijCache
{
public:
	static const char G_HashBits = 20; //20 = +- 1 miljoen
	static const int G_HashMasker = (1 << G_HashBits) - 1;
	static const bool G_WerkMetCache = true;
	static const int G_ZoekVerderInCache = 7;
	static const int G_GaNietInCacheZoekenOnderDiepte = 2;

	class CCacheItem
	{
	public:
		CCacheItem():m_Waarde(0), m_Bepaald(false), m_iDiepte(0), m_iPleurs(0){}

		VierOpRijVeld m_Veld;
		int			  m_Waarde;
		bool		  m_Bepaald;
		int			  m_iDiepte; //Gebruikte diepte om de waarde te bepalen
		int			  m_iPleurs; //Aantal pleurs die nodig waren om tot deze conclusie te komen
	};


	CVierOpRijCache():m_CCacheItemLijst(1 << G_HashBits), m_Gevuld(0), m_PleursDieNietNodigWaren(0){}

	bool GetCacheWaarde(const VierOpRijVeld& veld, int& waarde, int diepte)
	{
		if(!G_WerkMetCache || diepte < G_GaNietInCacheZoekenOnderDiepte)
			return false;

		int veldPlek = veld.Hash();
		CCacheItemLijst::iterator it;
		int i;
		for(i = 0; i < G_ZoekVerderInCache; ++i)
		{
			it = m_CCacheItemLijst.begin() + ((veldPlek + i) & G_HashMasker);
			if(!it->m_Bepaald)
				return false; //Nog niet bepaald... Gaan we nu doen.
			if(veld.IsZelfdeVeld(it->m_Veld))
				break; //Gevonden.
		}
		if(i == G_ZoekVerderInCache)
			return false; //Niet gevonden...
		CCacheItem& item = *it;
//		if(!item.m_Bepaald)
//			return false; //Nog niet bepaald... Gaan we nu doen.
		if(item.m_iDiepte < diepte && !CZetBedenker::IsWinWaarde(item.m_Waarde))
			return false; //Waarde bepaald bij verkeerde diepte. Helaas...
//		if(!veld.IsZelfdeVeld(item.m_Veld))
//			return false; //Het is een ander veld met dezelfde hash. Helaas...

		//Een cachehit!! Jay
		m_PleursDieNietNodigWaren += item.m_iPleurs;
		waarde = item.m_Waarde;
		return true;
	}

	int SetCacheItem(const VierOpRijVeld& veld, int waarde, int diepte, int pleurs)
	{
		if(!G_WerkMetCache || diepte < G_GaNietInCacheZoekenOnderDiepte)
			return 0;
		int veldPlek = veld.Hash();
		CCacheItemLijst::iterator itBeste = m_CCacheItemLijst.end();
		for(int i = 0; i < G_ZoekVerderInCache; ++i)
		{
			CCacheItemLijst::iterator it = m_CCacheItemLijst.begin() + ((veldPlek + i) & G_HashMasker);
			if(it->m_iDiepte > diepte)
				continue;//Deze was moeilijker te bepalen. Lekker zo laten.
			if(!it->m_Bepaald)
			{
				itBeste = it;
				break; //Nog niet bepaald. Gelijk kiezen
			}
			if(itBeste == m_CCacheItemLijst.end() || it->m_iDiepte < itBeste->m_iDiepte)
				itBeste = it;
		}
		if(itBeste == m_CCacheItemLijst.end())
			return 1;

		CCacheItem& item = *itBeste;
//		if(item.m_iDiepte > diepte)
//			return 1; //Deze was moeilijker te bepalen. Lekker zo laten.
//		bool isNietZelfdeVeld = item.m_Bepaald && !veld.IsZelfdeVeld(item.m_Veld);
//		if(isNietZelfdeVeld && item.m_iPleurs > pleurs)
//			return 1; //Niet zelfde veld, en al bestaande veld was moeilijker te bepalen. Dus zo laten.

		if(!item.m_Bepaald)	++m_Gevuld;

		item.m_Veld	   = veld;
		item.m_Bepaald = true;
		item.m_iPleurs = pleurs;
		item.m_iDiepte = diepte;
		item.m_Waarde  = waarde;

//		if(isNietZelfdeVeld)
//			return 2;
		return 0;
	}

	void			CacheVoorbereiden(int diepte)
	{
		CCacheItemLijst::iterator itEnd = m_CCacheItemLijst.end();
		for(CCacheItemLijst::iterator it = m_CCacheItemLijst.begin(); it != itEnd; ++it)
		{
			if(!it->m_Bepaald)
				continue;
			if(CZetBedenker::IsWinWaarde(it->m_Waarde))
				continue;
			if(it->m_iDiepte + it->m_Veld.GetAantal() == diepte)
				continue;
			
			*it = CCacheItem();//Leegmaken
			--m_Gevuld;
		}
	}

	inline int		GetPleursDieNietNodigWaren()const {return m_PleursDieNietNodigWaren;}
	inline int		GetVulling()const {return (int)(m_Gevuld * 100.0 / (1 << G_HashBits));}

private:

	typedef std::vector<CCacheItem> CCacheItemLijst;

	CCacheItemLijst m_CCacheItemLijst;
	int				m_Gevuld;
	int				m_PleursDieNietNodigWaren;
};

CVierOpRijCache G_Cache;

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
	m_Aantal(0),
	m_Hash(0)
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

char VierOpRijVeld::VorigeBeurt()
{
	//Let op, werkt alleen bij 2 spelers!
	return VolgendeBeurt();
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

int VierOpRijVeld::UnpleurUnchecked(int plek)
{
	char& hoogte = m_Hoogte[plek];
	if(hoogte <= 0)
		return -1;
	
	VorigeBeurt();
	PlaatsUnchecked(0, plek, --hoogte);
//	m_Beurt ^= 0x3;
	return hoogte;
}

//#define HASH_ADD(_speler, _plek) (((unsigned long long)(_speler) << (((_plek) * 1) % 64)))
//#define HASH_ADD(_speler, _plek) ((_speler) == 1 ? ((((unsigned long long)1) << (unsigned long long)(((_plek) * 1) % 64))) : \
//												   ((((unsigned long long)1) << (unsigned long long)(64 - (((_plek) * 1) % 64)))) )
#define HASH_ADD(_speler, _plek) ((_speler) == 1 ? ((((unsigned long long)1) << (unsigned long long)(((_plek) * 1) % 32))) : \
												   ((((unsigned long long)1) << (unsigned long long)(64 - (((_plek) * 1) % 32)))) )

void VierOpRijVeld::PlaatsUnchecked(int speler, int x, int y)
{
	char& plek = m_Veld[x][y];
	if(speler == 0)
	{
		//Unplaats
		m_SpelerWeegschaal[plek - 1] -= G_Weegschaal.m_Veld[x][y];
		m_Hash -= HASH_ADD(plek, &plek - *m_Veld);
		plek = 0;
		--m_Aantal;
		m_Win = 0;
		return;
	}
	m_Hash += HASH_ADD(speler, &plek - *m_Veld);
	
	plek = speler;
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

bool VierOpRijVeld::IsZelfdeVeld(const VierOpRijVeld& veld) const
{
	if(m_Hash != veld.m_Hash) return false;
	if(memcmp(&m_Veld, &veld.m_Veld, sizeof(m_Veld)) != 0) 
		return false;
	if(m_Beurt != veld.m_Beurt) return false;
	return true;
}

unsigned int VierOpRijVeld::Hash()const
{
	long long werkHash = m_Hash;
	unsigned int resultHash = 0;
	for(int i = 0; i < 63 / CVierOpRijCache::G_HashBits; ++i) //Doet t 1x te weinig
	{
		resultHash += werkHash & CVierOpRijCache::G_HashMasker;
		werkHash >>= CVierOpRijCache::G_HashBits;
	}
	resultHash += werkHash & CVierOpRijCache::G_HashMasker; // & operatie als het goed is hier niet nodig. Maar just for sure.

	return resultHash;
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
					return Sm_PlusMax; //Wint gelijk. Dus hoef niet meer verder te zoeken h�.
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

void CZetBedenker::BepaalVolgorde(VierOpRijVeld& veld, int (& volgorde)[VierOpRijVeld::Sm_Breedte], int zoekDiepte)
{
	//return;
	zoekDiepte -= 8; //8 niveaus minder doorzoeken.
	if(zoekDiepte <= 0)
		return; // Volgorde is niet boeiend.

	PlekScore plekScore[VierOpRijVeld::Sm_Breedte];
	for(int i = 0; i < VierOpRijVeld::Sm_Breedte; ++i)
	{
		int plek = PakVolgordePlek(volgorde, i);
		plekScore[i].plek = plek;
		if(veld.PleurUnchecked(plek) < 0)
		{
			plekScore[i].score = Sm_MinMax;
			continue;
		}
//		if(!G_Cache.GetCacheWaarde(veld,plekScore[i].score,3))
			plekScore[i].score = -Minimax(veld,min(zoekDiepte+1, 3));
//		else
//			plekScore[i].score = -plekScore[i].score;
//		return waarde; //Hee, iets gevonden!

	//Ok, nix. Dan maar even zelf zoeken.

		veld.UnpleurUnchecked(plek);
	}

	std::sort(plekScore, plekScore + VierOpRijVeld::Sm_Breedte, PlekScoreSort);
	for(int i = 0; i < VierOpRijVeld::Sm_Breedte; ++i)
		volgorde[i] = plekScore[i].plek;
}

int CZetBedenker::Minimax(VierOpRijVeld& veld, int zoekDiepte)
{
	if(veld.Win() != 0)	return Sm_MinMax;
	if(veld.Vol())      return 0;

	//Stiekem even in de cache neuzen...
	int waarde = 0;
//	if(G_Cache.GetCacheWaarde(veld,waarde,zoekDiepte))
//		return waarde; //Hee, iets gevonden!

	//Ok, nix. Dan maar even zelf zoeken.
	if(zoekDiepte == 0) return Evalueer(veld);


	waarde = Sm_MinMax;
	for(int i = 0; i < VierOpRijVeld::Sm_Breedte; ++i)
	{
		if(veld.PleurUnchecked(i) < 0)
			continue;
		int nieuweWaarde = -Minimax(veld, zoekDiepte - 1);
		if(nieuweWaarde > waarde)
			waarde = nieuweWaarde;
		veld.UnpleurUnchecked(i);
	}
	return waarde;
}

int CZetBedenker::BepaalScore(VierOpRijVeld& veld, int zoekDiepte, int alpha, int beta, int* pZet)
{
	if(veld.Win() != 0)
	{
		++m_statWins;
//		return Sm_MinMax - zoekDiepte;   //Pff, vorige speler heeft al gewonnen. Lekker eerlijk. (Oja, het is nog erger als de diepte nog hoog is.)
		return Sm_MinMax;
	}
	if(	m_bAbort ||			//Stoppen maar. Gebruiker heeft geen geduld.
		veld.Vol()) //Veld zit vol. Heeft ook niet veel zin meer h�...
		return 0;

//	if(false)
	if(pZet == NULL) //Doe dit alleen als alleen de waarde bepaald moet worden.
	{
		int cacheWaarde;
		if(G_Cache.GetCacheWaarde(veld, cacheWaarde, zoekDiepte))
			return cacheWaarde;//Flop, uit de cache getrokken! Is dat even handig!
	}

	//ff kijke of ik in deze situatie kan winne, zonder eerst dieper te zoeken.
	for(int i = 0; i < VierOpRijVeld::Sm_Breedte; ++i)
	{
		if(veld.PleurUnchecked(i) < 0)
			continue;
		bool win = veld.Win() != 0;
		veld.UnpleurUnchecked(i);
		if(win)
		{
			++m_statWins;
			if(pZet != NULL)
				*pZet = i;
			return Sm_PlusMax; //Ik ga winne!!
		}
	}

	if( zoekDiepte == 0)  //Niet meer verder zoeken (anders duurt 't beetje lang h�)
		return Evalueer(veld);//Eventueel evalueren...

	DiepteProgress& diepteProgress  = m_DiepteProgressLijst[zoekDiepte - 1];
	int pleursBegin = m_statPleurs;
	diepteProgress.beste = -1;
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

	BepaalVolgorde(veld, volgorde, zoekDiepte);

	//VierOpRijVeld veldMetZet(veld);
	for(int i = 0; i < VierOpRijVeld::Sm_Breedte; ++i)
	{
		int plek = PakVolgordePlek(volgorde, i);
		if(veld.PleurUnchecked(plek) < 0)
			continue;//Kan niet hier...
		diepteProgress.volgorde			= i;
		diepteProgress.plek				= plek;

		++m_statPleurs;
		int zetScore = -BepaalScore(veld, zoekDiepte - 1, -beta, -alpha, NULL);
		veld.UnpleurUnchecked(plek);//Zet weer ongedaan maken...

		if(pZet == NULL)
		{
			if(zetScore > alpha)
			{
				alpha = zetScore;
				diepteProgress.beste       = plek;
				diepteProgress.besteWaarde = zetScore;
			}
		}
//			alpha = __max(alpha, zetScore);
		else
		{
			ScoreBepaald(plek, zetScore);
			if(zetScore > alpha)
			{
				diepteProgress.beste       = plek;
				diepteProgress.besteWaarde = zetScore;
				alpha = zetScore;
				*pZet = plek;
			}
			if(zetScore == Sm_PlusMax)
				m_bWinst = true;
		}

		if(beta <= alpha)
			return alpha;
//			break; //Jay! Beta cutoff. Scheelt weer wat tijd.
	}

	G_Cache.SetCacheItem(veld, alpha, zoekDiepte, m_statPleurs - pleursBegin);

	//G_VierOpRijVeldWaardeLijst[veld.m_Hash & G_HashMasker].m_Veld = veld;
	return alpha;
}

int CZetBedenker::BedenkZet(int zoekDiepte)
{
	m_bWinst = false;
	if(zoekDiepte < 0)
		zoekDiepte = max(20, m_Veld.m_Aantal + 13);
	zoekDiepte = min(zoekDiepte, m_Veld.AantalZettenOver());
	m_ZoekDiepte					= zoekDiepte;
	G_Cache.CacheVoorbereiden(m_Veld.GetAantal() + zoekDiepte);
	m_statPleursDieNietHoevenBegin	= G_Cache.GetPleursDieNietNodigWaren();
	m_DiepteProgressLijst.resize(m_ZoekDiepte);
	m_Timestamp_Begin				= clock();
	BepaalScore(m_Veld, zoekDiepte, Sm_MinMax, Sm_PlusMax, &m_Zet);
	return m_Zet;
}

int CZetBedenker::GetPleursDieNietHoeven()const
{
	return G_Cache.GetPleursDieNietNodigWaren() - m_statPleursDieNietHoevenBegin;
}

int CZetBedenker::GetVulling()const
{
	return G_Cache.GetVulling();
}
