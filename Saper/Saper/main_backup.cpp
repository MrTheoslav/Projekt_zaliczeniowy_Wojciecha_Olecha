#include <iostream>	//Wyswietlanie na ekranie
#include <fstream>	//Transport zmiennych do zewnetrznych plikow
#include <conio.h>	//_getch
#include <random>	//Random
#include <windows.h>//Kolorki

using namespace std;
HANDLE hOut;

//Menu

int menu();
void graj();
void opcje();
void instrukcja();
char enter = 13;


//Mechanika
struct Pole
{
	bool mina;
	int ile_min_dookola;
	bool odkryte;
	bool flaga;
	bool gracz;
};
void niestandardowy();
char gora{ 'w' }, dol{ 's' }, lewo{ 'a' }, prawo{ 'd' }, flaga{ 'f' }, odkryj{ 'p' };
int max_szerokosc{}, max_dlugosc{}, ile_min{};
bool gramy_dalej{ true }, czy_mina{ false }, czy_wygrana{ false };
Pole** plansza{};
void losuj_miny();
void wyswietl_plansze();
void ruch();
void lancuchowe_odkrywanie(int x, int y);
void wyjscie_opcje();

//Estetyka
char a{ 'Ą' }, A{ '¤' }, c{ '†' }, C{ 'Ź' }, e{ '©' }, E{ '¨' }, l{ '' }, L{ 'ť' }, n{ 'ä' }, N{ 'ă' }, o{ '˘' }, O{ 'ŕ' }, s{ '' }, S{ '—' }, x{ '«' }, X{ 'Ť' }, z{ 'ľ' }, Z{ '˝' }, sflaga{}, spole{}, smina{};
int main()
{
	fstream sterowanie;
	sterowanie.open("opcje.txt", ios::in | ios::out);
	if (sterowanie.good() == false)
	{
		ofstream sterowanie{ "opcje.txt" };//Tworzy plik
		sterowanie << gora << dol << lewo << prawo << flaga << odkryj;
	}
	else
	{
		sterowanie >> gora >> dol >> lewo >> prawo >> flaga >> odkryj;
	}
	sterowanie.close();
	char exit{};
	do
	{
		switch (menu())
		{
		case 0:
		{
			graj();
			break;
		}
		case 1:
		{
			opcje();
			break;
		}
		case 2:
		{
			instrukcja();
			break;
		}
		default:
		{

			cout << "Czy jeste pewny, ľe chcesz wyj†? [T/N]";
			do
			{
				exit = _getch();
				if (exit == 't' || exit == 'T')
					return 0;
			} while (exit != 'n' && exit != 'N');
			break;
		}
		}
	} while (TRUE);
}

//Menu
int menu()
{
	int kursor{};
	char ruch{};
	do
	{
		system("cls");
		cout << "MENU" << endl << endl;
		if (kursor == 0)
		{
			cout << ">";
			cout.width(sizeof("Graj"));
		}
		else
			cout.width(sizeof("Graj") + 1);
		cout << "Graj" << endl;
		if (kursor == 1)
		{
			cout << ">";
			cout.width(sizeof("Opcje"));
		}
		else
			cout.width(sizeof("Opcje") + 1);
		cout << "Opcje" << endl;
		if (kursor == 2)
		{
			cout << ">";
			cout.width(sizeof("Instrukcja"));
		}
		else
			cout.width(sizeof("Instrukcja") + 1);
		cout << "Instrukcja" << endl;
		if (kursor == 3)
		{
			cout << ">";
			cout.width(sizeof("Wyj"));
		}
		else
			cout.width(sizeof("Wyj") + 1);
		cout << "Wyj" << s << "cie" << endl;
		cout << endl << "Ruch w g" << o << "r" << e << ": " << gora << endl;
		cout << "Ruch w d" << o << l << ": " << dol << endl;
		cout << "Potwierdzenie wyboru: ENTER" << endl;
		ruch = _getch();
		if (ruch == gora)
		{
			kursor--;
			if (kursor < 0)//Pozycja pierwszej opcji
				kursor = 3;//Przechodzi na ostatnia pozycje
		}
		else if (ruch == dol)
		{
			kursor++;
			if (kursor > 3)//Pozycja ostatniej opcji
				kursor = 0;//Przechodzi na piersza pozycje
		}
	} while (ruch != enter);
	system("cls");
	return kursor;
}

void graj()
{
	bool powrot = 0;
	int kursor{};
	char ruch{};
	do
	{
		system("cls");
		cout << "Wybierz poziom trudnoci" << endl << endl;
		if (kursor == 0)
		{
			cout << ">";
			cout.width(10);
		}
		else
			cout.width(11);
		cout << "Kontynuuj" << endl;
		if (kursor == 1)
		{
			cout << ">";
			cout.width(6);
		}
		else
			cout.width(7);
		cout << "ťatwy" << endl;
		if (kursor == 2)
		{
			cout << ">";
			cout.width(7);
		}
		else
			cout.width(8);
		cout << "—redni" << endl;
		if (kursor == 3)
		{
			cout << ">";
			cout.width(7);
		}
		else
			cout.width(8);
		cout << "Trudny" << endl;
		if (kursor == 4)
		{
			cout << ">";
			cout.width(15);
		}
		else
			cout.width(16);
		cout << "Niestandardowy" << endl;
		if (kursor == 5)
		{
			cout << ">";
			cout.width(7);
		}
		else
			cout.width(8);
		cout << "Wstecz" << endl;
		cout << endl << "Ruch w g˘r©: w" << endl;
		cout << "Ruch w d˘: s" << endl;
		cout << "Potwierdzenie wyboru: ENTER" << endl;
		if (kursor == 4)
			cout << "Uwaga! W trybie niestandardowym wyniki nie sĄ zapisywane do tabeli wynik˘w!" << endl;
		ruch = _getch();
		if (ruch == gora)
		{
			kursor--;
			if (kursor < 0)//Pozycja pierwszej opcji
				kursor = 5;//Przechodzi na ostatnia pozycje
		}
		else if (ruch == dol)
		{
			kursor++;
			if (kursor > 5)//Pozycja ostatniej opcji
				kursor = 0;//Przechodzi na piersza pozycje
		}
	} while (ruch != enter);
	system("cls");
	while (!powrot)
	{
		powrot = 0;
		switch (kursor)
		{
		case 0:
		{
			//Odczytanie zapisu gry, jezeli istnieje
			max_dlugosc = 5;
			max_szerokosc = 5;
			ile_min = 5;
			break;
		}
		case 1:
		{
			max_dlugosc = 9;
			max_szerokosc = 9;
			ile_min = 10;
			break;
		}
		case 2:
		{
			max_dlugosc = 16;
			max_szerokosc = 16;
			ile_min = 40;
			break;
		}
		case 3:
		{
			max_dlugosc = 25;
			max_szerokosc = 16;
			ile_min = 60;
			break;
		}
		case 4:
		{
			niestandardowy();
			break;
		}
		case 5:
		{
			powrot = 1;
			break;
		}

		}
		if (powrot != 1)
		{
			losuj_miny();//Losowanie min i przyporzadkowywanie liczb do pol sasiadujacych z minami:
			char potwierdzenie{};
			if (czy_wygrana)
			{
				czy_wygrana = false;
				czy_mina = false;
				main();
			}
			if (!czy_mina)
				main();
			cout << "Czy chcesz zagra jeszcze raz? [T/N]" << endl;
			czy_mina = false;
			czy_wygrana = false;
			do
			{
				potwierdzenie = _getch();
			} while (potwierdzenie != 'T' && potwierdzenie != 't' && potwierdzenie != 'N' && potwierdzenie != 'n');
			system("cls");
			if (potwierdzenie == 'n' || potwierdzenie == 'N')
				main();
			else
			{
				continue;
			}
		}
	}
}

void opcje()
{
	int kursor{};
	char ruch{};
	fstream sterowanie;
	sterowanie.open("opcje.txt", ios::out);
	do
	{
		system("cls");
		cout << "OPCJE" << endl << endl;
		if (kursor == 0)
		{
			cout << ">";
			cout.width(sizeof("Ruch w górę: "));
		}
		else
			cout.width(sizeof("Ruch w górę: ") + 1);
		cout << "Ruch w górę: " << gora << endl;
		if (kursor == 1)
		{
			cout << ">";
			cout.width(sizeof("Ruch w dół: "));
		}
		else
			cout.width(sizeof("Ruch w dół: ") + 1);
		cout << "Ruch w dół: " << dol << endl;
		if (kursor == 2)
		{
			cout << ">";
			cout.width(sizeof("Ruch w lewo: "));
		}
		else
			cout.width(sizeof("Ruch w lewo: ") + 1);
		cout << "Ruch w lewo: " << lewo << endl;
		if (kursor == 3)
		{
			cout << ">";
			cout.width(sizeof("Ruch w prawo: "));
		}
		else
			cout.width(sizeof("Ruch w prawo: ") + 1);
		cout << "Ruch w prawo: " << prawo << endl;
		if (kursor == 4)
		{
			cout << ">";
			cout.width(sizeof("Oflagowanie: "));
		}
		else
			cout.width(sizeof("Oflagowanie: ") + 1);
		cout << "Oflagowanie: " << flaga << endl;
		if (kursor == 5)
		{
			cout << ">";
			cout.width(sizeof("Odkrywanie pola: "));
		}
		else
			cout.width(sizeof("Odkrywanie pola: ") + 1);
		cout << "Odkrywanie pola: " << odkryj << endl;
		if (kursor == 6)
		{
			cout << ">";
			cout.width(sizeof("Wyj"));
		}
		else
			cout.width(sizeof("Wyj") + 1);
		cout << "Wyj" << s << "cie" << endl;
		cout << endl << "Ruch w g" << o << "r" << e << ": " << gora << endl;
		cout << "Ruch w d" << o << l << ": " << dol << endl;
		cout << "Potwierdzenie wyboru: ENTER" << endl;
		ruch = _getch();
		if (ruch == gora)
		{
			kursor--;
			if (kursor < 0)//Pozycja pierwszej opcji
				kursor = 6;//Przechodzi na ostatnia pozycje
		}
		else if (ruch == dol)
		{
			kursor++;
			if (kursor > 6)//Pozycja ostatniej opcji
				kursor = 0;//Przechodzi na piersza pozycje
		}
	} while (ruch != enter);
	sterowanie << gora << dol << lewo << prawo << flaga << odkryj;
	system("cls");
	sterowanie.close();
}

void instrukcja()
{
	cout << "Funkcja instrukcja" << endl;
	cout << "Wciśnij dowolny przycisk, aby wrócić do głównego menu";
	_getch();
}

//Mechanika gry

void niestandardowy()
{
	double baza_szerokosc{}, baza_dlugosc{}, baza_ileMin{};//antydebilny system
	do
	{
		cout << "Podaj szeroko?† planszy (z przedzia" << l << "u od 3 do 25) : ";
		cin >> baza_szerokosc;
		if (baza_szerokosc < 3 || baza_szerokosc>25)
			cout << "Podale nieprawidlowĄ warto?†. Podaj jeszcze raz." << endl;
	} while (baza_szerokosc < 3 || baza_szerokosc>25);
	max_szerokosc = static_cast<int>(baza_szerokosc);
	do
	{
		cout << "Podaj dlugo† planszy (z przedzia" << l << "u od 3 do 25) : ";
		cin >> baza_dlugosc;
		if (baza_dlugosc < 3 || baza_dlugosc>25)
			cout << "Podale nieprawidlowĄ warto?†. Podaj jeszcze raz." << endl;
	} while (baza_dlugosc < 3);
	max_dlugosc = static_cast<int>(baza_dlugosc);
	do
	{
		cout << "Podaj ile min ma by† na planszy (z przedzia" << l << "u od 1 do " << max_dlugosc * max_szerokosc << ") : ";
		cin >> baza_ileMin;
		if (baza_ileMin < 1 || baza_ileMin >= (max_dlugosc * max_szerokosc))
			cout << "Podale nieprawidlowĄ warto?†. Podaj jeszcze raz." << endl;
	} while (baza_ileMin < 1 || baza_ileMin >= (max_dlugosc * max_szerokosc));
	ile_min = static_cast<int>(baza_ileMin);
	system("cls");
}

void losuj_miny()
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	plansza = new Pole * [max_dlugosc] {};
	for (int dlugosc = 0; dlugosc < max_dlugosc; dlugosc++)
		plansza[dlugosc] = new Pole[max_szerokosc]{};
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distr1(0, max_dlugosc - 1);
	uniform_int_distribution<> distr2(0, max_szerokosc - 1);
	int pozostalo_min = ile_min;
	while (pozostalo_min != 0)//losowanie min
	{
		int x = distr1(gen), y = distr2(gen);
		if (!plansza[x][y].mina)
		{
			plansza[x][y].mina = true;
			plansza[x][y].ile_min_dookola = 9;
			pozostalo_min--;
		}
	}
	for (int dlugosc = 0; dlugosc < max_dlugosc; dlugosc++)//Oznaczanie reszty kratek i ich zakrywanie
	{
		for (int szerokosc = 0; szerokosc < max_szerokosc; szerokosc++)
		{
			int licznik{};
			if (!plansza[dlugosc][szerokosc].mina)
			{
				if (dlugosc > 0)
				{
					if (szerokosc > 0)
						if (plansza[dlugosc - 1][szerokosc - 1].mina)
							licznik++;
					if (plansza[dlugosc - 1][szerokosc].mina)
						licznik++;
					if (szerokosc < max_szerokosc - 1)
						if (plansza[dlugosc - 1][szerokosc + 1].mina)
							licznik++;
				}
				if (szerokosc > 0)
					if (plansza[dlugosc][szerokosc - 1].mina)
						licznik++;
				if (szerokosc < max_szerokosc - 1)
					if (plansza[dlugosc][szerokosc + 1].mina)
						licznik++;
				if (dlugosc < max_dlugosc - 1)
				{
					if (szerokosc > 0)
						if (plansza[dlugosc + 1][szerokosc - 1].mina)
							licznik++;
					if (plansza[dlugosc + 1][szerokosc].mina)
						licznik++;
					if (szerokosc < max_szerokosc - 1)
						if (plansza[dlugosc + 1][szerokosc + 1].mina)
							licznik++;
				}
				plansza[dlugosc][szerokosc].ile_min_dookola = licznik;//Zapisanie liczby sasiadujacych min
			}
			plansza[dlugosc][szerokosc].odkryte = false;//chowanie
		}
	}
	//}
	//void wyswietl_plansze
	//{
	int x{ 0 }, y{ 0 }; //pozycja gracza
	plansza[y][x].gracz = TRUE;
	int licznik_pol{}, licznik_flag{};
	char ruch{};
	while (gramy_dalej)
	{
		plansza[y][x].gracz = TRUE;
		licznik_pol = 0;
		for (int dlugosc = 0; dlugosc < max_dlugosc; dlugosc++)//analiza
		{
			for (int szerokosc = 0; szerokosc < max_szerokosc; szerokosc++)
			{
				//Zliczanie pol nieodkrytych, min i pól oflagowanych
				if (!plansza[dlugosc][szerokosc].odkryte)
				{
					licznik_pol++;
				}
				if (plansza[dlugosc][szerokosc].flaga)
				{
					licznik_flag++;
				}
				if (plansza[dlugosc][szerokosc].mina && plansza[dlugosc][szerokosc].odkryte)//Czy zostala odkryta mina (Warunek przegrywajacy)
					czy_mina = true;
			}
		}
		cout << "Ile min pozostalo do oznaczenia: " << ile_min - licznik_flag << endl;
		licznik_flag = 0;
		for (int dlugosc = 0; dlugosc < max_dlugosc; dlugosc++)//wypisywanie
		{
			for (int szerokosc = 0; szerokosc < max_szerokosc; szerokosc++)
			{
				if (czy_mina && !plansza[dlugosc][szerokosc].flaga)
					plansza[dlugosc][szerokosc].odkryte = true;

				if (szerokosc != 0)
					cout.width(4);
				if (plansza[dlugosc][szerokosc].gracz)
					SetConsoleTextAttribute(hOut, FOREGROUND_RED);//Zmiana koloru na czerwony oznacza gracza
				else
					SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);//Powrot do normalnosci
				if (plansza[dlugosc][szerokosc].odkryte)
				{
					if (czy_mina && plansza[dlugosc][szerokosc].flaga && !plansza[dlugosc][szerokosc].mina)
						cout << "X";
					else if (plansza[dlugosc][szerokosc].mina)
					{
						if (czy_mina && plansza[dlugosc][szerokosc].flaga)
							cout << "F";
						else
							cout << "*";
					}
					else
					{
						if (!plansza[dlugosc][szerokosc].gracz)
						{
							switch (plansza[dlugosc][szerokosc].ile_min_dookola)
							{
							case 1:
								SetConsoleTextAttribute(hOut, FOREGROUND_BLUE);
								break;
							case 2:
								SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
								break;
							case 3:
								SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
								break;
							case 4:
								SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_RED);
								break;
							case 5:
								SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
								break;
							case 6:
								SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
								break;
							case 7:
								SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_RED);
								break;
							case 8:
								SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_GREEN);
								break;
							}
						}
						cout << plansza[dlugosc][szerokosc].ile_min_dookola;
						SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);//Powrot do normalnosci
					}
				}
				else
				{
					if (plansza[dlugosc][szerokosc].flaga)
					{
						if (czy_mina && !plansza[dlugosc][szerokosc].mina)
							cout << "f";
						else
							cout << "F";
					}
					else
					{
						cout << "X";
					}
				}
			}
			cout << endl;
		}
		if (licznik_pol == ile_min)//Wygrana
		{
			cout << "Wygrales!!! :)" << endl;
			system("pause");
			czy_wygrana = true;
			break;
		}
		if (czy_mina)//Przegrana
		{
			cout << "Wpadleś w minę :(" << endl;
			system("pause");
			break;
		}
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);//Powrot do normalnosci
		cout << "Ruch w górę: " << gora << endl << "Ruch w dół: " << dol << endl << "Ruch w prawo: " << prawo << endl << "Ruch w lewo: " << lewo << endl << "Oflagowanie: " << flaga << endl << "Odkrywanie pola: " << odkryj << endl << "Wyjście: ESC" << endl;
		//}
		//void ruch()
		//{
		ruch = _getch();
		if (ruch == lewo)
		{
			plansza[y][x].gracz = FALSE;
			x--;
			if (x < 0)
				x += max_szerokosc;
		}
		else if (ruch == prawo)
		{
			plansza[y][x].gracz = FALSE;
			x++;
			if (x >= max_szerokosc)
				x -= max_szerokosc;
		}
		else if (ruch == gora)
		{
			plansza[y][x].gracz = FALSE;
			y--;
			if (y < 0)
				y += max_dlugosc;
		}
		else if (ruch == dol)
		{
			plansza[y][x].gracz = FALSE;
			y++;
			if (y >= max_dlugosc)
				y -= max_dlugosc;
		}
		else if (ruch == flaga)
		{
			if (!plansza[y][x].odkryte)
			{
				if (plansza[y][x].flaga)
					plansza[y][x].flaga = FALSE;
				else
					plansza[y][x].flaga = TRUE;
			}
		}
		else if (ruch == odkryj)
		{
			if (!plansza[y][x].flaga)
			{
				plansza[y][x].odkryte = TRUE;
				if (plansza[y][x].odkryte && plansza[y][x].ile_min_dookola == 0)
					lancuchowe_odkrywanie(y, x);
			}
		}
		else if (ruch == 27)
		{
			gramy_dalej = false;
		}
		system("cls");
	}
	for (int dlugosc = 0; dlugosc < max_dlugosc; dlugosc++)
		delete[] plansza[dlugosc];
	delete[] plansza;
}

void lancuchowe_odkrywanie(int x, int y)
{

	if (!plansza[x][y].flaga && !plansza[x][y].mina)
	{
		plansza[x][y].odkryte = true;
		if (plansza[x][y].ile_min_dookola == 0)
		{
			if (x > 0)
			{
				if (y > 0)
					if (!plansza[x - 1][y - 1].odkryte)
						lancuchowe_odkrywanie(x - 1, y - 1);
				if (!plansza[x - 1][y].odkryte)
					lancuchowe_odkrywanie(x - 1, y);
				if (y < max_szerokosc - 1)
					if (!plansza[x - 1][y + 1].odkryte)
						lancuchowe_odkrywanie(x - 1, y + 1);
			}
			if (y > 0)
				if (!plansza[x][y - 1].odkryte)
					lancuchowe_odkrywanie(x, y - 1);
			if (y < max_szerokosc - 1)
				if (!plansza[x][y + 1].odkryte)
					lancuchowe_odkrywanie(x, y + 1);
			if (x < max_dlugosc - 1)
			{
				if (y > 0)
					if (!plansza[x + 1][y - 1].odkryte)
						lancuchowe_odkrywanie(x + 1, y - 1);
				if (!plansza[x + 1][y].odkryte)
					lancuchowe_odkrywanie(x + 1, y);
				if (y < max_szerokosc - 1)
					if (!plansza[x + 1][y + 1].odkryte)
						lancuchowe_odkrywanie(x + 1, y + 1);
			}
		}
	}
}

void wyjscie_opcje(char gora, char dol, char lewo, char prawo, char flaga, char pole)
{
	//cout << "Funkcja wyjscie_opcje" << endl;
	fstream sterowanie;
	if (sterowanie.good() == false)
		ofstream opcje("opcje.txt");
	sterowanie.open("opcje.txt", ios::out);
	sterowanie << gora << dol << lewo << prawo << flaga << pole;
	sterowanie.close();
	//system("pause");
}