#include <iostream>	//Wyswietlanie na ekranie
#include <fstream>	//Transport zmiennych do zewnetrznych plikow
#include <conio.h>	//_getch
#include <random>	//Random
#include <windows.h>//Kolorki

using namespace std;
HANDLE hOut;

//Menu

void uruchom();
int menu();
void graj();
void opcje();
void instrukcja();

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
char gora{ 'w' }, dol{ 's' }, lewo{ 'a' }, prawo{ 'd' }, oflaguj{ 'f' }, odkryj{ 'p' };
int max_szerokosc{}, max_dlugosc{}, ile_min{}, x_gracza{}, y_gracza{};
bool gramy_dalej{ true }, czy_mina{ false };
Pole** plansza{};
void losuj_miny();
void wyswietl_plansze();
void ruch_gracza(int dostepne_flagi);
void lancuchowe_odkrywanie(int x, int y);
void wyjscie_opcje();

//Estetyka
const char a{ 'Ą' }, c{ '†' }, e{ '©' }, l{ '' }, L{ 'ť' }, n{ 'ä' }, o{ '˘' }, s{ '' }, S{ '—' }, z{ 'ľ' };//Polskie litery
const char enter = 13;

//Poczatek funkcji
int main()
{
	fstream sterowanie;
	sterowanie.open("opcje.txt", ios::in | ios::out);
	if (sterowanie.good() == false)
	{
		ofstream sterowanie{ "opcje.txt" };//Tworzy plik
		sterowanie << gora << dol << lewo << prawo << oflaguj << odkryj;
	}
	else
	{
		sterowanie >> gora >> dol >> lewo >> prawo >> oflaguj >> odkryj;
	}
	sterowanie.close();
	uruchom();
}

void uruchom()
{
	char koniec_programu{};
	do
	{
		int wybierz = menu();
		switch (wybierz)
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
		case 3:
		{

			cout << "Czy jeste" << s << " pewny, " << z << "e chcesz wyj" << s << c << "? [T/N]";
			do
			{
				koniec_programu = _getch();
				if (koniec_programu == 't' || koniec_programu == 'T')
					exit(0);
			} while (koniec_programu != 'n' && koniec_programu != 'N');
			break;
		}
		default:
			break;
		}
	} while (true);

}
//Menu
int menu()
{
	int kursor{};
	char ruch{};
	do
	{
		system("cls");
		cout << "SAPER" << endl << endl;
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
	bool powrot = false;
	int kursor{};
	char ruch{};
	do
	{
		system("cls");
		cout << "Wybierz poziom trudnoci" << endl << endl;
		if (kursor == 0)
		{
			cout << ">";
			cout.width(2);
		}
		else
			cout.width(3);
		cout << L << "atwy" << endl;
		if (kursor == 1)
		{
			cout << ">";
			cout.width(2);
		}
		else
			cout.width(3);
		cout << S << "redni" << endl;
		if (kursor == 2)
		{
			cout << ">";
			cout.width(7);
		}
		else
			cout.width(8);
		cout << "Trudny" << endl;
		if (kursor == 3)
		{
			cout << ">";
			cout.width(15);
		}
		else
			cout.width(16);
		cout << "Niestandardowy" << endl;
		if (kursor == 4)
		{
			cout << ">";
			cout.width(7);
		}
		else
			cout.width(8);
		cout << "Wstecz" << endl;
		cout << endl << "Ruch w g" << o << "r" << e << ": " << gora << endl;
		cout << "Ruch w d" << o << l << ": " << dol << endl;
		cout << "Potwierdzenie wyboru: ENTER" << endl;
		ruch = _getch();
		if (ruch == gora)
		{
			kursor--;
			if (kursor < 0)//Pozycja pierwszej opcji
				kursor = 4;//Przechodzi na ostatnia pozycje
		}
		else if (ruch == dol)
		{
			kursor++;
			if (kursor > 4)//Pozycja ostatniej opcji
				kursor = 0;//Przechodzi na piersza pozycje
		}
	} while (ruch != enter);
	system("cls");
	while (!powrot)
	{
		switch (kursor)
		{
		case 0:
		{
			max_dlugosc = 9;
			max_szerokosc = 9;
			ile_min = 10;
			break;
		}
		case 1:
		{
			max_dlugosc = 16;
			max_szerokosc = 16;
			ile_min = 40;
			break;
		}
		case 2:
		{
			max_dlugosc = 25;
			max_szerokosc = 16;
			ile_min = 60;
			break;
		}
		case 3:
		{
			niestandardowy();
			break;
		}
		case 4:
		{
			powrot = true;
			break;
		}
		default:
			break;
		}
		if (powrot == false)
		{
			losuj_miny();//Losowanie min i przyporzadkowywanie liczb do pol sasiadujacych z minami:
			char potwierdzenie{};
			cout << "Czy chcesz zagra" << c << " jeszcze raz? [T/N]" << endl;
			czy_mina = false;
			gramy_dalej = true;
			do
			{
				potwierdzenie = _getch();
			} while (potwierdzenie != 'T' && potwierdzenie != 't' && potwierdzenie != 'N' && potwierdzenie != 'n');
			system("cls");
			if (potwierdzenie == 'n' || potwierdzenie == 'N')
			{
				powrot = true;
				uruchom();
			}
		}
	}
}

void opcje()
{
	int kursor{};
	char ruch{};
	bool wyjscie_z_opcji{};
	char zmiana_znaku{};
	fstream sterowanie;
	sterowanie.open("opcje.txt", ios::out);
	do
	{

		do
		{
			system("cls");
			cout << "OPCJE\n\n";
			if (kursor == 0)
			{
				cout << ">";
				cout.width(sizeof("Ruch w g"));
			}
			else
				cout.width(sizeof("Ruch w g") + 1);
			cout << "Ruch w g" << o << "r" << e << ": " << static_cast<char>(gora) << endl;
			if (kursor == 1)
			{
				cout << ">";
				cout.width(sizeof("Ruch w d"));
			}
			else
				cout.width(sizeof("Ruch w d") + 1);
			cout << "Ruch w d" << o << l << ": " << dol << endl;
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
			cout << "Oflagowanie: " << oflaguj << endl;
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
		system("cls");
		switch (kursor)
		{

		case 0:
		{
			cout << "Podaj znak: ";
			do
			{
				zmiana_znaku = _getch();
				system("cls");
				if (dol == zmiana_znaku || lewo == zmiana_znaku || prawo == zmiana_znaku || oflaguj == zmiana_znaku || odkryj == zmiana_znaku)
					cout << "Podany znak ju" << z << " jest w u" << z << "yciu. Prosz" << e << " Podaj inny znak.";
			} while (dol == zmiana_znaku || lewo == zmiana_znaku || prawo == zmiana_znaku || oflaguj == zmiana_znaku || odkryj == zmiana_znaku);
			gora = zmiana_znaku;
			break;
		}
		case 1:
		{
			cout << "Podaj znak: ";
			do
			{
				zmiana_znaku = _getch();
				system("cls");
				if (gora == zmiana_znaku || lewo == zmiana_znaku || prawo == zmiana_znaku || oflaguj == zmiana_znaku || odkryj == zmiana_znaku)
					cout << "Podany znak ju" << z << " jest w u" << z << "yciu. Prosz" << e << " Podaj inny znak.";
			} while (gora == zmiana_znaku || lewo == zmiana_znaku || prawo == zmiana_znaku || oflaguj == zmiana_znaku || odkryj == zmiana_znaku);
			dol = zmiana_znaku;
			break;
		}
		case 2:
		{
			cout << "Podaj znak: ";
			do
			{
				zmiana_znaku = _getch();
				system("cls");
				if (gora == zmiana_znaku || dol == zmiana_znaku || prawo == zmiana_znaku || oflaguj == zmiana_znaku || odkryj == zmiana_znaku)
					cout << "Podany znak ju" << z << " jest w u" << z << "yciu. Prosz" << e << " Podaj inny znak.";
			} while (gora == zmiana_znaku || dol == zmiana_znaku || prawo == zmiana_znaku || oflaguj == zmiana_znaku || odkryj == zmiana_znaku);
			lewo = zmiana_znaku;
			break;
		}
		case 3:
		{
			cout << "Podaj znak: ";
			do
			{
				zmiana_znaku = _getch();
				system("cls");
				if (gora == zmiana_znaku || dol == zmiana_znaku || lewo == zmiana_znaku || oflaguj == zmiana_znaku || odkryj == zmiana_znaku)
					cout << "Podany znak ju" << z << " jest w u" << z << "yciu. Prosz" << e << " Podaj inny znak.";
			} while (gora == zmiana_znaku || dol == zmiana_znaku || lewo == zmiana_znaku || oflaguj == zmiana_znaku || odkryj == zmiana_znaku);
			prawo = zmiana_znaku;
			break;
		}
		case 4:
		{
			cout << "Podaj znak: ";
			do
			{
				zmiana_znaku = _getch();
				system("cls");
				if (gora == zmiana_znaku || dol == zmiana_znaku || lewo == zmiana_znaku || prawo == zmiana_znaku || odkryj == zmiana_znaku)
					cout << "Podany znak ju" << z << " jest w u" << z << "yciu. Prosz" << e << " Podaj inny znak.";
			} while (gora == zmiana_znaku || dol == zmiana_znaku || lewo == zmiana_znaku || prawo == zmiana_znaku || odkryj == zmiana_znaku);
			oflaguj = zmiana_znaku;
			break;
		}
		case 5:
		{
			cout << "Podaj znak: ";
			do
			{
				zmiana_znaku = _getch();
				system("cls");
				if (gora == zmiana_znaku || dol == zmiana_znaku || lewo == zmiana_znaku || prawo == zmiana_znaku || oflaguj == zmiana_znaku)
					cout << "Podany znak ju" << z << " jest w u" << z << "yciu. Prosz" << e << " Podaj inny znak.";
			} while (gora == zmiana_znaku || dol == zmiana_znaku || lewo == zmiana_znaku || prawo == zmiana_znaku || oflaguj == zmiana_znaku);
			odkryj = zmiana_znaku;
			break;
		}
		case 6:
		{
			wyjscie_z_opcji = 1;
			break;
		}
		default:
		{
			break;
		}
		}
	} while (!wyjscie_z_opcji);
	wyjscie_opcje();
}

void instrukcja()
{
	cout << "Instrukcja gry w sapera:\n";
	cout << "\nSaper to jest gra logiczna polegaj" << a << "ca na odkryciu wszystkich p" << o << "l nie zawieraj" << a << "cych min.\n";
	cout << "Czerwony punkt na planszy oznacza gracza, kt" << o << "ry mo" << z << "e poruszac si" << e << " przy u" << z << "yciu klawiszy,\n";
	cout << "kt" << o << "re mo" << z << "na zmieni" << c << " w opcjach. Obecnie to s" << a << ":\n";
	cout << gora << " - ruch w g" << o << "r" << e << ",\n" << dol << " - ruch w d" << o << l << "," << endl << lewo << " - ruch w lewo,\n" << prawo << " - ruch w prawo.\n";
	cout << "Znak X oznacza pole nieodkryte. Mo" << z << "na je odkry" << c << " staj" << a << "c na polu i klikaj" << a << "c przycisk '" << odkryj << "'.\n";
	cout << "Mo" << z << "na oznacza" << c << " pola, pod kt" << o << "rymi podejrzewa si" << e << ", " << z << "e jest mina. Tak oznaczone pole nie moze by" << c << " odkryte,\n";
	cout << "dop" << o << "ki si" << e << " jego nie odoznaczy. Symbol oznaczenia pola to 'F'.\n";
	cout << "Aby 'oflagowa" << c << "' pole, nale" << z << "y stan" << a << c << " na polu i nacisn" << a << c << " przycisk '" << oflaguj << "'.\n";
	cout << "Liczby oznaczaj" << a << ", ile min znajduje si" << e << " w s" << a << "siedztwie. Mog" << "a znajdywac si" << e << " w linii poziomej, pionowej albo uko" << s << "nej.\n";
	cout << "Aby wygra" << c << " nale" << z << "y odkryc wszystkie pola nieb" << e << "d" << a << "ce minami\n";
	cout << "Dost" << e << "pne s" << a << " cztery poziomy trudno" << s << "ci:\n";
	cout << L << "atwy - pole 9x9, zawiera 10 min.\n";
	cout << S << "redni - pole 16x16, zawiera 40 min.\n";
	cout << "Trudny - pole 25x16, zawiera 60 min.\n";
	cout << "Niestandardowy - rozmiar i liczba min mo" << z << "e zosta" << c << " ustalona przez u" << z << "ytkownika.\n";
	cout << "\nMi" << l << "ego grania i powodzenia w rozbrajaniu planszy!\n";
	cout << "\nWci" << s << "nij dowolny przycisk, aby wr" << o << "ci" << c << " do g" << l << o << "wnego menu";
	_getch();
}

void wyjscie_opcje()
{
	fstream sterowanie;
	sterowanie.open("opcje.txt", ios::out);
	if (!sterowanie.good())
		ofstream opcje("opcje.txt");
	sterowanie << gora << dol << lewo << prawo << oflaguj << odkryj;
	sterowanie.close();
}//zapisywanie do pliku zewnetrznego obecnego sterowania

//Mechanika gry

void niestandardowy()
{
	double baza_szerokosc{}, baza_dlugosc{}, baza_ileMin{};//antydebilny system
	do
	{
		cout << "Podaj szeroko" << s << c << " planszy(z przedzia" << l << "u od 3 do 25) : ";
		cin >> baza_szerokosc;
		if (baza_szerokosc < 3 || baza_szerokosc>25)
			cout << "Podale" << s << " nieprawidlow" << a << " warto" << s << c << ". Podaj jeszcze raz." << endl;
	} while (baza_szerokosc < 3 || baza_szerokosc>25);
	max_szerokosc = static_cast<int>(baza_szerokosc);
	do
	{
		cout << "Podaj dlugo" << s << c << " planszy (z przedzia" << l << "u od 3 do 25) : ";
		cin >> baza_dlugosc;
		if (baza_dlugosc < 3 || baza_dlugosc>25)
			cout << "Podale" << s << " nieprawidlow" << a << " warto" << s << c << ". Podaj jeszcze raz." << endl;
	} while (baza_dlugosc < 3);
	max_dlugosc = static_cast<int>(baza_dlugosc);
	do
	{
		cout << "Podaj ile min ma by" << c << " na planszy (z przedzia" << l << "u od 1 do " << max_dlugosc * max_szerokosc << ") : ";
		cin >> baza_ileMin;
		if (baza_ileMin < 1 || baza_ileMin >= (max_dlugosc * max_szerokosc))
			cout << "Podale" << s << " nieprawidlow" << a << " warto" << s << c << ". Podaj jeszcze raz." << endl;
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
			plansza[dlugosc][szerokosc].odkryte = false;//Chowanie
		}
	}
	x_gracza = 0;//Poczatkowe wspolrzedne gracza
	y_gracza = 0;
	wyswietl_plansze();
}

void wyswietl_plansze()
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	int licznik_pol{}, licznik_flag{};
	while (gramy_dalej)
	{
		plansza[y_gracza][x_gracza].gracz = true;
		licznik_flag = 0;
		licznik_pol = 0;
		for (int dlugosc = 0; dlugosc < max_dlugosc; dlugosc++)//analiza planszy
		{
			for (int szerokosc = 0; szerokosc < max_szerokosc; szerokosc++)
			{
				//Zliczanie pol nieodkrytych pól oflagowanych
				if (!plansza[dlugosc][szerokosc].odkryte)
				{
					licznik_pol++;
				}
				if (plansza[dlugosc][szerokosc].flaga)
				{
					licznik_flag++;
				}
				if (plansza[dlugosc][szerokosc].mina && plansza[dlugosc][szerokosc].odkryte)//Czy zostala odkryta mina (Warunek przegrywajacy)
				{
					czy_mina = true;
					gramy_dalej = false;
				}
			}
		}
		cout << "Pozosta" << l << "o flag: " << ile_min - licznik_flag << endl;
		for (int dlugosc = 0; dlugosc < max_dlugosc; dlugosc++)//Wypisywanie
		{
			for (int szerokosc = 0; szerokosc < max_szerokosc; szerokosc++)
			{
				if (czy_mina && !plansza[dlugosc][szerokosc].flaga)//Odkrycie wszystkich pol przy warunku przegrywajacym
					plansza[dlugosc][szerokosc].odkryte = true;

				if (szerokosc != 0)
					cout.width(4);
				if (plansza[dlugosc][szerokosc].gracz)
					SetConsoleTextAttribute(hOut, FOREGROUND_RED);//Zmiana koloru na czerwony oznacza gracza
				else
					SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);//Powrot do normalnosci
				if (plansza[dlugosc][szerokosc].odkryte)//Pola odkryte
				{
					if (czy_mina && plansza[dlugosc][szerokosc].flaga && !plansza[dlugosc][szerokosc].mina)//pola oflagowane niepoprawnie
						cout << "x";
					else if (plansza[dlugosc][szerokosc].mina)
					{
						if (czy_mina && plansza[dlugosc][szerokosc].flaga)//Flaga
							cout << "F";
						else//Mina
							cout << "*";
					}
					else
					{
						if (!plansza[dlugosc][szerokosc].gracz)//kolorowanie liczb, na ktorych nie stoi gracz
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
				else//Pola nieodkryte
				{
					if (plansza[dlugosc][szerokosc].flaga)//Pola oflagowane
					{
						cout << "F";
					}
					else//Pola nieodkryte i nieoflagowane
					{
						cout << "X";
					}
				}
			}
			cout << endl;
		}
		if (licznik_pol == ile_min && !czy_mina)//Wygrana
		{
			cout << "Wygra" << l << "e" << s << " !!!:)\nWci" << s << "nij dowolny przycisk, aby kontynuowa" << c;
			gramy_dalej = false;
			_getch();//czeka na akcje gracza
		}
		if (czy_mina)//Przegrana
		{
			cout << "Wpadle" << s << " w minę :(";
			cout << endl << "Legenda oznacze" << n << ":\nF - flaga postawiona na minie\nx - flaga niepostawiona na minie\n* - mina\nWci" << s << "nij dowolny przycisk, aby kontynuowa" << c;
			_getch();//czeka na akcje gracza
		}
		if (gramy_dalej)
		{
			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);//Powrot do normalnosci
			cout << "Ruch w g" << o << "r" << e << " : " << gora << endl;
			cout << "Ruch w d" << o << l << ": " << dol << endl;
			cout << "Ruch w prawo: " << prawo << endl;
			cout << "Ruch w lewo: " << lewo << endl;
			cout << "Oflagowanie: " << oflaguj << endl;
			cout << "Odkrywanie pola: " << odkryj << endl;
			cout << "Wyj" << s << "cie: ESC" << endl;
			ruch_gracza(ile_min - licznik_flag);
		}
		else
		{
			for (int dlugosc = 0; dlugosc < max_dlugosc; dlugosc++)
				delete[] plansza[dlugosc];
			delete[] plansza;
		}
		system("cls");
	}
}

void ruch_gracza(int dostepne_flagi)
{
	char ruch = _getch();
	if (ruch == lewo)
	{
		plansza[y_gracza][x_gracza].gracz = false;
		x_gracza--;
		if (x_gracza < 0)
			x_gracza += max_szerokosc;
	}
	else if (ruch == prawo)
	{
		plansza[y_gracza][x_gracza].gracz = false;
		x_gracza++;
		if (x_gracza >= max_szerokosc)
			x_gracza -= max_szerokosc;
	}
	else if (ruch == gora)
	{
		plansza[y_gracza][x_gracza].gracz = false;
		y_gracza--;
		if (y_gracza < 0)
			y_gracza += max_dlugosc;
	}
	else if (ruch == dol)
	{
		plansza[y_gracza][x_gracza].gracz = false;
		y_gracza++;
		if (y_gracza >= max_dlugosc)
			y_gracza -= max_dlugosc;
	}
	else if (ruch == oflaguj)
	{
		if (!plansza[y_gracza][x_gracza].odkryte)
		{
			if (plansza[y_gracza][x_gracza].flaga)
				plansza[y_gracza][x_gracza].flaga = false;
			else
				if (dostepne_flagi > 0)
					plansza[y_gracza][x_gracza].flaga = true;
		}
	}
	else if (ruch == odkryj)
	{
		if (!plansza[y_gracza][x_gracza].flaga)
		{
			plansza[y_gracza][x_gracza].odkryte = true;
			if (plansza[y_gracza][x_gracza].odkryte && plansza[y_gracza][x_gracza].ile_min_dookola == 0)
				lancuchowe_odkrywanie(x_gracza, y_gracza);
		}
	}
	if (ruch == 27)
	{
		gramy_dalej = false;
	}
}

void lancuchowe_odkrywanie(int x, int y)
{

	if (!plansza[y][x].flaga && !plansza[y][x].mina)
	{
		plansza[y][x].odkryte = true;
		if (plansza[y][x].ile_min_dookola == 0)
		{
			if (y > 0)
			{
				if (x > 0)
					if (!plansza[y - 1][x - 1].odkryte)
						lancuchowe_odkrywanie(x - 1, y - 1);
				if (!plansza[y - 1][x].odkryte)
					lancuchowe_odkrywanie(x, y - 1);
				if (x < max_szerokosc - 1)
					if (!plansza[y - 1][x + 1].odkryte)
						lancuchowe_odkrywanie(x + 1, y - 1);
			}
			if (x > 0)
				if (!plansza[y][x - 1].odkryte)
					lancuchowe_odkrywanie(x - 1, y);
			if (x < max_szerokosc - 1)
				if (!plansza[y][x + 1].odkryte)
					lancuchowe_odkrywanie(x + 1, y);
			if (y < max_dlugosc - 1)
			{
				if (x > 0)
					if (!plansza[y + 1][x - 1].odkryte)
						lancuchowe_odkrywanie(x - 1, y + 1);
				if (!plansza[y + 1][x].odkryte)
					lancuchowe_odkrywanie(x, y + 1);
				if (x < max_szerokosc - 1)
					if (!plansza[y + 1][x + 1].odkryte)
						lancuchowe_odkrywanie(x + 1, y + 1);
			}
		}
	}
}