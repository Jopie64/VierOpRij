#include "StdAfx.h"
#include "VierOpRijVeld.h"

VierOpRijVeld::VierOpRijVeld(void)
:	m_Beurt(1),
	m_Win(-1)
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
	if(m_Beurt > 1)
		m_Beurt = 1;
	return huidigeBeurt;
}

void VierOpRijVeld::Pleur(int plek)
{
	if(plek < 0 || plek >= Sm_Breedte)
		throw std::runtime_error("Foute plek l*l");
	if(!PleurUnchecked(plek))
		throw std::runtime_error("Hier zit ie vol...");
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

char VierOpRijVeld::Win()
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

	return -1;
}
