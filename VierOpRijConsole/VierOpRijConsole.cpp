// VierOpRijConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\VierOpRijVeld.h"
#include <list>

using namespace std;

enum Color { DBLUE=1,GREEN,GREY,DRED,DPURP,BROWN,LGREY,DGREY,BLUE,LIMEG,TEAL,RED,PURPLE,YELLOW,WHITE,B_B };

void SetKleur(Color P_Kleur)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), P_Kleur);
}

void ToonVeld(const VierOpRijVeld& veld)
{
	for(int y = VierOpRijVeld::Sm_Hoogte - 1; y >= 0; --y)
	{
		for(int x = 0; x < VierOpRijVeld::Sm_Breedte; ++x)
		{
			char speler = veld.Wie(x, y);
			switch(speler)
			{
			case 0: SetKleur(WHITE);   cout << ".. "; break;
			case 1: SetKleur(RED);    cout << "oo "; break;
			case 2: SetKleur(YELLOW); cout << "xx "; break;
			}			
		}
		cout << endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	list<VierOpRijVeld> veldStapel;
	veldStapel.push_back(VierOpRijVeld());

	string invoer;
	char invoerChar;
	do
	{
		try
		{
			VierOpRijVeld veld = veldStapel.back();
			ToonVeld(veld);
			SetKleur(WHITE);


			cout << endl << "Invoer aub: ";
			cin >> invoer;
			if(invoer.empty())
				continue;

			invoerChar = tolower(invoer[0]);
			switch(invoerChar)
			{
			case 'b': 
				{
					int level = atoi((&*invoer.begin())+1);
					if(level == 0)
						level = 16;
					cout << "Denk denk... level " << level << "..." << endl;
					CZetBedenker bedenker(veld);
					int zet = bedenker.BedenkZet(level);
					if(zet < 0)
						cout << "Ik heb geen idee... Doe maar iets." << endl;
					else
					{
						cout << "Ik doe 'm op " << zet + 1 << "." << endl;
						veld.Pleur(zet);
						veldStapel.push_back(veld);
					}
				}
				break;
			case 'q': break;
			case 'r': veldStapel.clear(); veldStapel.push_back(VierOpRijVeld()); break;
			case 'u': if(veldStapel.size() <= 1)throw std::runtime_error("Wat moet er nu nog weg dan?"); veldStapel.pop_back(); break;
			default:
				if(invoerChar >= '0' && invoerChar <= '9')
				{
					invoerChar -= '1';
					veld.Pleur(invoerChar);
					veldStapel.push_back(veld);
				}
				else
					throw std::runtime_error("Wat is dit? " + invoer);
			}
		}
		catch(std::exception& e)
		{
			SetKleur(RED);
			cout << "*** " << e.what() << endl;
		}
	}while(invoerChar != 'q');

	return 0;
}

