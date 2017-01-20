#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_image.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_primitives.h>
#include <iostream>
#include <cmath>

enum wsad { W, S, A, D };
enum RuchOrka { Gora, Dol, Lewo, Prawo, LewoGora, LewoDol, PrawoGora, PrawoDol };
RuchOrka PozycjaOrka[8] = { Gora,Gora,Gora,Gora,Gora,Gora,Gora,Gora };
enum przeciwnik { PRZECIWNIK };
enum pocisk { POCISK };
enum szaman { SZAMAN };
enum pocisk_szamana {POCISKSZAMANA};

struct Ork
{
	int ID;
	float x;
	float y;
	bool live;
	float predkosc;
	int boundx;
	int boundy;
	int a;
	int ktoryX_ork;
	int ktoryY_ork;
	int hp;
	bool efekt_pocisku_sopel;
	bool efekt_pocisku_trucizna;
	int pomocnicza_do_efektu;
	bool efekt_krwi;
	int pomocnicza_do_krwi;
	ALLEGRO_BITMAP *OrkSprite[51];
};
struct PociskSzamana
{
	int ID;
	float x;
	float y;
	bool live;
	float predkosc;
	int ktoryX_pocisk_szamana;
	int ktoryY_pocisk_szamana;
	float odleglosc_x;  // a (postac i kursor)
	float odleglosc_y;  // b (postac i kursor)
	float odleglosc_xy; // c (postac i kursor)
	float sinus;        // sinus pomiedzy postacia a kursorem
	float cosinus;      // cosinus pomiedzy postacia a kursorem
	int kierunek;
	int rodzaj_pocisku;
	ALLEGRO_BITMAP *pocisk_szamana[12];
	bool czy_moze_strzelic_szaman;
};

struct Szaman
{
	int ID;
	float x;
	float y;
	bool live;
	float predkosc;
	int boundx;
	int boundy;
	int a;
	int ktoryX_szaman;
	int ktoryY_szaman;
	int hp;
	bool efekt_pocisku_sopel;
	bool efekt_pocisku_trucizna;
	int pomocnicza_do_efektu;
	bool efekt_krwi;
	int pomocnicza_do_krwi;
	ALLEGRO_BITMAP *SzamanSprite[51];
	int PozycjaSzamana;  // 1 - prawo, 2 - lewo, 3 - dol, 4 - gora, 5 - lewogora, 6 - lewodol, 7 - prawogora, 8 - prawodol
	float odleglosc_x;  // a (postac i szaman)
	float odleglosc_y;  // b (postac i szaman)
	float odleglosc_xy; // c (postac i szaman)
	float sinus;        // sinus pomiedzy postacia a szamanem
	float cosinus;      // cosinus pomiedzy postacia a szamanem
	int kierunek;
	int pomocnicza_do_strzalu_szamana;
	PociskSzamana pociski_szamana;
};

struct Pocisk
{
	int ID;
	float x;
	float y;
	bool live;
	float predkosc;
	int ktoryX_pocisk;
	int ktoryY_pocisk;
	float odleglosc_x;  // a (postac i kursor)
	float odleglosc_y;  // b (postac i kursor)
	float odleglosc_xy; // c (postac i kursor)
	float sinus;        // sinus pomiedzy postacia a kursorem
	float cosinus;      // cosinus pomiedzy postacia a kursorem
	int kierunek;
	int rodzaj_pocisku;
	ALLEGRO_BITMAP *pocisk[36];
};

// dmg strzalek
float fireball_dmg = 25;
float sopel_dmg = 20;
float trucizna_dmg = 15;

// losowanie
int losowanie_orka=500;
int losowanie_szamana=100;

// wczytanie czcionki
ALLEGRO_FONT *font24 = al_load_ttf_font("centuary.ttf", 24, 0);

// siatka
void KtoryXY(float pos_x, float pos_y, float mapa_x, float mapa_y, int & ktoryX, int & ktoryY);

// ruch postaci z kolizja
void RuchPostaciW(float pos_x, float & pos_y, int ktoryX, int ktoryY, float predkosc_postaci);
void RuchPostaciS(float pos_x, float & pos_y, int ktoryX, int ktoryY, float predkosc_postaci);
void RuchPostaciA(float & pos_x, float pos_y, int ktoryX, int ktoryY, float predkosc_postaci);
void RuchPostaciD(float & pos_x, float pos_y, int ktoryX, int ktoryY, float predkosc_postaci);

// orkowie
void RuchOrkaW(float pos_x, float & pos_y, int ktoryX, int ktoryY, float predkosc_postaci);
void RuchOrkaS(float pos_x, float & pos_y, int ktoryX, int ktoryY, float predkosc_postaci);
void RuchOrkaA(float & pos_x, float pos_y, int ktoryX, int ktoryY, float predkosc_postaci);
void RuchOrkaD(float & pos_x, float pos_y, int ktoryX, int ktoryY, float predkosc_postaci);
void InitOrk(Ork orkowie[], int ile);
void DrawOrk(Ork orkowie[], int ile, int pomocnicza_do_sprite_orka);
void StartOrk(Ork orkowie[], int ile);
void RuchOrk(Ork orkowie[], int ile);
void Sciezka(float pos_x, float pos_y, float &  przeciwnik_x, float & przeciwnik_y, float predkosc_orka, int ktoryX_ork, int ktoryY_ork, int ile);

//szamani
void InitSzaman(Szaman szamani[], int ile);
void DrawSzaman(Szaman szamani[], int ile, int pomocnicza_do_sprite_szamana);
void StartSzaman(Szaman szamani[], int ile);
void RuchSzaman(Szaman szamani[], int ile);
void SciezkaSzamana(float pos_x, float pos_y, float &  przeciwnik_x, float & przeciwnik_y, float predkosc_szamana, int ile, int & PozycjaSzamana);

// pociski
void InitPocisk(Pocisk pociski[], int ile);
void DrawPocisk(Pocisk pociski[], int ile);
void StartPocisk(Pocisk pociski[], int ile, float pos_x, float pos_y);
void RuchPocisk(Pocisk pociski[], int ile);

// pociski szamana
void InitPociskSzamana(int ile,Szaman szamani[]);
void DrawPociskSzamana( int ile, Szaman szamani[]);
void StartPociskSzamana( int ile, Szaman szamani[]);
void RuchPociskSzamana( int ile, Szaman szamani[]);

// okno 
int szerokosc = 1250;
int wysokosc = 700;

// siatka
bool czy_mozna_wejsc[30][30];
int SiatkaX[30];
int SiatkaY[30];
int ktoryX = 14;
int ktoryY = 14;

// mapa
float mapa_x = -500;
float mapa_y = -500;
float predkosc_mapy = 2;

// postac
float pos_x = (szerokosc / 2) - 125; //500
float pos_y = (wysokosc / 2) - 125; //225
float predkosc_postaci = 2;
float pos_hp = 100;

// orkowie
int ilosc_orkow = 10;
int pomocnicza_do_sprite_orka = 0;

// szamani
int ilosc_szamanow = 5;
int pomocnicza_do_sprite_szamana = 0;

// pociski
const int ilosc_pociskow = 5; // ktore moga istniec jednoczesnie
int RodzajPocisku=1; // 1-Fireball ,  2-Sopel , 3-Trucizna
int ilosc_fireball=10000000;
int ilosc_sopel=10;
int ilosc_trucizna=10;

// kursor
float kursor_x;
float kursor_y;

// parametry postaci
float pieniadze=100;
float doswiadczenie;
int level = 1;
int level_up = 0;
float score=0;

// pomocnicza do timera
int pomocnicza_do_straty_hp = 0;
int pomocnicza_do_konca_gry_przegrana;
int pomocnicza_do_konca_gry_wygrana;
int pomocnicza_do_straty_hp2;

// zegar
int zegar;
float czas = 0;

// rest orka w tym samym miejscu
float resp_prawo_gora_x = 0;
float resp_prawo_gora_y = 0;

// faza gry
int faza_gry = 1; // 1 - menu glowne, 2 - jak grac, 3 - gra , 4 - sklep, 5 - wynik

// 0 trawa 1 wieza 2 domek 3,4,5,6 hall 7 krzak 8 suchedrzewo 9 drzewo 10 woda 11 most
int mapa[] =
{
	9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
	9, 3, 4, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 3, 4, 8, 8,
	7, 5, 6, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 6, 8, 8,
	9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8,
	9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8,
	10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9,
	9, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 9, 9,
	9, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9,
	9, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8,
	9, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 11, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
	7, 10, 0, 0, 9, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
	7, 10, 0, 0, 9, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 7, 7,
	9, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 7, 7,
	10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0  , 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 7, 7,
	10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 7, 7, 7,
	9, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 7, 7, 7,
	9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 7, 7, 9,
	7, 7, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
	7, 7, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
	9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
	9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	2, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 2,
	2, 0, 5, 6, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 2,
	9, 2, 0, 0, 0, 0, 8, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 8, 0, 0, 0, 0, 2,
	9, 9, 9, 9, 9, 9, 9, 9, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 9, 9, 9, 9, 2, 2, 2,
};

int main(void) {
	// timer
	const float FPS = 60.0;
	int ilosc_klatek = 0;


	//przeciwnicy
	Ork orkowie[50];
	Szaman szamani[50];
	// pociski
	Pocisk pociski[ilosc_pociskow];
	// pomocnicza do sprite postaci
	char ostatnia_pozycja;

	//boole
	bool wsad[4] = { false,false,false,false};
	bool redraw = true;
	bool done = false;

	// przewijanie mapy
	int szerokosc_mapy = 30;  // ilosc w jednej linii kolumn 50x50
	int rozmiar_mapy = 900;   // lacznie ile jest kolumn 50x50
	int rozmiar_pola = 50;

	// Siatka X i Siatka Y
	for (int i = 0; i < 30; i++)
	{
		SiatkaX[i] = 50 * i;
		SiatkaY[i] = 50 * i;
	}
	int pom = 0;
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			if (mapa[pom] != 0)
			{
				czy_mozna_wejsc[j][i] = false;
			}
			else
			{
				czy_mozna_wejsc[j][i] = true;
			}
			pom++;
		}
	}



	// zmienne allegro
	ALLEGRO_DISPLAY *okno = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *bgImage = NULL;

	// wlaczenie bilbiotek allegro
	al_init();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();

	if (!al_init())
		return -1;

	// timer
	timer = al_create_timer(1.0 / FPS);
	srand(time(NULL));

	// orkowie
	InitOrk(orkowie, 50);

	//szamani
	InitSzaman(szamani, 50);
	InitPociskSzamana(ilosc_szamanow,szamani);
	// pociski
	InitPocisk(pociski, ilosc_pociskow);

	// bitmapy postaci
	ALLEGRO_BITMAP *Postac[48];
	Postac[0] = al_load_bitmap("Postac/Postac w dol.png");
	Postac[1] = al_load_bitmap("Postac/Postac w dol 1.png");
	Postac[2] = al_load_bitmap("Postac/Postac w dol 2.png");
	Postac[3] = al_load_bitmap("Postac/Postac w dol.png");
	Postac[4] = al_load_bitmap("Postac/Postac w dol 3.png");
	Postac[5] = al_load_bitmap("Postac/Postac w dol 4.png");

	Postac[6] = al_load_bitmap("Postac/Postac w gore.png");
	Postac[7] = al_load_bitmap("Postac/Postac w gore 1.png");
	Postac[8] = al_load_bitmap("Postac/Postac w gore 2.png");
	Postac[9] = al_load_bitmap("Postac/Postac w gore.png");
	Postac[10] = al_load_bitmap("Postac/Postac w gore 3.png");
	Postac[11] = al_load_bitmap("Postac/Postac w gore 4.png");

	Postac[12] = al_load_bitmap("Postac/Postac w prawo.png");
	Postac[13] = al_load_bitmap("Postac/Postac w prawo 1.png");
	Postac[14] = al_load_bitmap("Postac/Postac w prawo 2.png");
	Postac[15] = al_load_bitmap("Postac/Postac w prawo.png");
	Postac[16] = al_load_bitmap("Postac/Postac w prawo 3.png");
	Postac[17] = al_load_bitmap("Postac/Postac w prawo 4.png");

	Postac[18] = al_load_bitmap("Postac/Postac w lewo.png");
	Postac[19] = al_load_bitmap("Postac/Postac w lewo 1.png");
	Postac[20] = al_load_bitmap("Postac/Postac w lewo 2.png");
	Postac[21] = al_load_bitmap("Postac/Postac w lewo.png");
	Postac[22] = al_load_bitmap("Postac/Postac w lewo 3.png");
	Postac[23] = al_load_bitmap("Postac/Postac w lewo 4.png");

	Postac[24] = al_load_bitmap("Postac/Postac w dol prawo.png");
	Postac[25] = al_load_bitmap("Postac/Postac w dol prawo 1.png");
	Postac[26] = al_load_bitmap("Postac/Postac w dol prawo 2.png");
	Postac[27] = al_load_bitmap("Postac/Postac w dol prawo.png");
	Postac[28] = al_load_bitmap("Postac/Postac w dol prawo 3.png");
	Postac[29] = al_load_bitmap("Postac/Postac w dol prawo 4.png");

	Postac[30] = al_load_bitmap("Postac/Postac w dol lewo.png");
	Postac[31] = al_load_bitmap("Postac/Postac w dol lewo 1.png");
	Postac[32] = al_load_bitmap("Postac/Postac w dol lewo 2.png");
	Postac[33] = al_load_bitmap("Postac/Postac w dol lewo.png");
	Postac[34] = al_load_bitmap("Postac/Postac w dol lewo 3.png");
	Postac[35] = al_load_bitmap("Postac/Postac w dol lewo 4.png");

	Postac[36] = al_load_bitmap("Postac/Postac w gore prawo.png");
	Postac[37] = al_load_bitmap("Postac/Postac w gore prawo 1.png");
	Postac[38] = al_load_bitmap("Postac/Postac w gore prawo 2.png");
	Postac[39] = al_load_bitmap("Postac/Postac w gore prawo.png");
	Postac[40] = al_load_bitmap("Postac/Postac w gore prawo 3.png");
	Postac[41] = al_load_bitmap("Postac/Postac w gore prawo 4.png");

	Postac[42] = al_load_bitmap("Postac/Postac w gore lewo.png");
	Postac[43] = al_load_bitmap("Postac/Postac w gore lewo 1.png");
	Postac[44] = al_load_bitmap("Postac/Postac w gore lewo 2.png");
	Postac[45] = al_load_bitmap("Postac/Postac w gore lewo.png");
	Postac[46] = al_load_bitmap("Postac/Postac w gore lewo 3.png");
	Postac[47] = al_load_bitmap("Postac/Postac w gore lewo 4.png");

	for (int i = 0; i < 48; i++)
	{
		al_convert_mask_to_alpha(Postac[i], al_map_rgb(255, 100, 255));
	}

	// mapa
	bgImage = al_load_bitmap("background.png");

	// ekran startowy
	ALLEGRO_BITMAP *Ekran_startowy = al_load_bitmap("Ekran startowy.png");
	// parametry bitmapy
	ALLEGRO_BITMAP *LevelUp = al_load_bitmap("Ikony/Level Up.png");
	ALLEGRO_BITMAP *HP = al_load_bitmap("Ikony/HP.png");
	ALLEGRO_BITMAP *EXP = al_load_bitmap("Ikony/EXP.png");
	ALLEGRO_BITMAP *Pieniadze = al_load_bitmap("Ikony/Pieniadze.png");
	ALLEGRO_BITMAP *ZwojFireball = al_load_bitmap("Ikony/Zwoj Fireball.png");
	ALLEGRO_BITMAP *ZwojSopel = al_load_bitmap("Ikony/Zwoj Sopel.png");
	ALLEGRO_BITMAP *ZwojTrucizna = al_load_bitmap("Ikony/Zwoj Trucizna.png");
	ALLEGRO_BITMAP *Nieskonczonosc = al_load_bitmap("Ikony/Nieskonczonosc.png");
	// zegarek
	ALLEGRO_BITMAP *Zegar = al_load_bitmap("Ikony/Zegar.png");
	// menu
	ALLEGRO_BITMAP *Menu = al_load_bitmap("Menu/Menu.png");
	ALLEGRO_BITMAP *Jak_grac = al_load_bitmap("Menu/Jak grac.png");
	// sklep
	ALLEGRO_BITMAP *Sklep = al_load_bitmap("Sklep/Sklep.png");
	ALLEGRO_BITMAP *Pocisk_lodu = al_load_bitmap("Sklep/Pocisk lodu.png");
	ALLEGRO_BITMAP *Pocisk_trucizny = al_load_bitmap("Sklep/Pocisk trucizny.png");
	// ekran wynikow
	ALLEGRO_BITMAP *Wynik = al_load_bitmap("Ikony/Wynik.png");

	// okno
	okno = al_create_display(szerokosc, wysokosc);
	if (!okno)
		return -1;

	// eventy
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(okno));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));


	//czcionka
	ALLEGRO_FONT *font24 = al_load_ttf_font("font.ttf", 24, 0);
	ALLEGRO_FONT *font36 = al_load_ttf_font("centuary.ttf", 36, 0);

	// kursory
	float kursor_x1;
	float kursor_y1;
	float kursor_x3;
	float kursor_y3;
	float kursor_x4;
	float kursor_y4;
	float kursor_x5;
	float kursor_y5;

	
	// timer
	al_start_timer(timer);

	//ekran startowy
	
	al_draw_bitmap(Ekran_startowy, 0, 0, 0);
	al_flip_display();
	al_rest(1.5);

	//sklep
	bool czy_ma_wyswietlac1 = false;
	bool czy_ma_wyswietlac2 = false;
	float cena_sopla = 100;
	float cena_trucizny = 100;
	float cena_sopla5 = cena_sopla*5;
	float cena_trucizny5 = cena_trucizny*5;
	float cena_sopla10 = cena_sopla*10;
	float cena_trucizny10 = cena_trucizny*10;
	while (!done)
	{
	if (faza_gry == 1)
		{
			ALLEGRO_EVENT ev1;
			al_wait_for_event(event_queue, &ev1);
			if (ev1.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch (ev1.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
				{
					int wyjsc_z_gry = al_show_native_message_box
					(
						okno,
						"OrcVille",
						"Wyjsc z gry?",
						NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO
					);
					if (wyjsc_z_gry == 1)
						done = true;
				}
				}
			}
			else if (ev1.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				kursor_x1 = ev1.mouse.x;
				kursor_y1 = ev1.mouse.y;
			}
			else if (ev1.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				int wyjsc_z_gry = al_show_native_message_box
				(
					okno,
					"OrcVille",
					"Wyjsc z gry??",
					NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO
				);
				if (wyjsc_z_gry == 1)
					done = true;

				done = true;
			}
			
			// GRAJ
			if (kursor_x1 > 384 && kursor_x1 < 863 && kursor_y1 > 58 && kursor_y1 <185)
			{
				faza_gry = 2;
				kursor_x1 = -100;
				kursor_y1 = -100;
			}
			// JAK GRAC
			if (kursor_x1 > 384 && kursor_x1 < 863 && kursor_y1 > 213 && kursor_y1 < 339)
			{
				faza_gry = 3;
				kursor_x1 = -100;
				kursor_y1 = -100;
			}
			// WYJSCIE
			if (kursor_x1 > 384 && kursor_x1 < 863 && kursor_y1 > 361 && kursor_y1 < 488)
			{
				kursor_x1 = -100;
				kursor_y1 = -100;
				int wyjsc_z_gry = al_show_native_message_box
				(
					okno,
					"OrcVille",
					"Wyjsc z gry??",
					NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO
				);
				if (wyjsc_z_gry == 1)
					done = true;

			}
			al_draw_bitmap(Menu, 0, 0, 0);
			al_flip_display();
		}
	if (faza_gry == 2)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_W:
				wsad[W] = true;
				break;
			case ALLEGRO_KEY_S:
				wsad[S] = true;
				break;
			case ALLEGRO_KEY_A:
				wsad[A] = true;
				break;
			case ALLEGRO_KEY_D:
				wsad[D] = true;
				break;
			case ALLEGRO_KEY_SPACE:
			{
				faza_gry = 4;
				wsad[W] = false;
				wsad[S] = false;
				wsad[A] = false;
				wsad[D] = false;
				break;
			}
			case ALLEGRO_KEY_1:
				RodzajPocisku = 1;
				break;
			case ALLEGRO_KEY_2:
				RodzajPocisku = 2;
				break;
			case ALLEGRO_KEY_3:
				RodzajPocisku = 3;
				break;

			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_W:
				wsad[W] = false;
				break;
			case ALLEGRO_KEY_S:
				wsad[S] = false;
				break;
			case ALLEGRO_KEY_D:
				wsad[D] = false;
				break;
			case ALLEGRO_KEY_A:
				wsad[A] = false;
				break;
			case ALLEGRO_KEY_ESCAPE:
			{
				faza_gry = 1;
				wsad[W] = false;
				wsad[S] = false;
				wsad[A] = false;
				wsad[D] = false;
				break;
			}
			
			}
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.button & 1)
			{
				kursor_x = ev.mouse.x;
				kursor_y = ev.mouse.y;
				//std::cout << "X: " << kursor_x << std::endl;
				//std::cout << "Y: " << kursor_y << std::endl;
				if (faza_gry == 2)
				{
					if (RodzajPocisku == 1 && ilosc_fireball > 0)
					{
						StartPocisk(pociski, ilosc_pociskow, pos_x, pos_y);
					}
					if (RodzajPocisku == 2 && ilosc_sopel > 0)
					{
						StartPocisk(pociski, ilosc_pociskow, pos_x, pos_y);
					}
					if (RodzajPocisku == 3 && ilosc_trucizna > 0)
					{
						StartPocisk(pociski, ilosc_pociskow, pos_x, pos_y);
					}
				}
			}

		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			int wyjsc_z_gry = al_show_native_message_box
			(
				okno,
				"OrcVille",
				"Wyjsc z gry??",
				NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO
			);
			if (wyjsc_z_gry == 1)
				done = true;

		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			
				// czas gry
				zegar++;
				if (zegar % 60 == 0)
				{
					czas++;
				}
				if(zegar%600==0)
				{
					if(losowanie_orka>200)
						losowanie_orka -= 25;
					if(losowanie_szamana>40)
						losowanie_szamana -= 5;
					if (ilosc_orkow < 20)
						ilosc_orkow + 1;
					if (ilosc_szamanow<10)
						ilosc_szamanow += 1;

					for(int i=0; i<50; i++)
					{ 
						orkowie[i].hp += 5;
						szamani[i].hp += 10;
					}
				}
				pomocnicza_do_sprite_orka++;
				pomocnicza_do_sprite_szamana++;

				StartOrk(orkowie, ilosc_orkow);
				StartSzaman(szamani, ilosc_szamanow);
				RuchOrk(orkowie, ilosc_orkow);
				RuchSzaman(szamani, ilosc_szamanow);
				RuchPocisk(pociski, ilosc_pociskow);
				
			
			if (pomocnicza_do_sprite_orka == 50)
				pomocnicza_do_sprite_orka = 0;
			if (pomocnicza_do_sprite_szamana == 50)
				pomocnicza_do_sprite_szamana = 0;

			// pomocnicze by sprawdzac czy mapa sie porusza
			int pom_mapa_x = mapa_x;
			int pom_mapa_y = mapa_y;
			// warunki przewijania mapy
			
				if (mapa_x < 0)
					mapa_x += wsad[A] * predkosc_mapy;
				if (mapa_y < 0)
					mapa_y += wsad[W] * predkosc_mapy;
				if (mapa_x < 1500)
					mapa_x -= wsad[D] * predkosc_mapy;
				if (mapa_y < 1500)
					mapa_y -= wsad[S] * predkosc_mapy;
			
			
			// warunki zablokowania mapy
			if (mapa_x < -250)
				mapa_x = -250;
			if (mapa_y < -800)
				mapa_y = -800;
			if (mapa_x > 0)
				mapa_x = 0;
			if (mapa_y > 0)
				mapa_y = 0;

			// mapa nie porusza ...
			if (faza_gry == 2)
			{
				// mapa nie porusza postaci
				if (faza_gry == 2)
				{
					if (((wsad[W] == true) && (wsad[A] == false) && (wsad[D] == false)) || ((wsad[W] == true) && (wsad[A] == true) && (wsad[D] == true)))
					{
						KtoryXY(pos_x, pos_y, mapa_x, mapa_y, ktoryX, ktoryY);
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciW(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							resp_prawo_gora_y -=predkosc_postaci ;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							RuchPostaciW(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_y += predkosc_mapy;
							resp_prawo_gora_y -= predkosc_postaci;
						}
					}

					if ((wsad[W] == true) && (wsad[A] == true) && (wsad[D] == false))
					{
						KtoryXY(pos_x, pos_y, mapa_x, mapa_y, ktoryX, ktoryY);
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
						{
							RuchPostaciW(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciA(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_y += predkosc_mapy;
							pos_x += predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							RuchPostaciW(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciA(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_y += predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciW(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciA(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_x += predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciW(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciA(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
						}
					}

					if ((wsad[W] == true) && (wsad[A] == false) && (wsad[D] == true))
					{
						KtoryXY(pos_x, pos_y, mapa_x, mapa_y, ktoryX, ktoryY);
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
						{
							RuchPostaciW(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciD(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_y += predkosc_mapy;
							pos_x -= predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							RuchPostaciW(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciD(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_y += predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciW(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciD(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_x -= predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciW(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciD(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
						}
					}


					if (((wsad[A] == true) && (wsad[W] == false) && (wsad[S] == false)) || ((wsad[W] == true) && (wsad[A] == true) && (wsad[S] == true)))
					{
						KtoryXY(pos_x, pos_y, mapa_x, mapa_y, ktoryX, ktoryY);
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciA(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciA(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_x += predkosc_mapy;
						}
					}

					if ((wsad[A] == true) && (wsad[W] == false) && (wsad[S] == true))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
						{
							RuchPostaciA(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciS(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_x += predkosc_mapy;
							pos_y -= predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							RuchPostaciA(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciS(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_y -= predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciA(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciS(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_x += predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciA(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciS(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
						}
					}


					if (((wsad[S] == true) && (wsad[A] == false) && (wsad[D] == false)) || ((wsad[S] == true) && (wsad[A] == true) && (wsad[D] == true)))
					{
						KtoryXY(pos_x, pos_y, mapa_x, mapa_y, ktoryX, ktoryY);
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciS(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							RuchPostaciS(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_y -= predkosc_mapy;
						}
					}

					if ((wsad[S] == true) && (wsad[A] == false) && (wsad[D] == true))
					{
						KtoryXY(pos_x, pos_y, mapa_x, mapa_y, ktoryX, ktoryY);
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
						{
							RuchPostaciS(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciD(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_y -= predkosc_mapy;
							pos_x -= predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							RuchPostaciS(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciD(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_y -= predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciS(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciD(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_x -= predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciS(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							RuchPostaciD(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
						}
					}


					if (((wsad[D] == true) && (wsad[W] == false) && (wsad[S] == false)) || ((wsad[W] == true) && (wsad[D] == true) && (wsad[S] == true)))
					{
						KtoryXY(pos_x, pos_y, mapa_x, mapa_y, ktoryX, ktoryY);
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciD(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							RuchPostaciD(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
							pos_x -= predkosc_mapy;
						}
					}
				}

				// mapa nie porusza orkow
				for (int i = 0; i < ilosc_orkow; i++)
				{

					if (((wsad[W] == true) && (wsad[A] == false) && (wsad[D] == false)) || ((wsad[W] == true) && (wsad[A] == true) && (wsad[D] == true)))
					{
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							orkowie[i].y += predkosc_mapy;
						}
					}

					if ((wsad[W] == true) && (wsad[A] == true) && (wsad[D] == false))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
						{
							orkowie[i].y += predkosc_mapy;
							orkowie[i].x += predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							orkowie[i].y += predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							orkowie[i].x += predkosc_mapy;
						}
					}

					if ((wsad[W] == true) && (wsad[A] == false) && (wsad[D] == true))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
						{
							orkowie[i].y += predkosc_mapy;
							orkowie[i].x -= predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							orkowie[i].y += predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							orkowie[i].x -= predkosc_mapy;
						}
					}


					if ((wsad[A] == true) && (wsad[W] == false) && (wsad[S] == false))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							orkowie[i].x += predkosc_mapy;
						}
					}

					if ((wsad[A] == true) && (wsad[W] == false) && (wsad[S] == true))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
						{
							orkowie[i].x += predkosc_mapy;
							orkowie[i].y -= predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							orkowie[i].y -= predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							orkowie[i].x += predkosc_mapy;
						}
					}


					if ((wsad[S] == true) && (wsad[A] == false) && (wsad[D] == false))
					{
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							orkowie[i].y -= predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							orkowie[i].y -= predkosc_mapy;
						}
					}

					if ((wsad[S] == true) && (wsad[A] == false) && (wsad[D] == true))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
						{
							orkowie[i].y -= predkosc_mapy;
							orkowie[i].x -= predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							orkowie[i].y -= predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							orkowie[i].x -= predkosc_mapy;
						}
					}


					if ((wsad[D] == true) && (wsad[W] == false) && (wsad[S] == false))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							orkowie[i].x -= predkosc_mapy;
						}
					}

				}

				//mapa nie porusza szamanow
				for (int i = 0; i < ilosc_szamanow; i++)
				{

					if (((wsad[W] == true) && (wsad[A] == false) && (wsad[D] == false)) || ((wsad[W] == true) && (wsad[A] == true) && (wsad[D] == true)))
					{
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							szamani[i].y += predkosc_mapy;
						}
					}

					if ((wsad[W] == true) && (wsad[A] == true) && (wsad[D] == false))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
						{
							szamani[i].y += predkosc_mapy;
							szamani[i].x += predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							szamani[i].y += predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							szamani[i].x += predkosc_mapy;
						}
					}

					if ((wsad[W] == true) && (wsad[A] == false) && (wsad[D] == true))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
						{
							szamani[i].y += predkosc_mapy;
							szamani[i].x -= predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							szamani[i].y += predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							szamani[i].x -= predkosc_mapy;
						}
					}


					if ((wsad[A] == true) && (wsad[W] == false) && (wsad[S] == false))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							szamani[i].x += predkosc_mapy;
						}
					}

					if ((wsad[A] == true) && (wsad[W] == false) && (wsad[S] == true))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
						{
							szamani[i].x += predkosc_mapy;
							szamani[i].y -= predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							szamani[i].y -= predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							szamani[i].x += predkosc_mapy;
						}
					}


					if ((wsad[S] == true) && (wsad[A] == false) && (wsad[D] == false))
					{
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							szamani[i].y -= predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							szamani[i].y -= predkosc_mapy;
						}
					}

					if ((wsad[S] == true) && (wsad[A] == false) && (wsad[D] == true))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
						{
							szamani[i].y -= predkosc_mapy;
							szamani[i].x -= predkosc_mapy;
						}
						if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
						{
							szamani[i].y -= predkosc_mapy;
						}
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							szamani[i].x -= predkosc_mapy;
						}
					}


					if ((wsad[D] == true) && (wsad[W] == false) && (wsad[S] == false))
					{
						if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
						{
							szamani[i].x -= predkosc_mapy;
						}
					}

				}

				// mapa nie porusza pociskow
				for (int i = 0; i < ilosc_pociskow; i++)
				{
					if (pociski[i].live)
					{
						if ((wsad[W] == true) && (wsad[A] == false) && (wsad[D] == false))
						{
							if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
							{
								pociski[i].y += predkosc_mapy;
							}
						}

						if ((wsad[W] == true) && (wsad[A] == true) && (wsad[D] == false))
						{
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
							{
								pociski[i].y += predkosc_mapy;
								pociski[i].x += predkosc_mapy;
							}
							if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
							{
								pociski[i].y += predkosc_mapy;
							}
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								pociski[i].x += predkosc_mapy;
							}
						}

						if ((wsad[W] == true) && (wsad[A] == false) && (wsad[D] == true))
						{
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
							{
								pociski[i].y += predkosc_mapy;
								pociski[i].x -= predkosc_mapy;
							}
							if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
							{
								pociski[i].y += predkosc_mapy;
							}
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								pociski[i].x -= predkosc_mapy;
							}
						}


						if ((wsad[A] == true) && (wsad[W] == false) && (wsad[S] == false))
						{
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								pociski[i].x += predkosc_mapy;
							}
						}

						if ((wsad[A] == true) && (wsad[W] == false) && (wsad[S] == true))
						{
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
							{
								pociski[i].x += predkosc_mapy;
								pociski[i].y -= predkosc_mapy;
							}
							if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
							{
								pociski[i].y -= predkosc_mapy;
							}
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								pociski[i].x += predkosc_mapy;
							}
						}


						if ((wsad[S] == true) && (wsad[A] == false) && (wsad[D] == false))
						{
							if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
							{
								pociski[i].y -= predkosc_mapy;
							}
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								pociski[i].y -= predkosc_mapy;
							}
						}

						if ((wsad[S] == true) && (wsad[A] == false) && (wsad[D] == true))
						{
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
							{
								pociski[i].y -= predkosc_mapy;
								pociski[i].x -= predkosc_mapy;
							}
							if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
							{
								pociski[i].y -= predkosc_mapy;
							}
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								pociski[i].x -= predkosc_mapy;
							}
						}


						if ((wsad[D] == true) && (wsad[W] == false) && (wsad[S] == false))
						{
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								pociski[i].x -= predkosc_mapy;
							}
						}
					}
				}
				
				// mapa nie porusza pociskow szamanow
				for (int i = 0; i < ilosc_pociskow; i++)
				{
					if (szamani[i].pociski_szamana.live==true)
					{
						if ((wsad[W] == true) && (wsad[A] == false) && (wsad[D] == false))
						{
							if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
							{
								szamani[i].pociski_szamana.y += predkosc_mapy;
							}
						}

						if ((wsad[W] == true) && (wsad[A] == true) && (wsad[D] == false))
						{
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
							{
								szamani[i].pociski_szamana.y += predkosc_mapy;
								szamani[i].pociski_szamana.x += predkosc_mapy;
							}
							if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
							{
								szamani[i].pociski_szamana.y += predkosc_mapy;
							}
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								szamani[i].pociski_szamana.x += predkosc_mapy;
							}
						}

						if ((wsad[W] == true) && (wsad[A] == false) && (wsad[D] == true))
						{
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
							{
								szamani[i].pociski_szamana.y += predkosc_mapy;
								szamani[i].pociski_szamana.x -= predkosc_mapy;
							}
							if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
							{
								szamani[i].pociski_szamana.y += predkosc_mapy;
							}
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								szamani[i].pociski_szamana.x -= predkosc_mapy;
							}
						}


						if ((wsad[A] == true) && (wsad[W] == false) && (wsad[S] == false))
						{
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								szamani[i].pociski_szamana.x += predkosc_mapy;
							}
						}

						if ((wsad[A] == true) && (wsad[W] == false) && (wsad[S] == true))
						{
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
							{
								szamani[i].pociski_szamana.x += predkosc_mapy;
								szamani[i].pociski_szamana.y -= predkosc_mapy;
							}
							if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
							{
								szamani[i].pociski_szamana.y -= predkosc_mapy;
							}
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								szamani[i].pociski_szamana.x += predkosc_mapy;
							}
						}


						if ((wsad[S] == true) && (wsad[A] == false) && (wsad[D] == false))
						{
							if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
							{
								szamani[i].pociski_szamana.y -= predkosc_mapy;
							}
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								szamani[i].pociski_szamana.y -= predkosc_mapy;
							}
						}

						if ((wsad[S] == true) && (wsad[A] == false) && (wsad[D] == true))
						{
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y != mapa_y))
							{
								szamani[i].pociski_szamana.y -= predkosc_mapy;
								szamani[i].pociski_szamana.x -= predkosc_mapy;
							}
							if ((pom_mapa_x == mapa_x) && (pom_mapa_y != mapa_y))
							{
								szamani[i].pociski_szamana.y -= predkosc_mapy;
							}
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								szamani[i].pociski_szamana.x -= predkosc_mapy;
							}
						}


						if ((wsad[D] == true) && (wsad[W] == false) && (wsad[S] == false))
						{
							if ((pom_mapa_x != mapa_x) && (pom_mapa_y == mapa_y))
							{
								szamani[i].pociski_szamana.x -= predkosc_mapy;
							}
						}
					}
				}
				
			}

			// kolizja pocisk-ork oraz postac-ork
			
			for (int i = 0; i < ilosc_orkow; i++)
			{
				for (int j = 0; j < ilosc_pociskow; j++)
				{
					if (orkowie[i].live == true)
					{
							// efekt sopla
							if (orkowie[i].efekt_pocisku_sopel == true)
							{
								orkowie[i].predkosc = 0.0; // slow na ~~5sec
								orkowie[i].pomocnicza_do_efektu++;
								if (orkowie[i].pomocnicza_do_efektu == 1300)
								{
									orkowie[i].predkosc = 1;
									orkowie[i].efekt_pocisku_sopel = false;
									orkowie[i].pomocnicza_do_efektu = 0;
								}
							}
							// efekt trucizny
							if (orkowie[i].efekt_pocisku_trucizna == true)
							{
								orkowie[i].pomocnicza_do_efektu++;
								if (orkowie[i].pomocnicza_do_efektu == 3000)
								{
									orkowie[i].pomocnicza_do_efektu = 0;
									orkowie[i].efekt_pocisku_trucizna = false;
								}
								if (orkowie[i].pomocnicza_do_efektu % 250 == 0)
								{
									orkowie[i].hp -= 20; // trucizna zabiera 30hp  w ~~3,5sec
								}
							}
						

						if (((((orkowie[i].x - 40) <= pociski[j].x - 20) && ((orkowie[i].x + 40) >= pociski[j].x + 20))) &&
							((((orkowie[i].y - 40) <= pociski[j].y - 20) && ((orkowie[i].y + 40) >= pociski[j].y + 20))))
						{

							pociski[j].live = false;
							pociski[j].x = -1000;
							pociski[j].y = -1000;

							if (pociski[j].rodzaj_pocisku == 1)
							{
								orkowie[i].hp -= fireball_dmg;
							}
							if (pociski[j].rodzaj_pocisku == 2)
							{
								orkowie[i].hp -= sopel_dmg;
								orkowie[i].efekt_pocisku_sopel = true;
							}
							if (pociski[j].rodzaj_pocisku == 3)
							{
								orkowie[i].hp -= trucizna_dmg;
								orkowie[i].efekt_pocisku_trucizna = true;
							}
						}
					}
				}
				if (orkowie[i].hp <= 0)
				{

					orkowie[i].live = false;
					pieniadze += 150;
					doswiadczenie += 10;
					score += 10;
					orkowie[i].hp = 100;
					orkowie[i].predkosc = 1;
					orkowie[i].efekt_pocisku_sopel = false;
					orkowie[i].efekt_pocisku_trucizna = false;
					orkowie[i].efekt_krwi = true;
				}

				if (orkowie[i].live == true)
				{
					if (((((orkowie[i].x - 40) <= pos_x) && ((orkowie[i].x + 40) >= pos_x))) &&
						((((orkowie[i].y - 40) <= pos_y) && ((orkowie[i].y + 40) >= pos_y))))
					{
						if (pomocnicza_do_straty_hp <50)
						{
							pomocnicza_do_straty_hp++;
							if (pomocnicza_do_straty_hp == 50)
								pomocnicza_do_straty_hp = 0;
							if(pomocnicza_do_straty_hp==25)
								pos_hp -= 25;
						}
					}
				}
			}

			// kolizcja pocisk-szaman oraz szaman-postac - pociski postaci
			for (int i = 0; i < ilosc_szamanow; i++)
			{
				if (szamani[i].live == true)
				{
					if (szamani[i].odleglosc_xy <= 400)
					{
						if (szamani[i].pociski_szamana.czy_moze_strzelic_szaman == false)
						{
							szamani[i].pomocnicza_do_strzalu_szamana++;
							if (szamani[i].pomocnicza_do_strzalu_szamana == 120)
							{
								szamani[i].pomocnicza_do_strzalu_szamana = 0;
								szamani[i].pociski_szamana.czy_moze_strzelic_szaman = true;
							}
						}
						if (szamani[i].pociski_szamana.czy_moze_strzelic_szaman == true)
						{
							szamani[i].pociski_szamana.czy_moze_strzelic_szaman = false;
							StartPociskSzamana(ilosc_szamanow, szamani);
						}
					}
				}
				for (int j = 0; j < ilosc_pociskow; j++)
				{
					if (szamani[i].live == true)
					{
							// efekt sopla
							if (szamani[i].efekt_pocisku_sopel == true)
							{
								szamani[i].predkosc = 0.0; // slow na ~~5sec
								szamani[i].pomocnicza_do_efektu++;
								if (szamani[i].pomocnicza_do_efektu == 1300)
								{
									szamani[i].predkosc = 1;
									szamani[i].efekt_pocisku_sopel = false;
									szamani[i].pomocnicza_do_efektu = 0;
								}
							}
							// efekt trucizny
							if (szamani[i].efekt_pocisku_trucizna == true)
							{
								szamani[i].pomocnicza_do_efektu++;
								if (szamani[i].pomocnicza_do_efektu == 3000)
								{
									szamani[i].pomocnicza_do_efektu = 0;
									szamani[i].efekt_pocisku_trucizna = false;
								}
								if (szamani[i].pomocnicza_do_efektu % 250 == 0)
								{
									szamani[i].hp -= 20; // trucizna zabiera 30hp  w ~~3,5sec
								}
							}
						
						
						if (((((szamani[i].x - 40) <= pociski[j].x - 20) && ((szamani[i].x + 40) >= pociski[j].x + 20))) &&
							((((szamani[i].y - 40) <= pociski[j].y - 20) && ((szamani[i].y + 40) >= pociski[j].y + 20))))
						{
							
							pociski[j].live = false;
							pociski[j].x = -1000;
							pociski[j].y = -1000;

							if (pociski[j].rodzaj_pocisku == 1)
							{
								szamani[i].hp -= fireball_dmg;
							}
							if (pociski[j].rodzaj_pocisku == 2)
							{
								szamani[i].hp -= sopel_dmg;
								szamani[i].efekt_pocisku_sopel = true;
							}
							if (pociski[j].rodzaj_pocisku == 3)
							{
								szamani[i].hp -= trucizna_dmg;
								szamani[i].efekt_pocisku_trucizna = true;
							}
						}
						
					}
				}
				
				if (szamani[i].hp <= 0)
				{

					szamani[i].live = false;
					pieniadze += 250;
					doswiadczenie += 15;
					score += 15;
					szamani[i].hp = 120;
					szamani[i].predkosc = 1;
					szamani[i].efekt_pocisku_sopel = false;
					szamani[i].efekt_pocisku_trucizna = false;
					szamani[i].efekt_krwi = true;
				}

				if (szamani[i].live == true)
				{
					if (((((szamani[i].x - 40) <= pos_x) && ((szamani[i].x + 40) >= pos_x))) &&
						((((szamani[i].y - 40) <= pos_y) && ((szamani[i].y + 40) >= pos_y))))
					{
						if (pomocnicza_do_straty_hp2 <50)
						{
							pomocnicza_do_straty_hp2++;
							if (pomocnicza_do_straty_hp2 == 50)
								pomocnicza_do_straty_hp2 = 0;
							if (pomocnicza_do_straty_hp2 == 25)
								pos_hp -= 25;
						}
					}
				}
			}
			RuchPociskSzamana(ilosc_szamanow,szamani );

			// kolizja pocisk_szamana-postac  - pociski szamana
			
				for (int j = 0; j < ilosc_szamanow; j++)
				{
					if (((((szamani[j].pociski_szamana.x - 20) <= pos_x) && ((szamani[j].pociski_szamana.x + 20) >= pos_x))) &&
						((((szamani[j].pociski_szamana.y - 20) <= pos_y) && ((szamani[j].pociski_szamana.y + 20) >= pos_y))))

						{
							szamani[j].pociski_szamana.live = false;
							szamani[j].pociski_szamana.x = -1000;
							szamani[j].pociski_szamana.y = -1000;
							pos_hp -= 10;
						}
				}
				
			// koniec gry
			if (pos_hp <= 0)
			{

				
				pomocnicza_do_konca_gry_przegrana++;
				if (pomocnicza_do_konca_gry_przegrana == 10)
				{
					score = score + czas * 5;
					faza_gry = 5;
				}
			}

			// postac nie moze wyjsc poza obszar okna
			if (pos_x < 0)
				pos_x = 0;
			if (pos_y < 0)
				pos_y = 0;
			if (pos_x > 1200)
				pos_x = 1200;
			if (pos_y > 650)
				pos_y = 650;

			if (level == 1)
			{
				if (doswiadczenie >= 100)
				{
					pos_hp = 100;
					level++;
					doswiadczenie -= 100;
				}
			}
			if (level == 2)
			{
				level_up++;
				score += 100;
				if (doswiadczenie >= 200)
				{
					pos_hp = 100;
					level++;
					doswiadczenie -= 200;
					level_up = 0;
				}
			}
			if (level == 3)
			{
				level_up++;
				score += 500;
				if (doswiadczenie >= 300)
				{
					pos_hp = 100;
					level++;
					doswiadczenie -= 300;
					level_up = 0;
				}
			}
			if (level == 4)
			{
				level_up++;
				score += 1000;
				if (doswiadczenie >= 400)
				{
					pos_hp = 100;
					level++;
					doswiadczenie -= 400;
					level_up = 0;
				}
			}
			if (level == 5)
			{
				level_up++;
				score += 1500;
				if (doswiadczenie >= 500)
				{
					pos_hp = 100;
					level++;
					doswiadczenie -= 500;
					level_up = 0;
				}
			}
			if (level == 6)
			{
				level_up++;
				score += 2000;
				if (doswiadczenie >= 600)
				{
					pos_hp = 100;
					level++;
					doswiadczenie -= 600;
					level_up = 0;
				}
			}

			redraw = true;
				// rysowanie mapy
				for (int i = 0; i < rozmiar_mapy; i++)
				{
					al_draw_bitmap_region(bgImage, rozmiar_pola * mapa[i], 0, rozmiar_pola, rozmiar_pola,
						mapa_x + rozmiar_pola * (i % szerokosc_mapy), mapa_y + rozmiar_pola * (i / szerokosc_mapy), 0);
				}
				if (redraw == true)
				{
					//sprite postaci
					if (wsad[W] == true && wsad[S] == false && wsad[A] == false && wsad[D] == false)
					{
						if (faza_gry == 2)
							ilosc_klatek++;
						ostatnia_pozycja = W;
						//predkosc_linia_prosta = 4;
						if (ilosc_klatek == 0)
						{
							al_draw_bitmap(Postac[6], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 0 && ilosc_klatek <= 10)
						{
							al_draw_bitmap(Postac[7], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 10 && ilosc_klatek <= 20)
						{
							al_draw_bitmap(Postac[8], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 20 && ilosc_klatek <= 30)
						{
							al_draw_bitmap(Postac[9], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 30 && ilosc_klatek <= 40)
						{
							al_draw_bitmap(Postac[10], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 40 && ilosc_klatek <= 50)
						{
							al_draw_bitmap(Postac[11], pos_x, pos_y, 0);
						}
						if (ilosc_klatek == 50)
						{
							ilosc_klatek = 0;
						}
					}
					if (wsad[W] == false && wsad[S] == true && wsad[A] == false && wsad[D] == false)
					{
						if (faza_gry == 2)
							ilosc_klatek++;
						ostatnia_pozycja = S;
						//predkosc_linia_prosta = 4;
						if (ilosc_klatek == 0)
						{
							al_draw_bitmap(Postac[0], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 0 && ilosc_klatek <= 10)
						{
							al_draw_bitmap(Postac[1], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 10 && ilosc_klatek <= 20)
						{
							al_draw_bitmap(Postac[2], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 20 && ilosc_klatek <= 30)
						{
							al_draw_bitmap(Postac[3], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 30 && ilosc_klatek <= 40)
						{
							al_draw_bitmap(Postac[4], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 40 && ilosc_klatek <= 50)
						{
							al_draw_bitmap(Postac[5], pos_x, pos_y, 0);
						}
						if (ilosc_klatek == 50)
						{
							ilosc_klatek = 0;
						}
					}
					if (wsad[W] == false && wsad[S] == false && wsad[A] == false && wsad[D] == true)
					{
						if (faza_gry == 2)
							ilosc_klatek++;
						ostatnia_pozycja = D;
						//predkosc_linia_prosta = 4;
						if (ilosc_klatek == 0)
						{
							al_draw_bitmap(Postac[12], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 0 && ilosc_klatek <= 10)
						{
							al_draw_bitmap(Postac[13], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 10 && ilosc_klatek <= 20)
						{
							al_draw_bitmap(Postac[14], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 20 && ilosc_klatek <= 30)
						{
							al_draw_bitmap(Postac[15], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 30 && ilosc_klatek <= 40)
						{
							al_draw_bitmap(Postac[16], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 40 && ilosc_klatek <= 50)
						{
							al_draw_bitmap(Postac[17], pos_x, pos_y, 0);
						}
						if (ilosc_klatek == 50)
						{
							ilosc_klatek = 0;
						}
					}
					if (wsad[W] == false && wsad[S] == false && wsad[A] == true && wsad[D] == false)
					{
						if (faza_gry == 2)
							ilosc_klatek++;
						ostatnia_pozycja = A;
						//predkosc_linia_prosta = 4;
						if (ilosc_klatek == 0)
						{
							al_draw_bitmap(Postac[18], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 0 && ilosc_klatek <= 10)
						{
							al_draw_bitmap(Postac[19], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 10 && ilosc_klatek <= 20)
						{
							al_draw_bitmap(Postac[20], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 20 && ilosc_klatek <= 30)
						{
							al_draw_bitmap(Postac[21], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 30 && ilosc_klatek <= 40)
						{
							al_draw_bitmap(Postac[22], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 40 && ilosc_klatek <= 50)
						{
							al_draw_bitmap(Postac[23], pos_x, pos_y, 0);
						}
						if (ilosc_klatek == 50)
						{
							ilosc_klatek = 0;
						}
					}
					if (wsad[S] == true && wsad[D] == true && wsad[A] == false && wsad[W] == false)
					{
						if (faza_gry == 2)
							ilosc_klatek++;
						//predkosc_linia_prosta = 3;
						if (ilosc_klatek == 0)
						{
							al_draw_bitmap(Postac[24], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 0 && ilosc_klatek <= 10)
						{
							al_draw_bitmap(Postac[25], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 10 && ilosc_klatek <= 20)
						{
							al_draw_bitmap(Postac[26], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 20 && ilosc_klatek <= 30)
						{
							al_draw_bitmap(Postac[27], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 30 && ilosc_klatek <= 40)
						{
							al_draw_bitmap(Postac[28], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 40 && ilosc_klatek <= 50)
						{
							al_draw_bitmap(Postac[29], pos_x, pos_y, 0);
						}
						if (ilosc_klatek == 50)
						{
							ilosc_klatek = 0;
						}
					}
					if (wsad[S] == true && wsad[D] == false && wsad[A] == true && wsad[W] == false)
					{
						if (faza_gry == 2)
							ilosc_klatek++;
						//predkosc_linia_prosta = 3;
						if (ilosc_klatek == 0)
						{
							al_draw_bitmap(Postac[30], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 0 && ilosc_klatek <= 10)
						{
							al_draw_bitmap(Postac[31], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 10 && ilosc_klatek <= 20)
						{
							al_draw_bitmap(Postac[32], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 20 && ilosc_klatek <= 30)
						{
							al_draw_bitmap(Postac[33], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 30 && ilosc_klatek <= 40)
						{
							al_draw_bitmap(Postac[34], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 40 && ilosc_klatek <= 50)
						{
							al_draw_bitmap(Postac[35], pos_x, pos_y, 0);
						}
						if (ilosc_klatek == 50)
						{
							ilosc_klatek = 0;
						}
					}
					if (wsad[S] == false && wsad[D] == true && wsad[A] == false && wsad[W] == true)
					{
						if (faza_gry == 2)
							ilosc_klatek++;
						//predkosc_linia_prosta = 3;
						if (ilosc_klatek == 0)
						{
							al_draw_bitmap(Postac[36], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 0 && ilosc_klatek <= 10)
						{
							al_draw_bitmap(Postac[37], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 10 && ilosc_klatek <= 20)
						{
							al_draw_bitmap(Postac[38], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 20 && ilosc_klatek <= 30)
						{
							al_draw_bitmap(Postac[39], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 30 && ilosc_klatek <= 40)
						{
							al_draw_bitmap(Postac[40], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 40 && ilosc_klatek <= 50)
						{
							al_draw_bitmap(Postac[41], pos_x, pos_y, 0);
						}
						if (ilosc_klatek == 50)
						{
							ilosc_klatek = 0;
						}
					}
					if (wsad[S] == false && wsad[D] == false && wsad[A] == true && wsad[W] == true)
					{
						if (faza_gry == 2)
							ilosc_klatek++;
						//predkosc_linia_prosta = 3;
						if (ilosc_klatek == 0)
						{
							al_draw_bitmap(Postac[42], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 0 && ilosc_klatek <= 10)
						{
							al_draw_bitmap(Postac[43], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 10 && ilosc_klatek <= 20)
						{
							al_draw_bitmap(Postac[44], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 20 && ilosc_klatek <= 30)
						{
							al_draw_bitmap(Postac[45], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 30 && ilosc_klatek <= 40)
						{
							al_draw_bitmap(Postac[46], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 40 && ilosc_klatek <= 50)
						{
							al_draw_bitmap(Postac[47], pos_x, pos_y, 0);
						}
						if (ilosc_klatek == 50)
						{
							ilosc_klatek = 0;
						}
					}
					if (wsad[W] == false && wsad[S] == false && wsad[D] == false && wsad[A] == false)
					{
						//predkosc_linia_prosta = 3;
						if (ostatnia_pozycja != S && ostatnia_pozycja != W && ostatnia_pozycja != D && ostatnia_pozycja != A)
							al_draw_bitmap(Postac[0], pos_x, pos_y, 0);
						if (ostatnia_pozycja == W)
							al_draw_bitmap(Postac[6], pos_x, pos_y, 0);
						if (ostatnia_pozycja == S)
							al_draw_bitmap(Postac[0], pos_x, pos_y, 0);
						if (ostatnia_pozycja == A)
							al_draw_bitmap(Postac[18], pos_x, pos_y, 0);
						if (ostatnia_pozycja == D)
							al_draw_bitmap(Postac[12], pos_x, pos_y, 0);
					}
					if (wsad[W] == true && wsad[S] == true && wsad[D] == false && wsad[A] == false)
					{
						//predkosc_linia_prosta = 3;
						if (ostatnia_pozycja != S && ostatnia_pozycja != W && ostatnia_pozycja != D && ostatnia_pozycja != A)
							al_draw_bitmap(Postac[0], pos_x, pos_y, 0);
						if (ostatnia_pozycja == W)
							al_draw_bitmap(Postac[6], pos_x, pos_y, 0);
						if (ostatnia_pozycja == S)
							al_draw_bitmap(Postac[0], pos_x, pos_y, 0);
						if (ostatnia_pozycja == A)
							al_draw_bitmap(Postac[18], pos_x, pos_y, 0);
						if (ostatnia_pozycja == D)
							al_draw_bitmap(Postac[12], pos_x, pos_y, 0);
					}
					if (wsad[W] == false && wsad[S] == false && wsad[D] == true && wsad[A] == true)
					{
						//predkosc_linia_prosta = 3;
						if (ostatnia_pozycja != S && ostatnia_pozycja != W && ostatnia_pozycja != D && ostatnia_pozycja != A)
							al_draw_bitmap(Postac[0], pos_x, pos_y, 0);
						if (ostatnia_pozycja == W)
							al_draw_bitmap(Postac[6], pos_x, pos_y, 0);
						if (ostatnia_pozycja == S)
							al_draw_bitmap(Postac[0], pos_x, pos_y, 0);
						if (ostatnia_pozycja == A)
							al_draw_bitmap(Postac[18], pos_x, pos_y, 0);
						if (ostatnia_pozycja == D)
							al_draw_bitmap(Postac[12], pos_x, pos_y, 0);
					}
					if (wsad[W] == true && wsad[S] == true && wsad[D] == true && wsad[A] == false)
					{
						if (faza_gry == 2)
							ilosc_klatek++;
						ostatnia_pozycja = D;
						//predkosc_linia_prosta = 2;
						if (ilosc_klatek == 0)
						{
							al_draw_bitmap(Postac[12], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 0 && ilosc_klatek <= 10)
						{
							al_draw_bitmap(Postac[13], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 10 && ilosc_klatek <= 20)
						{
							al_draw_bitmap(Postac[14], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 20 && ilosc_klatek <= 30)
						{
							al_draw_bitmap(Postac[15], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 30 && ilosc_klatek <= 40)
						{
							al_draw_bitmap(Postac[16], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 40 && ilosc_klatek <= 50)
						{
							al_draw_bitmap(Postac[17], pos_x, pos_y, 0);
						}
						if (ilosc_klatek == 50)
						{
							ilosc_klatek = 0;
						}
					}
					if (wsad[W] == true && wsad[S] == true && wsad[D] == false && wsad[A] == true)
					{
						if (faza_gry == 2)
							ilosc_klatek++;
						ostatnia_pozycja = A;
						//predkosc_linia_prosta = 2;
						if (ilosc_klatek == 0)
						{
							al_draw_bitmap(Postac[18], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 0 && ilosc_klatek <= 10)
						{
							al_draw_bitmap(Postac[19], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 10 && ilosc_klatek <= 20)
						{
							al_draw_bitmap(Postac[20], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 20 && ilosc_klatek <= 30)
						{
							al_draw_bitmap(Postac[21], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 30 && ilosc_klatek <= 40)
						{
							al_draw_bitmap(Postac[22], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 40 && ilosc_klatek <= 50)
						{
							al_draw_bitmap(Postac[23], pos_x, pos_y, 0);
						}
						if (ilosc_klatek == 50)
						{
							ilosc_klatek = 0;
						}
					}
					if (wsad[W] == true && wsad[S] == false && wsad[D] == true && wsad[A] == true)
					{
						if (faza_gry == 2)
							ilosc_klatek++;
						ostatnia_pozycja = W;
						//predkosc_linia_prosta = 2;
						if (ilosc_klatek == 0)
						{
							al_draw_bitmap(Postac[6], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 0 && ilosc_klatek <= 10)
						{
							al_draw_bitmap(Postac[7], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 10 && ilosc_klatek <= 20)
						{
							al_draw_bitmap(Postac[8], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 20 && ilosc_klatek <= 30)
						{
							al_draw_bitmap(Postac[9], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 30 && ilosc_klatek <= 40)
						{
							al_draw_bitmap(Postac[10], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 40 && ilosc_klatek <= 50)
						{
							al_draw_bitmap(Postac[11], pos_x, pos_y, 0);
						}
						if (ilosc_klatek == 50)
						{
							ilosc_klatek = 0;
						}
					}
					if (wsad[W] == false && wsad[S] == true && wsad[D] == true && wsad[A] == true)
					{
						if (faza_gry == 2)
							ilosc_klatek++;
						ostatnia_pozycja = S;
						//predkosc_linia_prosta = 2;
						if (ilosc_klatek == 0)
						{
							al_draw_bitmap(Postac[0], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 0 && ilosc_klatek <= 10)
						{
							al_draw_bitmap(Postac[1], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 10 && ilosc_klatek <= 20)
						{
							al_draw_bitmap(Postac[2], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 20 && ilosc_klatek <= 30)
						{
							al_draw_bitmap(Postac[3], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 30 && ilosc_klatek <= 40)
						{
							al_draw_bitmap(Postac[4], pos_x, pos_y, 0);
						}
						if (ilosc_klatek > 40 && ilosc_klatek <= 50)
						{
							al_draw_bitmap(Postac[5], pos_x, pos_y, 0);
						}
						if (ilosc_klatek == 50)
						{
							ilosc_klatek = 0;
						}
					}
					if (wsad[W] == true && wsad[S] == true && wsad[D] == true && wsad[A] == true)
					{
						//predkosc_linia_prosta = 2;
						if (ostatnia_pozycja != S && ostatnia_pozycja != W && ostatnia_pozycja != D && ostatnia_pozycja != A)
							al_draw_bitmap(Postac[0], pos_x, pos_y, 0);
						if (ostatnia_pozycja == W)
							al_draw_bitmap(Postac[6], pos_x, pos_y, 0);
						if (ostatnia_pozycja == S)
							al_draw_bitmap(Postac[0], pos_x, pos_y, 0);
						if (ostatnia_pozycja == A)
							al_draw_bitmap(Postac[18], pos_x, pos_y, 0);
						if (ostatnia_pozycja == D)
							al_draw_bitmap(Postac[12], pos_x, pos_y, 0);
					}

				}
		}
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			DrawPocisk(pociski, ilosc_pociskow);
			
			DrawOrk(orkowie, ilosc_orkow, pomocnicza_do_sprite_orka);
			DrawSzaman(szamani, ilosc_szamanow, pomocnicza_do_sprite_szamana);
			DrawPociskSzamana(ilosc_szamanow,szamani);

			// wyswietlanie level up
			if ((level_up > 1) && (level_up < 150))
			{
				al_draw_bitmap(LevelUp, 455, 30, 0);
			}

			// wyswietlanie paremetrow postaci
			al_draw_bitmap(HP, 25, 15, 0);
			al_draw_textf(font24, al_map_rgb(255, 255, 255), 90, 25, ALLEGRO_ALIGN_LEFT,
				"%.0f", pos_hp);

			if (redraw == false)
			{
				al_draw_bitmap(EXP, 145, 15, 0);
				if (doswiadczenie < 100)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 225, 25, ALLEGRO_ALIGN_RIGHT,
						"%.0f", doswiadczenie);
				}
				if (doswiadczenie >= 100)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 235, 25, ALLEGRO_ALIGN_RIGHT,
						"%.0f", doswiadczenie);
				}

				al_draw_bitmap(Pieniadze, 255, 15, 0);
				if (pieniadze < 100)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 25, ALLEGRO_ALIGN_RIGHT,
						"%.0f", pieniadze);
				}
				if (pieniadze >= 100 && pieniadze < 1000)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 355, 25, ALLEGRO_ALIGN_RIGHT,
						"%.0f", pieniadze);
				}
				if (pieniadze >= 1000)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 365, 25, ALLEGRO_ALIGN_RIGHT,
						"%.0f", pieniadze);
				}

				// wyswietlanie czasu gry
				al_draw_bitmap(Zegar, 1125, 15, 0);
				if (czas < 100)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 1210, 25, ALLEGRO_ALIGN_RIGHT,
						"%.0f", czas);
				}
				if (czas >= 100)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 1220, 25, ALLEGRO_ALIGN_RIGHT,
						"%.0f", czas);
				}

				al_draw_bitmap(ZwojFireball, 25, 640, 0);
				al_draw_bitmap(Nieskonczonosc, 90, 653, 0);
				al_draw_bitmap(ZwojSopel, 135, 640, 0);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 220, 650, ALLEGRO_ALIGN_RIGHT,
					"%i", ilosc_sopel);
				al_draw_bitmap(ZwojTrucizna, 245, 640, 0);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 330, 650, ALLEGRO_ALIGN_RIGHT,
					"%i", ilosc_trucizna);
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}
	if (faza_gry == 3)
		{
				ALLEGRO_EVENT ev3;
				al_wait_for_event(event_queue, &ev3);
				
				if (ev3.type == ALLEGRO_EVENT_KEY_UP)
				{
					switch (ev3.keyboard.keycode)
					{
					case ALLEGRO_KEY_ESCAPE:
					{
						faza_gry = 1;
					}
					}
				}
				else if (ev3.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
				{
					kursor_x3 = ev3.mouse.x;
					kursor_y3 = ev3.mouse.y;
				}
				else if (ev3.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				{
					int wyjsc_z_gry = al_show_native_message_box
					(
						okno,
						"OrcVille",
						"Wyjsc z gry??",
						NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO
					);
					if (wyjsc_z_gry == 1)
						done = true;

					done = true;
				}

			if ((kursor_x3 > 384) && (kursor_x3 < 873) && (kursor_y3 > 395) &&( kursor_y3 < 508))
			{
				faza_gry = 1;
				kursor_x3 = -100;
				kursor_y3 = -100;
			}
			al_draw_bitmap(Jak_grac, 0, 0, 0);
			al_flip_display();
			
		}
	if (faza_gry == 4)
		{
			ALLEGRO_EVENT ev4;
			al_wait_for_event(event_queue, &ev4);
			if (ev4.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch (ev4.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
				{
					faza_gry = 2;
				}
				}
			}
			if (ev4.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch (ev4.keyboard.keycode)
				{
				case ALLEGRO_KEY_SPACE:
				{
					faza_gry = 2;
					break;
				}
				}
			}
			else if (ev4.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				kursor_x4 = ev4.mouse.x;
				kursor_y4 = ev4.mouse.y;
			}
			else if (ev4.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				int wyjsc_z_gry = al_show_native_message_box
				(
					okno,
					"OrcVille",
					"Wyjsc z gry??",
					NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO
				);
				if (wyjsc_z_gry == 1)
					done = true;

				done = true;
			}
			else if (ev4.type == ALLEGRO_EVENT_TIMER)
			{
				if (pieniadze >= cena_sopla)
				{
					if ((kursor_x4 > 734) && (kursor_x4 < 858) && (kursor_y4 > 240) && (kursor_y4 < 300))
					{
						ilosc_sopel += 1;
						pieniadze -= cena_sopla;
						kursor_x4 = -100;
						kursor_y4 = -100;
					}
				}
				if (pieniadze >= cena_sopla5)
				{
					if ((kursor_x4 > 734) && (kursor_x4 < 858) && (kursor_y4 > 347) && (kursor_y4 < 402))
					{
						ilosc_sopel += 5;
						pieniadze -= cena_sopla5;
						kursor_x4 = -100;
						kursor_y4 = -100;
					}
				}
				if (pieniadze >= cena_sopla10)
				{
					if ((kursor_x4 > 734) && (kursor_x4 < 858) && (kursor_y4 > 455) && (kursor_y4 < 508))
					{
						ilosc_sopel += 10;
						pieniadze -= cena_sopla10;
						kursor_x4 = -100;
						kursor_y4 = -100;
					}
				}
				if (pieniadze >= cena_trucizny)
				{
					if ((kursor_x4 > 1012) && (kursor_x4 < 1140) && (kursor_y4 > 241) && (kursor_y4 < 300))
					{
						ilosc_trucizna += 1;
						pieniadze -= cena_trucizny;
						kursor_x4 = -100;
						kursor_y4 = -100;
					}
				}
				if (pieniadze >= cena_trucizny5)
				{
					if ((kursor_x4 > 1012) && (kursor_x4 < 1140) && (kursor_y4 > 347) && (kursor_y4 < 403))
					{
						ilosc_trucizna += 5;
						pieniadze -= cena_trucizny5;
						kursor_x4 = -100;
						kursor_y4 = -100;
					}
				}
				if (pieniadze >= cena_trucizny10)
				{
					if ((kursor_x4 > 1012) && (kursor_x4 < 1140) && (kursor_y4 > 455) && (kursor_y4 < 507))
					{
						ilosc_trucizna += 10;
						pieniadze -= cena_trucizny10;
						kursor_x4 = -100;
						kursor_y4 = -100;
					}
				}



				if ((kursor_x4 > 735) && (kursor_x4 < 855) && (kursor_y4 > 561) && (kursor_y4 < 613))
				{
					czy_ma_wyswietlac1 = true;
					czy_ma_wyswietlac2 = false;
					kursor_x4 = -100;
					kursor_y4 = -100;
				}
				if ((kursor_x4 > 1010) && (kursor_x4 < 1135) && (kursor_y4 > 560) && (kursor_y4 < 615))
				{
					czy_ma_wyswietlac2 = true;
					czy_ma_wyswietlac1 = false;
					kursor_x4 = -100;
					kursor_y4 = -100;
				}
				redraw = true;
			}
			if (redraw && al_is_event_queue_empty(event_queue))
			{
				redraw = false;

				al_draw_bitmap(Sklep, 0, 0, 0);
				if (czy_ma_wyswietlac1 == true && czy_ma_wyswietlac2 == false)
					al_draw_bitmap(Pocisk_lodu, 45, 305, 0);
				if (czy_ma_wyswietlac2 == true && czy_ma_wyswietlac1 == false)
					al_draw_bitmap(Pocisk_trucizny, 45, 305, 0);
				if (redraw == false)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 145, 110, ALLEGRO_ALIGN_LEFT,
					"%.0f", pos_hp);

					if (pieniadze < 100)
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 160, 190, ALLEGRO_ALIGN_RIGHT,
							"%.0f", pieniadze);
					}
					if (pieniadze >= 100 && pieniadze < 1000)
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 180, 190, ALLEGRO_ALIGN_RIGHT,
							"%.0f", pieniadze);
					}
					if (pieniadze >= 1000)
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 190, 190, ALLEGRO_ALIGN_RIGHT,
							"%.0f", pieniadze);
					}
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 390, 110, ALLEGRO_ALIGN_RIGHT,
						"%i", ilosc_sopel);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 390, 185, ALLEGRO_ALIGN_RIGHT,
						"%i", ilosc_trucizna);


					al_draw_textf(font24, al_map_rgb(255, 255, 255), 812, 302, ALLEGRO_ALIGN_RIGHT,
						"%.0f", cena_sopla);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 812, 407, ALLEGRO_ALIGN_RIGHT,
						"%.0f", cena_sopla5);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 820, 512, ALLEGRO_ALIGN_RIGHT,
						"%.0f", cena_sopla10);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 1092, 302, ALLEGRO_ALIGN_RIGHT,
						"%.0f", cena_trucizny);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 1092, 407, ALLEGRO_ALIGN_RIGHT,
						"%.0f", cena_trucizny5);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 1100, 512, ALLEGRO_ALIGN_RIGHT,
						"%.0f", cena_trucizny10);









				}
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
			}
		}
	if (faza_gry == 5)
		{
			ALLEGRO_EVENT ev5;
			al_wait_for_event(event_queue, &ev5);
			if (ev5.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch (ev5.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
				{
					int wyjsc_z_gry = al_show_native_message_box
					(
						okno,
						"OrcVille",
						"Wyjsc z gry??",
						NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO
					);
					if (wyjsc_z_gry == 1)
						done = true;

				}
				}
			}
			else if (ev5.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				kursor_x5 = ev5.mouse.x;
				kursor_y5 = ev5.mouse.y;
			}
			else if (ev5.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				int wyjsc_z_gry = al_show_native_message_box
				(
					okno,
					"OrcVille",
					"Wyjsc z gry??",
					NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO
				);
				if (wyjsc_z_gry == 1)
					done = true;

			}
			else if (ev5.type == ALLEGRO_EVENT_TIMER)
			{
				if (kursor_x5 > 384 && kursor_x5 < 863 && kursor_y5 > 350 && kursor_y5 < 498)
				{
					kursor_x5 = -100;
					kursor_y5 = -100;
					int wyjsc_z_gry = al_show_native_message_box
					(
						okno,
						"OrcVille",
						"Wyjsc z gry??",
						NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO
					);
					if (wyjsc_z_gry == 1)
						done = true;

				}

				redraw = true;
			}
			if (redraw && al_is_event_queue_empty(event_queue))
			{
				redraw = false;
				al_draw_bitmap(Wynik, 0, 0, 0);
				if (redraw == false)
				{
					if (score < 100)
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 620, 206, ALLEGRO_ALIGN_RIGHT,
							"%.0f", score);
					if (score < 1000 && score >= 100)
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 630, 206, ALLEGRO_ALIGN_RIGHT,
							"%.0f", score);
					if (score >=1000)
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 640, 206, ALLEGRO_ALIGN_RIGHT,
							"%.0f", score);
				}
			}
				al_flip_display();
			}
		
	}

	al_destroy_event_queue(event_queue);
	al_destroy_display(okno);
	al_destroy_timer(timer);
	al_destroy_font(font24);
	al_destroy_bitmap(Ekran_startowy);

	for (int i = 0; i < 48; i++)
	{
		al_destroy_bitmap(Postac[i]);
	}
	al_destroy_bitmap(bgImage);
	return 0;
	
}

void KtoryXY(float pos_x, float pos_y, float mapa_x, float mapa_y, int & ktoryX, int & ktoryY)
{
	//w jakiej pozycji siatki jestem na X
	for (int i = 0; i < 30; i++)
	{
		int pozycja = pos_x - mapa_x;
		if ((pozycja >= SiatkaX[i]) && (pozycja <= SiatkaX[i + 1]))
		{
			ktoryX = i;
			break;
		}
	}

	//w jakiej pozycji siatki jestem na Y
	for (int i = 0; i < 30; i++)
	{
		int pozycja = pos_y - mapa_y;
		if ((pozycja >= (SiatkaY[i])) && (pozycja <= (SiatkaY[i + 1])))
		{
			ktoryY = i;
			break;
		}
	}
}
void RuchPostaciW(float pos_x, float & pos_y, int ktoryX, int ktoryY, float predkosc_postaci)
{
	float gdzie_bedeY = pos_y - predkosc_postaci;
	bool obszar_mapy = ((gdzie_bedeY) >0) ? 1 : 0;

	if ((czy_mozna_wejsc[ktoryX][ktoryY] == false) && (czy_mozna_wejsc[ktoryX + 1][ktoryY] == false) && (obszar_mapy == true))
	{
		if ((pos_x >= (ktoryX * 50)) && (pos_x <= ((ktoryX + 2) * 50)))
		{
			pos_y += 0;
		}
	}
	else if ((czy_mozna_wejsc[ktoryX][ktoryY] == false) && (obszar_mapy == true))
	{
		if (pos_x < (((ktoryX + 1) * 50)))
		{
			pos_y += 0;
		}
		else
		{
			pos_y -= predkosc_postaci;
		}
	}
	else if ((czy_mozna_wejsc[ktoryX + 1][ktoryY] == false) && (obszar_mapy == true))
	{
		if ((pos_x + 50 > (((ktoryX + 1) * 50))))
		{
			pos_y += 0;
		}
	}
	else if (obszar_mapy == true)
	{
		pos_y -= predkosc_postaci;
	}
	else if (obszar_mapy == false)
	{
		pos_y = 0;
	}
}
void RuchPostaciS(float pos_x, float & pos_y, int ktoryX, int ktoryY, float predkosc_postaci)
{
	float gdzie_bedeY = pos_y + predkosc_postaci;
	bool obszar_mapy = ((gdzie_bedeY) < (1500 - 50 + 1)) ? 1 : 0;

	if ((czy_mozna_wejsc[ktoryX][ktoryY + 1] == false) && (czy_mozna_wejsc[ktoryX + 1][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_x >= (ktoryX * 50)) && (pos_x <= ((ktoryX + 2) * 50)))
		{
			pos_y -= 0;
		}
	}
	else if ((czy_mozna_wejsc[ktoryX][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if (pos_x < (((ktoryX + 1) * 50)))
		{
			pos_y -= 0;
		}
		else
		{
			pos_y += predkosc_postaci;
		}

	}
	else if ((czy_mozna_wejsc[ktoryX + 1][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_x + 50) > (((ktoryX + 1) * 50)))
		{
			pos_y -= 0;
		}
	}
	else if (obszar_mapy == true)
	{
		pos_y += predkosc_postaci;
	}
	else if (obszar_mapy == false)
	{
		pos_y = 1500 - 50 + 1;
	}
}
void RuchPostaciA(float & pos_x, float pos_y, int ktoryX, int ktoryY, float predkosc_postaci)
{
	float gdzie_bedeY = pos_x - predkosc_postaci;
	bool obszar_mapy = ((gdzie_bedeY) > 0) ? 1 : 0;

	if ((czy_mozna_wejsc[ktoryX][ktoryY] == false) && (czy_mozna_wejsc[ktoryX][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_y >= (ktoryY * 50)) && (pos_y <= ((ktoryY + 2) * 50)))
		{
			pos_x += 0;
		}
	}
	else if ((czy_mozna_wejsc[ktoryX][ktoryY] == false) && (obszar_mapy == true))
	{
		if (pos_y < (((ktoryY + 1) * 50)))
		{
			pos_x += 0;
		}
		else
		{
			pos_x -= predkosc_postaci;
		}

	}
	else if ((czy_mozna_wejsc[ktoryX][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_y + 50) > (((ktoryY + 1) * 50)))
		{
			pos_x += 0;
		}
	}
	else if (obszar_mapy == true)
	{
		pos_x -= predkosc_postaci;
	}
	else if (obszar_mapy == false)
		pos_x = 0;
}
void RuchPostaciD(float & pos_x, float pos_y, int ktoryX, int ktoryY, float predkosc_postaci)
{
	float gdzie_bedeY = pos_x + predkosc_postaci;
	bool obszar_mapy = ((gdzie_bedeY) < (1500 - 50 + 1)) ? 1 : 0;

	if ((czy_mozna_wejsc[ktoryX + 1][ktoryY] == false) && (czy_mozna_wejsc[ktoryX + 1][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_y >= (ktoryY * 50)) && (pos_y <= ((ktoryY + 2) * 50)))
		{
			pos_x -= 0;
		}
	}
	else if ((czy_mozna_wejsc[ktoryX + 1][ktoryY] == false) && (obszar_mapy == true))
	{
		if (pos_y < (((ktoryY + 1) * 50)))
		{
			pos_x -= 0;
		}
		else
		{
			pos_x += predkosc_postaci;
		}

	}
	else if ((czy_mozna_wejsc[ktoryX + 1][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_y + 50) > (((ktoryY + 1) * 50)))
		{
			pos_x -= 0;
		}
	}
	else if (obszar_mapy == true)
	{
		pos_x += predkosc_postaci;
	}
	else if (obszar_mapy == false)
		pos_x = 1500 - 50 + 1;
}

void RuchOrkaW(float pos_x, float & pos_y, int ktoryX, int ktoryY, float predkosc_postaci)
{
	float gdzie_bedeY = pos_y - predkosc_postaci;
	bool obszar_mapy = true;

	if ((czy_mozna_wejsc[ktoryX][ktoryY] == false) && (czy_mozna_wejsc[ktoryX + 1][ktoryY] == false) && (obszar_mapy == true))
	{
		if ((pos_x >= (ktoryX * 50)) && (pos_x <= ((ktoryX + 2) * 50)))
		{
			pos_y += 0;
		}
	}
	else if ((czy_mozna_wejsc[ktoryX][ktoryY] == false) && (obszar_mapy == true))
	{
		if (pos_x < (((ktoryX + 1) * 50)))
		{
			pos_y += 0;
		}
		else
		{
			pos_y -= predkosc_postaci;
		}
	}
	else if ((czy_mozna_wejsc[ktoryX + 1][ktoryY] == false) && (obszar_mapy == true))
	{
		if ((pos_x + 50 > (((ktoryX + 1) * 50))))
		{
			pos_y += 0;
		}
	}
	
	else if (obszar_mapy == true)
	{
		pos_y -= predkosc_postaci;
	}
	else if (obszar_mapy == false)
	{
		pos_y = 0;
	}
}
void RuchOrkaS(float pos_x, float & pos_y, int ktoryX, int ktoryY, float predkosc_postaci)
{

	float gdzie_bedeY = pos_y + predkosc_postaci;
	bool obszar_mapy = true;

	if ((czy_mozna_wejsc[ktoryX][ktoryY + 1] == false) && (czy_mozna_wejsc[ktoryX + 1][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_x >= (ktoryX * 50)) && (pos_x <= ((ktoryX + 2) * 50)))
		{
			pos_y -= 0;
		}
	}
	else if ((czy_mozna_wejsc[ktoryX][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if (pos_x < (((ktoryX + 1) * 50)))
		{
			pos_y -= 0;
		}
		else
		{
			pos_y += predkosc_postaci;
		}

	}
	else if ((czy_mozna_wejsc[ktoryX + 1][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_x + 50) > (((ktoryX + 1) * 50)))
		{
			pos_y -= 0;
		}
	}
	else if (obszar_mapy == true)
	{
		pos_y += predkosc_postaci;
	}
	else if (obszar_mapy == false)
	{
		pos_y = 1500 - 50 + 1;
	}
}
void RuchOrkaA(float & pos_x, float pos_y, int ktoryX, int ktoryY, float predkosc_postaci)
{

	float gdzie_bedeY = pos_x - predkosc_postaci;
	bool obszar_mapy = true;

	if ((czy_mozna_wejsc[ktoryX][ktoryY] == false) && (czy_mozna_wejsc[ktoryX][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_y >= (ktoryY * 50)) && (pos_y <= ((ktoryY + 2) * 50)))
		{
			pos_x += 0;
		}
	}
	else if ((czy_mozna_wejsc[ktoryX][ktoryY] == false) && (obszar_mapy == true))
	{
		if (pos_y < (((ktoryY + 1) * 50)))
		{
			pos_x += 0;
		}
		else
		{
			pos_x -= predkosc_postaci;
		}

	}
	else if ((czy_mozna_wejsc[ktoryX][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_y + 50) > (((ktoryY + 1) * 50)))
		{
			pos_x += 0;
		}
	}
	else if (obszar_mapy == true)
	{
		pos_x -= predkosc_postaci;
	}
	else if (obszar_mapy == false)
		pos_x = 0;
}
void RuchOrkaD(float & pos_x, float pos_y, int ktoryX, int ktoryY, float predkosc_postaci)
{
	float gdzie_bedeY = pos_x + predkosc_postaci;
	bool obszar_mapy = true;

	if ((czy_mozna_wejsc[ktoryX + 1][ktoryY] == false) && (czy_mozna_wejsc[ktoryX + 1][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_y >= (ktoryY * 50)) && (pos_y <= ((ktoryY + 2) * 50)))
		{
			pos_x -= 0;
		}
	}
	else if ((czy_mozna_wejsc[ktoryX + 1][ktoryY] == false) && (obszar_mapy == true))
	{
		if (pos_y < (((ktoryY + 1) * 50)))
		{
			pos_x -= 0;
		}
		else
		{
			pos_x += predkosc_postaci;
		}

	}
	else if ((czy_mozna_wejsc[ktoryX + 1][ktoryY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_y + 50) > (((ktoryY + 1) * 50)))
		{
			pos_x -= 0;
		}
	}
	else if (obszar_mapy == true)
	{
		pos_x += predkosc_postaci;
	}
	else if (obszar_mapy == false)
		pos_x = 1500 - 50 + 1;
}
void InitOrk(Ork orkowie[], int ile)
{
	for (int i = 0; i < ile; i++)
	{
		orkowie[i].ID = PRZECIWNIK;
		orkowie[i].live = false;
		orkowie[i].predkosc = 1;
		orkowie[i].boundx = 50;
		orkowie[i].boundy = 50;
		orkowie[i].a = rand() % 4;
		orkowie[i].ktoryX_ork;
		orkowie[i].ktoryY_ork;
		orkowie[i].hp = 100;
		orkowie[i].efekt_pocisku_sopel =false;
		orkowie[i].efekt_pocisku_trucizna=false;
		orkowie[i].pomocnicza_do_efektu = 0;
		orkowie[i].efekt_krwi = false;
		orkowie[i].pomocnicza_do_krwi = 0;


		orkowie[i].OrkSprite[0] = al_load_bitmap("Ork/Ork w dol.png");
		orkowie[i].OrkSprite[1] = al_load_bitmap("Ork/Ork w dol 1.png");
		orkowie[i].OrkSprite[2] = al_load_bitmap("Ork/Ork w dol 2.png");
		orkowie[i].OrkSprite[3] = al_load_bitmap("Ork/Ork w dol.png");
		orkowie[i].OrkSprite[4] = al_load_bitmap("Ork/Ork w dol 3.png");
		orkowie[i].OrkSprite[5] = al_load_bitmap("Ork/Ork w dol 4.png");

		orkowie[i].OrkSprite[6] = al_load_bitmap("Ork/Ork w gore.png");
		orkowie[i].OrkSprite[7] = al_load_bitmap("Ork/Ork w gore 1.png");
		orkowie[i].OrkSprite[8] = al_load_bitmap("Ork/Ork w gore 2.png");
		orkowie[i].OrkSprite[9] = al_load_bitmap("Ork/Ork w gore.png");
		orkowie[i].OrkSprite[10] = al_load_bitmap("Ork/Ork w gore 3.png");
		orkowie[i].OrkSprite[11] = al_load_bitmap("Ork/Ork w gore 4.png");

		orkowie[i].OrkSprite[12] = al_load_bitmap("Ork/Ork w prawo.png");
		orkowie[i].OrkSprite[13] = al_load_bitmap("Ork/Ork w prawo 1.png");
		orkowie[i].OrkSprite[14] = al_load_bitmap("Ork/Ork w prawo 2.png");
		orkowie[i].OrkSprite[15] = al_load_bitmap("Ork/Ork w prawo.png");
		orkowie[i].OrkSprite[16] = al_load_bitmap("Ork/Ork w prawo 3.png");
		orkowie[i].OrkSprite[17] = al_load_bitmap("Ork/Ork w prawo 4.png");

		orkowie[i].OrkSprite[18] = al_load_bitmap("Ork/Ork w lewo.png");
		orkowie[i].OrkSprite[19] = al_load_bitmap("Ork/Ork w lewo 1.png");
		orkowie[i].OrkSprite[20] = al_load_bitmap("Ork/Ork w lewo 2.png");
		orkowie[i].OrkSprite[21] = al_load_bitmap("Ork/Ork w lewo.png");
		orkowie[i].OrkSprite[22] = al_load_bitmap("Ork/Ork w lewo 3.png");
		orkowie[i].OrkSprite[23] = al_load_bitmap("Ork/Ork w lewo 4.png");

		orkowie[i].OrkSprite[24] = al_load_bitmap("Ork/Ork w dol prawo.png");
		orkowie[i].OrkSprite[25] = al_load_bitmap("Ork/Ork w dol prawo 1.png");
		orkowie[i].OrkSprite[26] = al_load_bitmap("Ork/Ork w dol prawo 2.png");
		orkowie[i].OrkSprite[27] = al_load_bitmap("Ork/Ork w dol prawo.png");
		orkowie[i].OrkSprite[28] = al_load_bitmap("Ork/Ork w dol prawo 3.png");
		orkowie[i].OrkSprite[29] = al_load_bitmap("Ork/Ork w dol prawo 4.png");

		orkowie[i].OrkSprite[30] = al_load_bitmap("Ork/Ork w dol lewo.png");
		orkowie[i].OrkSprite[31] = al_load_bitmap("Ork/Ork w dol lewo 1.png");
		orkowie[i].OrkSprite[32] = al_load_bitmap("Ork/Ork w dol lewo 2.png");
		orkowie[i].OrkSprite[33] = al_load_bitmap("Ork/Ork w dol lewo.png");
		orkowie[i].OrkSprite[34] = al_load_bitmap("Ork/Ork w dol lewo 3.png");
		orkowie[i].OrkSprite[35] = al_load_bitmap("Ork/Ork w dol lewo 4.png");

		orkowie[i].OrkSprite[36] = al_load_bitmap("Ork/Ork w gore prawo.png");
		orkowie[i].OrkSprite[37] = al_load_bitmap("Ork/Ork w gore prawo 1.png");
		orkowie[i].OrkSprite[38] = al_load_bitmap("Ork/Ork w gore prawo 2.png");
		orkowie[i].OrkSprite[39] = al_load_bitmap("Ork/Ork w gore prawo.png");
		orkowie[i].OrkSprite[40] = al_load_bitmap("Ork/Ork w gore prawo 3.png");
		orkowie[i].OrkSprite[41] = al_load_bitmap("Ork/Ork w gore prawo 4.png");

		orkowie[i].OrkSprite[42] = al_load_bitmap("Ork/Ork w gore lewo.png");
		orkowie[i].OrkSprite[43] = al_load_bitmap("Ork/Ork w gore lewo 1.png");
		orkowie[i].OrkSprite[44] = al_load_bitmap("Ork/Ork w gore lewo 2.png");
		orkowie[i].OrkSprite[45] = al_load_bitmap("Ork/Ork w gore lewo.png");
		orkowie[i].OrkSprite[46] = al_load_bitmap("Ork/Ork w gore lewo 3.png");
		orkowie[i].OrkSprite[47] = al_load_bitmap("Ork/Ork w gore lewo 4.png");

		orkowie[i].OrkSprite[48] = al_load_bitmap("Krew/Krew1.png");
		orkowie[i].OrkSprite[49] = al_load_bitmap("Krew/Krew2.png");
		orkowie[i].OrkSprite[50] = al_load_bitmap("Krew/Krew3.png");

		for (int j = 0; j < 48; j++)
		{
			al_convert_mask_to_alpha(orkowie[i].OrkSprite[j], al_map_rgb(255, 100, 255));
		}
	}
}
void DrawOrk(Ork orkowie[], int ile, int pomocnicza_do_sprite_orka)
{

	for (int i = 0; i < ile; i++)
	{
		if (orkowie[i].live)
		{
				//sprite Orki
				if (PozycjaOrka[i] == Gora)
				{
					if (pomocnicza_do_sprite_orka == 0)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[6], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 0 && pomocnicza_do_sprite_orka <= 10)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[7], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 10 && pomocnicza_do_sprite_orka <= 20)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[8], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 20 && pomocnicza_do_sprite_orka <= 30)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[9], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 30 && pomocnicza_do_sprite_orka <= 40)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[10], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 40 && pomocnicza_do_sprite_orka <= 50)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[11], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka == 50)
					{
						pomocnicza_do_sprite_orka = 0;
					}
				}
				if (PozycjaOrka[i] == Dol)
				{
					if (pomocnicza_do_sprite_orka == 0)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[0], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 0 && pomocnicza_do_sprite_orka <= 10)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[1], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 10 && pomocnicza_do_sprite_orka <= 20)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[2], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 20 && pomocnicza_do_sprite_orka <= 30)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[3], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 30 && pomocnicza_do_sprite_orka <= 40)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[4], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 40 && pomocnicza_do_sprite_orka <= 50)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[5], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka == 50)
					{
						pomocnicza_do_sprite_orka = 0;
					}
				}
				if (PozycjaOrka[i] == Prawo)
				{
					if (pomocnicza_do_sprite_orka == 0)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[12], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 0 && pomocnicza_do_sprite_orka <= 10)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[13], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 10 && pomocnicza_do_sprite_orka <= 20)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[14], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 20 && pomocnicza_do_sprite_orka <= 30)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[15], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 30 && pomocnicza_do_sprite_orka <= 40)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[16], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 40 && pomocnicza_do_sprite_orka <= 50)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[17], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka == 50)
					{
						pomocnicza_do_sprite_orka = 0;
					}
				}
				if (PozycjaOrka[i] == Lewo)
				{
					if (pomocnicza_do_sprite_orka == 0)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[18], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 0 && pomocnicza_do_sprite_orka <= 10)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[19], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 10 && pomocnicza_do_sprite_orka <= 20)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[20], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 20 && pomocnicza_do_sprite_orka <= 30)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[21], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 30 && pomocnicza_do_sprite_orka <= 40)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[22], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 40 && pomocnicza_do_sprite_orka <= 50)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[23], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka == 50)
					{
						pomocnicza_do_sprite_orka = 0;
					}
				}
				if (PozycjaOrka[i] == PrawoDol)
				{
					if (pomocnicza_do_sprite_orka == 0)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[24], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 0 && pomocnicza_do_sprite_orka <= 10)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[25], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 10 && pomocnicza_do_sprite_orka <= 20)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[26], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 20 && pomocnicza_do_sprite_orka <= 30)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[27], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 30 && pomocnicza_do_sprite_orka <= 40)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[28], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 40 && pomocnicza_do_sprite_orka <= 50)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[29], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka == 50)
					{
						pomocnicza_do_sprite_orka = 0;
					}
				}
				if (PozycjaOrka[i] == LewoDol)
				{
					if (pomocnicza_do_sprite_orka == 0)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[30], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 0 && pomocnicza_do_sprite_orka <= 10)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[31], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 10 && pomocnicza_do_sprite_orka <= 20)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[32], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 20 && pomocnicza_do_sprite_orka <= 30)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[33], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 30 && pomocnicza_do_sprite_orka <= 40)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[34], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 40 && pomocnicza_do_sprite_orka <= 50)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[35], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka == 50)
					{
						pomocnicza_do_sprite_orka = 0;
					}
				}
				if (PozycjaOrka[i] == PrawoGora)
				{
					if (pomocnicza_do_sprite_orka == 0)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[36], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 0 && pomocnicza_do_sprite_orka <= 10)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[37], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 10 && pomocnicza_do_sprite_orka <= 20)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[38], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 20 && pomocnicza_do_sprite_orka <= 30)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[39], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 30 && pomocnicza_do_sprite_orka <= 40)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[40], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 40 && pomocnicza_do_sprite_orka <= 50)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[41], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka == 50)
					{
						pomocnicza_do_sprite_orka = 0;
					}
				}
				if (PozycjaOrka[i] == LewoGora)
				{
					if (pomocnicza_do_sprite_orka == 0)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[42], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 0 && pomocnicza_do_sprite_orka <= 10)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[43], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 10 && pomocnicza_do_sprite_orka <= 20)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[44], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 20 && pomocnicza_do_sprite_orka <= 30)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[45], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 30 && pomocnicza_do_sprite_orka <= 40)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[46], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka > 40 && pomocnicza_do_sprite_orka <= 50)
					{
						al_draw_bitmap(orkowie[i].OrkSprite[47], orkowie[i].x, orkowie[i].y, 0);
					}
					if (pomocnicza_do_sprite_orka == 50)
					{
						pomocnicza_do_sprite_orka = 0;
					}
				}
			
		}
			if (orkowie[i].efekt_krwi == true)
			{
				if (orkowie[i].pomocnicza_do_krwi <= 30 && faza_gry == 2)
				{
					orkowie[i].pomocnicza_do_krwi++;
				}
				if (orkowie[i].pomocnicza_do_krwi == 30)
				{
					orkowie[i].efekt_krwi = false;
					orkowie[i].pomocnicza_do_krwi = 0;
				}
				if (orkowie[i].pomocnicza_do_krwi >= 1 && orkowie[i].pomocnicza_do_krwi <= 10)
				{
					al_draw_bitmap(orkowie[i].OrkSprite[48], orkowie[i].x, orkowie[i].y, 0);
				}
				if (orkowie[i].pomocnicza_do_krwi > 10 && orkowie[i].pomocnicza_do_krwi <= 20)
				{
					al_draw_bitmap(orkowie[i].OrkSprite[49], orkowie[i].x, orkowie[i].y, 0);
				}
				if (orkowie[i].pomocnicza_do_krwi > 20 && orkowie[i].pomocnicza_do_krwi <= 29)
				{
					al_draw_bitmap(orkowie[i].OrkSprite[50], orkowie[i].x, orkowie[i].y, 0);
				}
			}
		
	}
}
void StartOrk(Ork orkowie[], int ile)
{
	for (int i = 0; i < ile; i++)
	{
		//zeby nie zrespil sie zanim animacja krwi sie skonczy
		if (orkowie[i].pomocnicza_do_krwi == 0)
		{
			if (!orkowie[i].live)
			{

				if (rand() % losowanie_orka == 0)
				{
					if (orkowie[i].a == 0)
					{
						orkowie[i].live = true;
						if (mapa_x < 0)
						{
							orkowie[i].x = ((rand() % 3+2)*50) +mapa_x;
						}
						if (mapa_x == 0)
						{
							orkowie[i].x = ((rand() % 3 + 2) * 50);
						}
						if (mapa_y < 0)
						{
							orkowie[i].y = ((rand() % 2 + 3)*50) + mapa_y;
						}
						if (mapa_y == 0)
						{
							orkowie[i].y = ((rand() % 2 + 3) * 50);
						}
						KtoryXY(orkowie[i].x, orkowie[i].y, mapa_x, mapa_y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork);
						
					}
					else if (orkowie[i].a == 1)
					{
							orkowie[i].live = true;
						if (mapa_x<0)
						{
							orkowie[i].x = ((rand() % 3 + 23) * 50) + mapa_x;
						}
						if (mapa_x == 0)
						{
							orkowie[i].x = ((rand() % 3 + 23) * 50);
						}
						if (mapa_y < 0)
						{
							orkowie[i].y = ((rand() % 2 + 3) * 50) + mapa_y;
						}
						if (mapa_y == 0)
						{
							orkowie[i].y = ((rand() % 2 + 3) * 50);
						}
						KtoryXY(orkowie[i].x, orkowie[i].y, mapa_x, mapa_y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork);
					}
					else if (orkowie[i].a == 2)
					{
						orkowie[i].live = true;
						if (mapa_x < 0)
						{
							orkowie[i].x = ((rand() % 3 + 3) * 50) + mapa_x;
						}
						if (mapa_x == 0)
						{
							orkowie[i].x = ((rand() % 3 + 3) * 50);
						}
						if (mapa_y > -800)
						{
							orkowie[i].y = ((rand() % 2 + 23) * 50) + mapa_y;
						}
						if (mapa_y == -800)
						{
							orkowie[i].y = ((rand() % 2 + 8) * 50);
						}
						KtoryXY(orkowie[i].x, orkowie[i].y, mapa_x, mapa_y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork);
					}
					else if (orkowie[i].a == 3)
					{
						orkowie[i].live = true;
						if (mapa_x > -250)
						{
							orkowie[i].x = ((rand() % 3 + 26) * 50) + mapa_x;
						}
						if (mapa_x == -250)
						{
							orkowie[i].x = ((rand() % 3 + 18) * 50);
						}
						if (mapa_y > -800)
						{
							orkowie[i].y = ((rand() % 2 + 24) * 50) + mapa_y;
						}
						if (mapa_y == -800)
						{
							orkowie[i].y = ((rand() % 2 + 9) * 50);
						}
						KtoryXY(orkowie[i].x, orkowie[i].y, mapa_x, mapa_y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork);
					}
					break;
				}
			}
		}
	}
}
void RuchOrk(Ork orkowie[], int ile)
{
	for (int i = 0; i < ile; i++)
	{
		if (orkowie[i].live)
		{
			Sciezka(pos_x, pos_y, orkowie[i].x, orkowie[i].y, orkowie[i].predkosc, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork, i);
			KtoryXY(orkowie[i].x, orkowie[i].y, mapa_x, mapa_y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork);
			RuchOrkaW(orkowie[i].x, orkowie[i].y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork, orkowie[i].predkosc);
			RuchOrkaS(orkowie[i].x, orkowie[i].y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork, orkowie[i].predkosc);
			RuchOrkaA(orkowie[i].x, orkowie[i].y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork, orkowie[i].predkosc);
			RuchOrkaD(orkowie[i].x, orkowie[i].y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork, orkowie[i].predkosc);
			
		}
	}
}
void Sciezka(float pos_x, float pos_y, float &  przeciwnik_x, float & przeciwnik_y, float predkosc_orka, int ktoryX_ork, int ktoryY_ork, int ile)
{
	//  w prawo
	if (przeciwnik_x < pos_x && przeciwnik_y == pos_y)
	{
		przeciwnik_x += predkosc_orka;
		PozycjaOrka[ile] = Prawo;
	}// w lewo
	if (przeciwnik_x > pos_x && przeciwnik_y == pos_y)
	{
		przeciwnik_x -= predkosc_orka;
		PozycjaOrka[ile] = Lewo;
	}// do dolu
	if (przeciwnik_y < pos_y && przeciwnik_x == pos_x)
	{
		przeciwnik_y += predkosc_orka;
		PozycjaOrka[ile] = Dol;
	}// do gory
	if (przeciwnik_y > pos_y && przeciwnik_x == pos_x)
	{
		przeciwnik_y -= predkosc_orka;
		PozycjaOrka[ile] = Gora;
	}// w lewo gore
	if (przeciwnik_y > pos_y && przeciwnik_x > pos_x)
	{
		przeciwnik_x -= predkosc_orka;
		przeciwnik_y -= predkosc_orka;
		PozycjaOrka[ile] = LewoGora;
	}// w lewo dol
	if (przeciwnik_y < pos_y && przeciwnik_x > pos_x)
	{
		przeciwnik_x -= predkosc_orka;
		przeciwnik_y += predkosc_orka;
		PozycjaOrka[ile] = LewoDol;
	}//w prawo gore
	if (przeciwnik_y > pos_y && przeciwnik_x < pos_x)
	{
		przeciwnik_x += predkosc_orka;
		przeciwnik_y -= predkosc_orka;
		PozycjaOrka[ile] = PrawoGora;
	}// w prawo dol
	if (przeciwnik_y < pos_y && przeciwnik_x < pos_x)
	{
		przeciwnik_x += predkosc_orka;
		przeciwnik_y += predkosc_orka;
		PozycjaOrka[ile] = PrawoDol;
	}
	if (przeciwnik_x == pos_x && przeciwnik_y == pos_y)
	{
		PozycjaOrka[ile] = Dol;
	}
}

void InitPocisk(Pocisk pociski[], int ile)
{
	for (int i = 0; i < ile; i++)
	{
		pociski[i].ID = POCISK;
		pociski[i].live = false;
		pociski[i].predkosc = 6.5;
		pociski[i].ktoryX_pocisk;
		pociski[i].ktoryY_pocisk;

		pociski[i].pocisk[0] = al_load_bitmap("Fireball/Fireball w dol.png");
		pociski[i].pocisk[1] = al_load_bitmap("Fireball/Fireball w gore.png");
		pociski[i].pocisk[2] = al_load_bitmap("Fireball/Fireball w prawo.png");
		pociski[i].pocisk[3] = al_load_bitmap("Fireball/Fireball w lewo.png");

		pociski[i].pocisk[4] = al_load_bitmap("Fireball/Fireball w lewo dol 1.png");
		pociski[i].pocisk[5] = al_load_bitmap("Fireball/Fireball w lewo dol 2.png");

		pociski[i].pocisk[6] = al_load_bitmap("Fireball/Fireball w prawo dol 1.png");
		pociski[i].pocisk[7] = al_load_bitmap("Fireball/Fireball w prawo dol 2.png");

		pociski[i].pocisk[8] = al_load_bitmap("Fireball/Fireball w lewo gore 1.png");
		pociski[i].pocisk[9] = al_load_bitmap("Fireball/Fireball w lewo gore 2.png");

		pociski[i].pocisk[10] = al_load_bitmap("Fireball/Fireball w prawo gore 1.png");
		pociski[i].pocisk[11] = al_load_bitmap("Fireball/Fireball w prawo gore 2.png");



		pociski[i].pocisk[12] = al_load_bitmap("Sopel/Sopel w dol.png");
		pociski[i].pocisk[13] = al_load_bitmap("Sopel/Sopel w gore.png");
		pociski[i].pocisk[14] = al_load_bitmap("Sopel/Sopel w prawo.png");
		pociski[i].pocisk[15] = al_load_bitmap("Sopel/Sopel w lewo.png");

		pociski[i].pocisk[16] = al_load_bitmap("Sopel/Sopel w lewo dol 1.png");
		pociski[i].pocisk[17] = al_load_bitmap("Sopel/Sopel w lewo dol 2.png");

		pociski[i].pocisk[18] = al_load_bitmap("Sopel/Sopel w prawo dol 1.png");
		pociski[i].pocisk[19] = al_load_bitmap("Sopel/Sopel w prawo dol 2.png");

		pociski[i].pocisk[20] = al_load_bitmap("Sopel/Sopel w lewo gore 1.png");
		pociski[i].pocisk[21] = al_load_bitmap("Sopel/Sopel w lewo gore 2.png");

		pociski[i].pocisk[22] = al_load_bitmap("Sopel/Sopel w prawo gore 1.png");
		pociski[i].pocisk[23] = al_load_bitmap("Sopel/Sopel w prawo gore 2.png");




		pociski[i].pocisk[24] = al_load_bitmap("Trucizna/Trucizna w dol.png");
		pociski[i].pocisk[25] = al_load_bitmap("Trucizna/Trucizna w gore.png");
		pociski[i].pocisk[26] = al_load_bitmap("Trucizna/Trucizna w prawo.png");
		pociski[i].pocisk[27] = al_load_bitmap("Trucizna/Trucizna w lewo.png");

		pociski[i].pocisk[28] = al_load_bitmap("Trucizna/Trucizna w lewo dol 1.png");
		pociski[i].pocisk[29] = al_load_bitmap("Trucizna/Trucizna w lewo dol 2.png");

		pociski[i].pocisk[30] = al_load_bitmap("Trucizna/Trucizna w prawo dol 1.png");
		pociski[i].pocisk[31] = al_load_bitmap("Trucizna/Trucizna w prawo dol 2.png");

		pociski[i].pocisk[32] = al_load_bitmap("Trucizna/Trucizna w lewo gore 1.png");
		pociski[i].pocisk[33] = al_load_bitmap("Trucizna/Trucizna w lewo gore 2.png");

		pociski[i].pocisk[34] = al_load_bitmap("Trucizna/Trucizna w prawo gore 1.png");
		pociski[i].pocisk[35] = al_load_bitmap("Trucizna/Trucizna w prawo gore 2.png");
	}
}
void DrawPocisk(Pocisk pociski[], int ile)
{
	for (int i = 0; i < ile; i++)
	{
		if (pociski[i].live)
		{

			// w lewo gore
			if (pociski[i].kierunek == 1)
			{
				if (pociski[i].rodzaj_pocisku == 1)
				{
					if (pociski[i].sinus >= 0 && pociski[i].sinus <= 0.1736)
						al_draw_bitmap(pociski[i].pocisk[3], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.9848 && pociski[i].sinus <= 1)
						al_draw_bitmap(pociski[i].pocisk[1], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.1736 && pociski[i].sinus <= 0.7071)
						al_draw_bitmap(pociski[i].pocisk[8], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.7071 && pociski[i].sinus < 0.9848)
						al_draw_bitmap(pociski[i].pocisk[9], pociski[i].x, pociski[i].y, 0);
				}
				if (pociski[i].rodzaj_pocisku == 2)
				{
					if (pociski[i].sinus >= 0 && pociski[i].sinus <= 0.1736)
						al_draw_bitmap(pociski[i].pocisk[15], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.9848 && pociski[i].sinus <= 1)
						al_draw_bitmap(pociski[i].pocisk[13], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.1736 && pociski[i].sinus <= 0.7071)
						al_draw_bitmap(pociski[i].pocisk[20], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.7071 && pociski[i].sinus < 0.9848)
						al_draw_bitmap(pociski[i].pocisk[21], pociski[i].x, pociski[i].y, 0);
				}
				if (pociski[i].rodzaj_pocisku == 3)
				{
					if (pociski[i].sinus >= 0 && pociski[i].sinus <= 0.1736)
						al_draw_bitmap(pociski[i].pocisk[27], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.9848 && pociski[i].sinus <= 1)
						al_draw_bitmap(pociski[i].pocisk[25], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.1736 && pociski[i].sinus <= 0.7071)
						al_draw_bitmap(pociski[i].pocisk[32], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.7071 && pociski[i].sinus < 0.9848)
						al_draw_bitmap(pociski[i].pocisk[33], pociski[i].x, pociski[i].y, 0);
				}
			}

			// w lewo dol
			if (pociski[i].kierunek == 2)
			{
				if (pociski[i].rodzaj_pocisku == 1)
				{
					if (pociski[i].sinus >= 0 && pociski[i].sinus <= 0.1736)
						al_draw_bitmap(pociski[i].pocisk[3], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.9848 && pociski[i].sinus <= 1)
						al_draw_bitmap(pociski[i].pocisk[0], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.7071 && pociski[i].sinus <= 0.9848)
						al_draw_bitmap(pociski[i].pocisk[4], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.1736 && pociski[i].sinus < 0.7071)
						al_draw_bitmap(pociski[i].pocisk[5], pociski[i].x, pociski[i].y, 0);
				}
				if (pociski[i].rodzaj_pocisku == 2)
				{
					if (pociski[i].sinus >= 0 && pociski[i].sinus <= 0.1736)
						al_draw_bitmap(pociski[i].pocisk[15], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.9848 && pociski[i].sinus <= 1)
						al_draw_bitmap(pociski[i].pocisk[12], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.1736 && pociski[i].sinus <= 0.7071)
						al_draw_bitmap(pociski[i].pocisk[17], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.7071 && pociski[i].sinus < 0.9848)
						al_draw_bitmap(pociski[i].pocisk[16], pociski[i].x, pociski[i].y, 0);
				}
				if (pociski[i].rodzaj_pocisku == 3)
				{
					if (pociski[i].sinus >= 0 && pociski[i].sinus <= 0.1736)
						al_draw_bitmap(pociski[i].pocisk[27], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.9848 && pociski[i].sinus <= 1)
						al_draw_bitmap(pociski[i].pocisk[24], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.1736 && pociski[i].sinus <= 0.7071)
						al_draw_bitmap(pociski[i].pocisk[29], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.7071 && pociski[i].sinus < 0.9848)
						al_draw_bitmap(pociski[i].pocisk[28], pociski[i].x, pociski[i].y, 0);
				}
			}
			//w prawo gore
			if (pociski[i].kierunek == 3)
			{
				if (pociski[i].rodzaj_pocisku == 1)
				{
					if (pociski[i].sinus >= 0 && pociski[i].sinus <= 0.1736)
						al_draw_bitmap(pociski[i].pocisk[2], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.9848 && pociski[i].sinus <= 1)
						al_draw_bitmap(pociski[i].pocisk[1], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.7071 && pociski[i].sinus < 0.9848)
						al_draw_bitmap(pociski[i].pocisk[10], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.1736 && pociski[i].sinus < 0.7071)
						al_draw_bitmap(pociski[i].pocisk[11], pociski[i].x, pociski[i].y, 0);
				}
				if (pociski[i].rodzaj_pocisku == 2)
				{
					if (pociski[i].sinus >= 0 && pociski[i].sinus <= 0.1736)
						al_draw_bitmap(pociski[i].pocisk[14], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.9848 && pociski[i].sinus <= 1)
						al_draw_bitmap(pociski[i].pocisk[13], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.1736 && pociski[i].sinus <= 0.7071)
						al_draw_bitmap(pociski[i].pocisk[23], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.7071 && pociski[i].sinus < 0.9848)
						al_draw_bitmap(pociski[i].pocisk[22], pociski[i].x, pociski[i].y, 0);
				}
				if (pociski[i].rodzaj_pocisku == 3)
				{
					if (pociski[i].sinus >= 0 && pociski[i].sinus <= 0.1736)
						al_draw_bitmap(pociski[i].pocisk[26], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.9848 && pociski[i].sinus <= 1)
						al_draw_bitmap(pociski[i].pocisk[25], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.1736 && pociski[i].sinus <= 0.7071)
						al_draw_bitmap(pociski[i].pocisk[35], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.7071 && pociski[i].sinus < 0.9848)
						al_draw_bitmap(pociski[i].pocisk[34], pociski[i].x, pociski[i].y, 0);
				}
			}
			// w prawo dol
			if (pociski[i].kierunek == 4)
			{
				if (pociski[i].rodzaj_pocisku == 1)
				{
					if (pociski[i].sinus >= 0 && pociski[i].sinus <= 0.1736)
						al_draw_bitmap(pociski[i].pocisk[2], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.9848 && pociski[i].sinus <= 1)
						al_draw_bitmap(pociski[i].pocisk[0], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.1736 && pociski[i].sinus <= 0.7071)
						al_draw_bitmap(pociski[i].pocisk[6], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.7071 && pociski[i].sinus < 0.9848)
						al_draw_bitmap(pociski[i].pocisk[7], pociski[i].x, pociski[i].y, 0);
				}
				if (pociski[i].rodzaj_pocisku == 2)
				{
					if (pociski[i].sinus >= 0 && pociski[i].sinus <= 0.1736)
						al_draw_bitmap(pociski[i].pocisk[14], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.9848 && pociski[i].sinus <= 1)
						al_draw_bitmap(pociski[i].pocisk[12], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.1736 && pociski[i].sinus <= 0.7071)
						al_draw_bitmap(pociski[i].pocisk[18], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.7071 && pociski[i].sinus < 0.9848)
						al_draw_bitmap(pociski[i].pocisk[19], pociski[i].x, pociski[i].y, 0);
				}
				if (pociski[i].rodzaj_pocisku == 3)
				{
					if (pociski[i].sinus >= 0 && pociski[i].sinus <= 0.1736)
						al_draw_bitmap(pociski[i].pocisk[26], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus >= 0.9848 && pociski[i].sinus <= 1)
						al_draw_bitmap(pociski[i].pocisk[24], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.1736 && pociski[i].sinus <= 0.7071)
						al_draw_bitmap(pociski[i].pocisk[30], pociski[i].x, pociski[i].y, 0);
					if (pociski[i].sinus > 0.7071 && pociski[i].sinus < 0.9848)
						al_draw_bitmap(pociski[i].pocisk[31], pociski[i].x, pociski[i].y, 0);
				}
			}
		}
	}
}
void StartPocisk(Pocisk pociski[], int ile, float pos_x, float pos_y)
{
	for (int i = 0; i < ile; i++)
	{
		if (!pociski[i].live)
		{
			pociski[i].live = true;
			pociski[i].x = pos_x + 1;
			pociski[i].y = pos_y + 1;
			if (RodzajPocisku == 1)
			{
				pociski[i].rodzaj_pocisku = 1;
				ilosc_fireball -= 1;
			}
			else if (RodzajPocisku == 2)
			{
				pociski[i].rodzaj_pocisku = 2;
				ilosc_sopel -= 1;
			}
			else if (RodzajPocisku == 3)
			{
				pociski[i].rodzaj_pocisku = 3;
				ilosc_trucizna -= 1;
			}
			else
			{
				pociski[i].rodzaj_pocisku = 1;
			}

			
			
			


			// w lewo gore
			if (kursor_y <= pos_y + 24 && kursor_x <= pos_x + 24)
			{
				pociski[i].kierunek = 1;
			}// w lewo dol
			else if (kursor_y >= pos_y + 24 && kursor_x <= pos_x + 24)
			{
				pociski[i].kierunek = 2;
			}
			//w prawo gore
			else if (kursor_y <= pos_y + 24 && kursor_x >= pos_x + 24)
			{
				pociski[i].kierunek = 3;
			}
			// w prawo dol
			else if (kursor_y >= pos_y + 24 && kursor_x >= pos_x + 24)
			{
				pociski[i].kierunek = 4;
			}


			// strzelanie w 360 stopniach, obliczenie sinusa
			pociski[i].odleglosc_x = sqrtf(pow(((kursor_x - mapa_x) - (pos_x + 24 - mapa_x)), 2));
			pociski[i].odleglosc_y = sqrtf(pow(((kursor_y - mapa_y) - (pos_y + 24 - mapa_y)), 2));
			pociski[i].odleglosc_xy = sqrtf(pow(pociski[i].odleglosc_x, 2) + pow(pociski[i].odleglosc_y, 2));
			pociski[i].sinus = pociski[i].odleglosc_y / pociski[i].odleglosc_xy;
			pociski[i].cosinus = pociski[i].odleglosc_x / pociski[i].odleglosc_xy;
			break;
		}
	}
}
void RuchPocisk(Pocisk pociski[], int ile)
{
	for (int i = 0; i < ile; i++)
	{
		if (pociski[i].live)
		{
			if (pociski[i].x > szerokosc)
				pociski[i].live = false;
			if (pociski[i].x < 0)
				pociski[i].live = false;
			if (pociski[i].y > wysokosc)
				pociski[i].live = false;
			if (pociski[i].y < 0)
				pociski[i].live = false;

			KtoryXY(pociski[i].x + 10, pociski[i].y +10, mapa_x, mapa_y, pociski[i].ktoryX_pocisk, pociski[i].ktoryY_pocisk);

			// kolizja przeszkody
			if (pociski[i].live)
			{
				// W
				if ((czy_mozna_wejsc[pociski[i].ktoryX_pocisk][pociski[i].ktoryY_pocisk] == false) && (czy_mozna_wejsc[pociski[i].ktoryX_pocisk + 1][pociski[i].ktoryY_pocisk] == false))
				{
					if ((pociski[i].x >= (pociski[i].ktoryX_pocisk * 50)) && (pociski[i].x <= ((pociski[i].ktoryX_pocisk + 2) * 50)))
					{
						pociski[i].live = false;
					}
				}
				else if ((czy_mozna_wejsc[pociski[i].ktoryX_pocisk][pociski[i].ktoryY_pocisk] == false))
				{
					if (pociski[i].x  < (((pociski[i].ktoryX_pocisk + 1) * 50)))
					{
						pociski[i].live = false;
					}
				}
				else if ((czy_mozna_wejsc[pociski[i].ktoryX_pocisk + 1][pociski[i].ktoryY_pocisk] == false))
				{
					if ((pociski[i].x + 50 >(((pociski[i].ktoryX_pocisk + 1) * 50))))
					{
						pociski[i].live = false;
					}
				}
				// S
				if ((czy_mozna_wejsc[pociski[i].ktoryX_pocisk + 1][pociski[i].ktoryY_pocisk] == false) && (czy_mozna_wejsc[pociski[i].ktoryX_pocisk + 1][pociski[i].ktoryY_pocisk + 1] == false))
				{
					if ((pociski[i].y >= (pociski[i].ktoryY_pocisk * 50)) && (pociski[i].y <= ((pociski[i].ktoryY_pocisk + 2) * 50)))
					{
						pociski[i].live = false;
					}
				}
				else if ((czy_mozna_wejsc[pociski[i].ktoryX_pocisk + 1][pociski[i].ktoryY_pocisk] == false))
				{
					if (pociski[i].y  < (((pociski[i].ktoryY_pocisk + 1) * 50)))
					{
						pociski[i].live = false;
					}
				}
				else if ((czy_mozna_wejsc[pociski[i].ktoryX_pocisk + 1][pociski[i].ktoryY_pocisk + 1] == false))
				{
					if ((pociski[i].y + 50) >(((pociski[i].ktoryY_pocisk + 1) * 50)))
					{
						pociski[i].live = false;
					}
				}
				// A
				if ((czy_mozna_wejsc[pociski[i].ktoryX_pocisk][pociski[i].ktoryY_pocisk + 1] == false) && (czy_mozna_wejsc[pociski[i].ktoryX_pocisk + 1][pociski[i].ktoryY_pocisk + 1] == false))
				{
					if ((pociski[i].x >= (pociski[i].ktoryX_pocisk * 50)) && (pociski[i].x <= ((pociski[i].ktoryX_pocisk + 2) * 50)))
					{
						pociski[i].live = false;
					}
				}
				else if ((czy_mozna_wejsc[pociski[i].ktoryX_pocisk][pociski[i].ktoryY_pocisk + 1] == false))
				{
					if (pociski[i].x < (((pociski[i].ktoryX_pocisk + 1) * 50)))
					{
						pociski[i].live = false;
					}
				}
				else if ((czy_mozna_wejsc[pociski[i].ktoryX_pocisk + 1][pociski[i].ktoryY_pocisk + 1] == false))
				{
					if ((pociski[i].x + 50) >(((pociski[i].ktoryX_pocisk + 1) * 50)))
					{
						pociski[i].live = false;
					}
				}
				// D
				if ((czy_mozna_wejsc[pociski[i].ktoryX_pocisk][pociski[i].ktoryY_pocisk] == false) && (czy_mozna_wejsc[pociski[i].ktoryX_pocisk][pociski[i].ktoryY_pocisk + 1] == false))
				{
					if ((pociski[i].y >= (pociski[i].ktoryY_pocisk * 50)) && (pociski[i].y <= ((pociski[i].ktoryY_pocisk + 2) * 50)))
					{
						pociski[i].live = false;
					}
				}
				if ((czy_mozna_wejsc[pociski[i].ktoryX_pocisk][pociski[i].ktoryY_pocisk] == false))
				{
					if (pociski[i].y < (((pociski[i].ktoryY_pocisk + 1) * 50)))
					{
						pociski[i].live = false;
					}

				}
				if ((czy_mozna_wejsc[pociski[i].ktoryX_pocisk][pociski[i].ktoryY_pocisk + 1] == false))
				{
					if ((pociski[i].y + 50) >(((pociski[i].ktoryY_pocisk + 1) * 50)))
					{
						pociski[i].live = false;
					}
				}
			}


			// strzelanie w 360 stopniach
			if (pociski[i].live)
			{

				// w lewo gore
				if (pociski[i].kierunek == 1)
				{
					pociski[i].x -= pociski[i].cosinus*pociski[i].predkosc;
					pociski[i].y -= pociski[i].sinus*pociski[i].predkosc;
				}// w lewo dol
				if (pociski[i].kierunek == 2)
				{
					pociski[i].x -= pociski[i].cosinus*pociski[i].predkosc;
					pociski[i].y += pociski[i].sinus*pociski[i].predkosc;
				}
				//w prawo gore
				if (pociski[i].kierunek == 3)
				{
					pociski[i].x += pociski[i].cosinus*pociski[i].predkosc;
					pociski[i].y -= pociski[i].sinus*pociski[i].predkosc;
				}
				// w prawo dol
				if (pociski[i].kierunek == 4)
				{
					pociski[i].x += pociski[i].cosinus*pociski[i].predkosc;
					pociski[i].y += pociski[i].sinus*pociski[i].predkosc;
				}
			}
		}
	}
}

void InitSzaman(Szaman szamani[], int ile)
{
	for (int i = 0; i < ile; i++)
	{
		szamani[i].ID = SZAMAN;
		szamani[i].live = false;
		szamani[i].predkosc = 1;
		szamani[i].boundx = 50;
		szamani[i].boundy = 50;
		szamani[i].a = rand() % 4;
		szamani[i].ktoryX_szaman;
		szamani[i].ktoryY_szaman;
		szamani[i].hp = 120;
		szamani[i].efekt_pocisku_sopel = false;
		szamani[i].efekt_pocisku_trucizna = false;
		szamani[i].pomocnicza_do_efektu = 0;
		szamani[i].efekt_krwi = false;
		szamani[i].pomocnicza_do_krwi = 0;
		szamani[i].PozycjaSzamana = 3;
		szamani[i].pomocnicza_do_strzalu_szamana = 0;

		
		szamani[i].SzamanSprite[0] = al_load_bitmap("Szaman/szaman w dol.png");
		szamani[i].SzamanSprite[1] = al_load_bitmap("Szaman/szaman w dol 1.png");
		szamani[i].SzamanSprite[2] = al_load_bitmap("Szaman/szaman w dol 2.png");
		szamani[i].SzamanSprite[3] = al_load_bitmap("Szaman/szaman w dol.png");
		szamani[i].SzamanSprite[4] = al_load_bitmap("Szaman/szaman w dol 3.png");
		szamani[i].SzamanSprite[5] = al_load_bitmap("Szaman/szaman w dol 4.png");

		szamani[i].SzamanSprite[6] = al_load_bitmap("Szaman/szaman w gore.png");
		szamani[i].SzamanSprite[7] = al_load_bitmap("Szaman/szaman w gore 1.png");
		szamani[i].SzamanSprite[8] = al_load_bitmap("Szaman/szaman w gore 2.png");
		szamani[i].SzamanSprite[9] = al_load_bitmap("Szaman/szaman w gore.png");
		szamani[i].SzamanSprite[10] = al_load_bitmap("Szaman/szaman w gore 3.png");
		szamani[i].SzamanSprite[11] = al_load_bitmap("Szaman/szaman w gore 4.png");

		szamani[i].SzamanSprite[12] = al_load_bitmap("Szaman/szaman w prawo.png");
		szamani[i].SzamanSprite[13] = al_load_bitmap("Szaman/szaman w prawo 1.png");
		szamani[i].SzamanSprite[14] = al_load_bitmap("Szaman/szaman w prawo 2.png");
		szamani[i].SzamanSprite[15] = al_load_bitmap("Szaman/szaman w prawo.png");
		szamani[i].SzamanSprite[16] = al_load_bitmap("Szaman/szaman w prawo 3.png");
		szamani[i].SzamanSprite[17] = al_load_bitmap("Szaman/szaman w prawo 4.png");

		szamani[i].SzamanSprite[18] = al_load_bitmap("Szaman/szaman w lewo.png");
		szamani[i].SzamanSprite[19] = al_load_bitmap("Szaman/szaman w lewo 1.png");
		szamani[i].SzamanSprite[20] = al_load_bitmap("Szaman/szaman w lewo 2.png");
		szamani[i].SzamanSprite[21] = al_load_bitmap("Szaman/szaman w lewo.png");
		szamani[i].SzamanSprite[22] = al_load_bitmap("Szaman/szaman w lewo 3.png");
		szamani[i].SzamanSprite[23] = al_load_bitmap("Szaman/szaman w lewo 4.png");

		szamani[i].SzamanSprite[24] = al_load_bitmap("Szaman/szaman w dol prawo.png");
		szamani[i].SzamanSprite[25] = al_load_bitmap("Szaman/szaman w dol prawo 1.png");
		szamani[i].SzamanSprite[26] = al_load_bitmap("Szaman/szaman w dol prawo 2.png");
		szamani[i].SzamanSprite[27] = al_load_bitmap("Szaman/szaman w dol prawo.png");
		szamani[i].SzamanSprite[28] = al_load_bitmap("Szaman/szaman w dol prawo 3.png");
		szamani[i].SzamanSprite[29] = al_load_bitmap("Szaman/szaman w dol prawo 4.png");

		szamani[i].SzamanSprite[30] = al_load_bitmap("Szaman/szaman w dol lewo.png");
		szamani[i].SzamanSprite[31] = al_load_bitmap("Szaman/szaman w dol lewo 1.png");
		szamani[i].SzamanSprite[32] = al_load_bitmap("Szaman/szaman w dol lewo 2.png");
		szamani[i].SzamanSprite[33] = al_load_bitmap("Szaman/szaman w dol lewo.png");
		szamani[i].SzamanSprite[34] = al_load_bitmap("Szaman/szaman w dol lewo 3.png");
		szamani[i].SzamanSprite[35] = al_load_bitmap("Szaman/szaman w dol lewo 4.png");

		szamani[i].SzamanSprite[36] = al_load_bitmap("Szaman/szaman w gore prawo.png");
		szamani[i].SzamanSprite[37] = al_load_bitmap("Szaman/szaman w gore prawo 1.png");
		szamani[i].SzamanSprite[38] = al_load_bitmap("Szaman/szaman w gore prawo 2.png");
		szamani[i].SzamanSprite[39] = al_load_bitmap("Szaman/szaman w gore prawo.png");
		szamani[i].SzamanSprite[40] = al_load_bitmap("Szaman/szaman w gore prawo 3.png");
		szamani[i].SzamanSprite[41] = al_load_bitmap("Szaman/szaman w gore prawo 4.png");

		szamani[i].SzamanSprite[42] = al_load_bitmap("Szaman/szaman w gore lewo.png");
		szamani[i].SzamanSprite[43] = al_load_bitmap("Szaman/szaman w gore lewo 1.png");
		szamani[i].SzamanSprite[44] = al_load_bitmap("Szaman/szaman w gore lewo 2.png");
		szamani[i].SzamanSprite[45] = al_load_bitmap("Szaman/szaman w gore lewo.png");
		szamani[i].SzamanSprite[46] = al_load_bitmap("Szaman/szaman w gore lewo 3.png");
		szamani[i].SzamanSprite[47] = al_load_bitmap("Szaman/szaman w gore lewo 4.png");

		szamani[i].SzamanSprite[48] = al_load_bitmap("Krew/Krew1.png");
		szamani[i].SzamanSprite[49] = al_load_bitmap("Krew/Krew2.png");
		szamani[i].SzamanSprite[50] = al_load_bitmap("Krew/Krew3.png");

	}
}
void DrawSzaman(Szaman szamani[], int ile, int pomocnicza_do_sprite_szamana)
{

	for (int i = 0; i < ile; i++)
	{
		if (szamani[i].live)
		{
			//sprite Szamanow
			if (szamani[i].PozycjaSzamana == 4)
			{
				if (pomocnicza_do_sprite_szamana == 0)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[6], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 0 && pomocnicza_do_sprite_szamana <= 10)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[7], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 10 && pomocnicza_do_sprite_szamana <= 20)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[8], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 20 && pomocnicza_do_sprite_szamana <= 30)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[9], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 30 && pomocnicza_do_sprite_szamana <= 40)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[10], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 40 && pomocnicza_do_sprite_szamana <= 50)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[11], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana == 50)
				{
					pomocnicza_do_sprite_szamana = 0;
				}
			}
			if (szamani[i].PozycjaSzamana == 3)
			{
				if (pomocnicza_do_sprite_szamana == 0)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[0], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 0 && pomocnicza_do_sprite_szamana <= 10)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[1], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 10 && pomocnicza_do_sprite_szamana <= 20)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[2], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 20 && pomocnicza_do_sprite_szamana <= 30)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[3], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 30 && pomocnicza_do_sprite_szamana <= 40)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[4], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 40 && pomocnicza_do_sprite_szamana <= 50)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[5], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana == 50)
				{
					pomocnicza_do_sprite_szamana = 0;
				}
			}
			if (szamani[i].PozycjaSzamana == 1)
			{
				if (pomocnicza_do_sprite_szamana == 0)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[12], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 0 && pomocnicza_do_sprite_szamana <= 10)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[13], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 10 && pomocnicza_do_sprite_szamana <= 20)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[14], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 20 && pomocnicza_do_sprite_szamana <= 30)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[15], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 30 && pomocnicza_do_sprite_szamana <= 40)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[16], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 40 && pomocnicza_do_sprite_szamana <= 50)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[17], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana == 50)
				{
					pomocnicza_do_sprite_szamana = 0;
				}
			}
			if (szamani[i].PozycjaSzamana == 2)
			{
				if (pomocnicza_do_sprite_szamana == 0)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[18], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 0 && pomocnicza_do_sprite_szamana <= 10)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[19], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 10 && pomocnicza_do_sprite_szamana <= 20)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[20], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 20 && pomocnicza_do_sprite_szamana <= 30)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[21], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 30 && pomocnicza_do_sprite_szamana <= 40)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[22], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 40 && pomocnicza_do_sprite_szamana <= 50)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[23], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana == 50)
				{
					pomocnicza_do_sprite_szamana = 0;
				}
			}
			if (szamani[i].PozycjaSzamana == 8)
			{
				if (pomocnicza_do_sprite_szamana == 0)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[24], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 0 && pomocnicza_do_sprite_szamana <= 10)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[25], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 10 && pomocnicza_do_sprite_szamana <= 20)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[26], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 20 && pomocnicza_do_sprite_szamana <= 30)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[27], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 30 && pomocnicza_do_sprite_szamana <= 40)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[28], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 40 && pomocnicza_do_sprite_szamana <= 50)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[29], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana == 50)
				{
					pomocnicza_do_sprite_szamana = 0;
				}
			}
			if (szamani[i].PozycjaSzamana == 6)
			{
				if (pomocnicza_do_sprite_szamana == 0)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[30], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 0 && pomocnicza_do_sprite_szamana <= 10)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[31], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 10 && pomocnicza_do_sprite_szamana <= 20)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[32], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 20 && pomocnicza_do_sprite_szamana <= 30)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[33], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 30 && pomocnicza_do_sprite_szamana <= 40)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[34], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 40 && pomocnicza_do_sprite_szamana <= 50)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[35], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana == 50)
				{
					pomocnicza_do_sprite_szamana = 0;
				}
			}
			if (szamani[i].PozycjaSzamana == 7)
			{
				if (pomocnicza_do_sprite_szamana == 0)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[36], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 0 && pomocnicza_do_sprite_szamana <= 10)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[37], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 10 && pomocnicza_do_sprite_szamana <= 20)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[38], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 20 && pomocnicza_do_sprite_szamana <= 30)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[39], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 30 && pomocnicza_do_sprite_szamana <= 40)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[40], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 40 && pomocnicza_do_sprite_szamana <= 50)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[41], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana == 50)
				{
					pomocnicza_do_sprite_szamana = 0;
				}
			}
			if (szamani[i].PozycjaSzamana == 5)
			{
				if (pomocnicza_do_sprite_szamana == 0)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[42], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 0 && pomocnicza_do_sprite_szamana <= 10)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[43], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 10 && pomocnicza_do_sprite_szamana <= 20)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[44], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 20 && pomocnicza_do_sprite_szamana <= 30)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[45], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 30 && pomocnicza_do_sprite_szamana <= 40)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[46], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana > 40 && pomocnicza_do_sprite_szamana <= 50)
				{
					al_draw_bitmap(szamani[i].SzamanSprite[47], szamani[i].x, szamani[i].y, 0);
				}
				if (pomocnicza_do_sprite_szamana == 50)
				{
					pomocnicza_do_sprite_szamana = 0;
				}
			}

		}
		if (szamani[i].efekt_krwi == true)
		{
			if (szamani[i].pomocnicza_do_krwi <= 30 && faza_gry == 2)
			{
				szamani[i].pomocnicza_do_krwi++;
			}
			if (szamani[i].pomocnicza_do_krwi == 30)
			{
				szamani[i].efekt_krwi = false;
				szamani[i].pomocnicza_do_krwi = 0;
			}
			if (szamani[i].pomocnicza_do_krwi >= 1 && szamani[i].pomocnicza_do_krwi <= 10)
			{
				al_draw_bitmap(szamani[i].SzamanSprite[48], szamani[i].x, szamani[i].y, 0);
			}
			if (szamani[i].pomocnicza_do_krwi > 10 && szamani[i].pomocnicza_do_krwi <= 20)
			{
				al_draw_bitmap(szamani[i].SzamanSprite[49], szamani[i].x, szamani[i].y, 0);
			}
			if (szamani[i].pomocnicza_do_krwi > 20 && szamani[i].pomocnicza_do_krwi <= 29)
			{
				al_draw_bitmap(szamani[i].SzamanSprite[50], szamani[i].x, szamani[i].y, 0);
			}
		}

	}
}
void StartSzaman(Szaman szamani[], int ile)
{
	for (int i = 0; i < ile; i++)
	{
		//zeby nie zrespil sie zanim animacja krwi sie skonczy
		if (szamani[i].pomocnicza_do_krwi == 0)
		{
			if (!szamani[i].live)
			{
				if (rand() % losowanie_szamana == 0)
				{
					if (szamani[i].a == 0)
					{
						szamani[i].live = true;
						if (mapa_x < 0)
						{
							szamani[i].x = ((rand() % 3 + 2) * 50) + mapa_x;
						}
						if (mapa_x == 0)
						{
							szamani[i].x = ((rand() % 3 + 2) * 50);
						}
						if (mapa_y < 0)
						{
							szamani[i].y = ((rand() % 2 + 3) * 50) + mapa_y;
						}
						if (mapa_y == 0)
						{
							szamani[i].y = ((rand() % 2 + 3) * 50);
						}
						KtoryXY(szamani[i].x, szamani[i].y, mapa_x, mapa_y, szamani[i].ktoryX_szaman, szamani[i].ktoryY_szaman);
					}
					else if (szamani[i].a == 1)
					{
						szamani[i].live = true;
						if (mapa_x<0)
						{
							szamani[i].x = ((rand() % 3 + 23) * 50) + mapa_x;
						}
						if (mapa_x == 0)
						{
							szamani[i].x = ((rand() % 3 + 23) * 50);
						}
						if (mapa_y < 0)
						{
							szamani[i].y = ((rand() % 2 + 3) * 50) + mapa_y;
						}
						if (mapa_y == 0)
						{
							szamani[i].y = ((rand() % 2 + 3) * 50);
						}
						KtoryXY(szamani[i].x, szamani[i].y, mapa_x, mapa_y, szamani[i].ktoryX_szaman, szamani[i].ktoryY_szaman);
					}
					else if (szamani[i].a == 2)
					{
						szamani[i].live = true;
						if (mapa_x < 0)
						{
							szamani[i].x = ((rand() % 3 + 3) * 50) + mapa_x;
						}
						if (mapa_x == 0)
						{
							szamani[i].x = ((rand() % 3 + 3) * 50);
						}
						if (mapa_y > -800)
						{
							szamani[i].y = ((rand() % 2 + 23) * 50) + mapa_y;
						}
						if (mapa_y == -800)
						{
							szamani[i].y = ((rand() % 2 + 8) * 50);
						}
						KtoryXY(szamani[i].x, szamani[i].y, mapa_x, mapa_y, szamani[i].ktoryX_szaman, szamani[i].ktoryY_szaman);
					}
					else if (szamani[i].a == 3)
					{
						szamani[i].live = true;
						if (mapa_x > -250)
						{
							szamani[i].x = ((rand() % 3 + 26) * 50) + mapa_x;
						}
						if (mapa_x == -250)
						{
							szamani[i].x = ((rand() % 3 + 18) * 50);
						}
						if (mapa_y > -800)
						{
							szamani[i].y = ((rand() % 2 + 24) * 50) + mapa_y;
						}
						if (mapa_y == -800)
						{
							szamani[i].y = ((rand() % 2 + 9) * 50);
						}
						KtoryXY(szamani[i].x, szamani[i].y, mapa_x, mapa_y, szamani[i].ktoryX_szaman, szamani[i].ktoryY_szaman);
					}
				}


				break;
			}
		}
	}
}
void RuchSzaman(Szaman szamani[], int ile)
{
	for (int i = 0; i < ile; i++)
	{
		if (szamani[i].live)
		{
			//  w prawo dol
			if (szamani[i].y + 24 <= pos_y + 24 && szamani[i].x + 24 <= pos_x + 24)
			{
				szamani[i].kierunek = 1;
			}// w prawo gore
			else if (szamani[i].y + 24 >= pos_y + 24 && szamani[i].x + 24 <= pos_x + 24)
			{
				szamani[i].kierunek = 2;
			}
			//w lewo dol
			else if (szamani[i].y + 24 <= pos_y + 24 && szamani[i].x + 24 >= pos_x + 24)
			{
				szamani[i].kierunek = 3;
			}
			// w lewo gora
			else if (szamani[i].y + 24 >= pos_y + 24 && szamani[i].x + 24 >= pos_x + 24)
			{
				szamani[i].kierunek = 4;
			}
			
			
			SciezkaSzamana(pos_x, pos_y, szamani[i].x, szamani[i].y, szamani[i].predkosc, i, szamani[i].PozycjaSzamana);
			KtoryXY(szamani[i].x, szamani[i].y, mapa_x, mapa_y, szamani[i].ktoryX_szaman, szamani[i].ktoryY_szaman);

			szamani[i].odleglosc_x = sqrtf(pow(((szamani[i].x + 24 - mapa_x) - (pos_x + 24 - mapa_x)), 2));
			szamani[i].odleglosc_y = sqrtf(pow(((szamani[i].y + 24 - mapa_y) - (pos_y + 24 - mapa_y)), 2));
			szamani[i].odleglosc_xy = sqrtf(pow(szamani[i].odleglosc_x, 2) + pow(szamani[i].odleglosc_y, 2));
			szamani[i].sinus = szamani[i].odleglosc_y / szamani[i].odleglosc_xy;
			szamani[i].cosinus = szamani[i].odleglosc_x / szamani[i].odleglosc_xy;
			RuchOrkaW(szamani[i].x, szamani[i].y, szamani[i].ktoryX_szaman, szamani[i].ktoryY_szaman, szamani[i].predkosc);
			RuchOrkaS(szamani[i].x, szamani[i].y, szamani[i].ktoryX_szaman, szamani[i].ktoryY_szaman, szamani[i].predkosc);
			RuchOrkaA(szamani[i].x, szamani[i].y, szamani[i].ktoryX_szaman, szamani[i].ktoryY_szaman, szamani[i].predkosc);
			RuchOrkaD(szamani[i].x, szamani[i].y, szamani[i].ktoryX_szaman, szamani[i].ktoryY_szaman, szamani[i].predkosc);

		}
	}
}
void SciezkaSzamana(float pos_x, float pos_y, float &  przeciwnik_x, float & przeciwnik_y, float predkosc_szamana,int ile, int & PozycjaSzamana)
{
			//  w prawo
			if (przeciwnik_x < pos_x && przeciwnik_y == pos_y)
			{
				przeciwnik_x += predkosc_szamana;
				PozycjaSzamana = 1;
			}// w lewo
			else if (przeciwnik_x > pos_x && przeciwnik_y == pos_y)
			{
				przeciwnik_x -= predkosc_szamana;
				PozycjaSzamana = 2;
			}// do dolu
			else if (przeciwnik_y < pos_y && przeciwnik_x == pos_x)
			{
				przeciwnik_y += predkosc_szamana;
				PozycjaSzamana = 3;
			}// do gory
			else if (przeciwnik_y > pos_y && przeciwnik_x == pos_x)
			{
				przeciwnik_y -= predkosc_szamana;
				PozycjaSzamana = 4;
			}// w lewo gore
			else if (przeciwnik_y > pos_y && przeciwnik_x > pos_x)
			{
				PozycjaSzamana = 5;
				przeciwnik_x -= predkosc_szamana;
				przeciwnik_y -= predkosc_szamana;
				
			}// w lewo dol
			else if (przeciwnik_y < pos_y && przeciwnik_x > pos_x)
			{
				PozycjaSzamana = 6;
				przeciwnik_x -= predkosc_szamana;
				przeciwnik_y += predkosc_szamana;
				
			}//w prawo gore
			else if (przeciwnik_y > pos_y && przeciwnik_x < pos_x)
			{
				PozycjaSzamana = 7;
				przeciwnik_x += predkosc_szamana;
				przeciwnik_y -= predkosc_szamana;
				
			}// w prawo dol
			else if (przeciwnik_y < pos_y && przeciwnik_x < pos_x)
			{
				PozycjaSzamana = 8;
				przeciwnik_x += predkosc_szamana;
				przeciwnik_y += predkosc_szamana;
				
			}
			else if (przeciwnik_x == pos_x && przeciwnik_y == pos_y)
			{
				PozycjaSzamana = 3;
			}
}


void InitPociskSzamana( int ile, Szaman szamani[])
{
	for (int i = 0; i < ile; i++)
	{
		szamani[i].pociski_szamana.ID = POCISKSZAMANA;
		szamani[i].pociski_szamana.live = false;
		szamani[i].pociski_szamana.predkosc = 6.0;
		szamani[i].pociski_szamana.ktoryX_pocisk_szamana;
		szamani[i].pociski_szamana.ktoryY_pocisk_szamana;
		szamani[i].pociski_szamana.czy_moze_strzelic_szaman = true;

		szamani[i].pociski_szamana.pocisk_szamana[0] = al_load_bitmap("PociskSzamana/Pocisk szamana w dol.png");
		szamani[i].pociski_szamana.pocisk_szamana[1] = al_load_bitmap("PociskSzamana/Pocisk szamana w gore.png");
		szamani[i].pociski_szamana.pocisk_szamana[2] = al_load_bitmap("PociskSzamana/Pocisk szamana w prawo.png");
		szamani[i].pociski_szamana.pocisk_szamana[3] = al_load_bitmap("PociskSzamana/Pocisk szamana w lewo.png");

		szamani[i].pociski_szamana.pocisk_szamana[4] = al_load_bitmap("PociskSzamana/Pocisk szamana w lewo dol 1.png");
		szamani[i].pociski_szamana.pocisk_szamana[5] = al_load_bitmap("PociskSzamana/Pocisk szamana w lewo dol 2.png");

		szamani[i].pociski_szamana.pocisk_szamana[6] = al_load_bitmap("PociskSzamana/Pocisk szamana w prawo dol 1.png");
		szamani[i].pociski_szamana.pocisk_szamana[7] = al_load_bitmap("PociskSzamana/Pocisk szamana w prawo dol 2.png");

		szamani[i].pociski_szamana.pocisk_szamana[8] = al_load_bitmap("PociskSzamana/Pocisk szamana w lewo gore 1.png");
		szamani[i].pociski_szamana.pocisk_szamana[9] = al_load_bitmap("PociskSzamana/Pocisk szamana w lewo gore 2.png");

		szamani[i].pociski_szamana.pocisk_szamana[10] = al_load_bitmap("PociskSzamana/Pocisk szamana w prawo gore 1.png");
		szamani[i].pociski_szamana.pocisk_szamana[11] = al_load_bitmap("PociskSzamana/Pocisk szamana w prawo gore 2.png");
	}
}
void DrawPociskSzamana(int ile, Szaman szamani[])
{
		for (int i = 0; i < ile; i++)
		{
			if (szamani[i].pociski_szamana.live==true)
			{
				// w lewo gore
				if (szamani[i].pociski_szamana.kierunek == 4)
				{

					if (szamani[i].pociski_szamana.sinus >= 0 && szamani[i].pociski_szamana.sinus <= 0.1736)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[3], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
					if (szamani[i].pociski_szamana.sinus >= 0.9848 && szamani[i].pociski_szamana.sinus <= 1)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[1], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
					if (szamani[i].pociski_szamana.sinus > 0.1736 && szamani[i].pociski_szamana.sinus <= 0.7071)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[8], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
					if (szamani[i].pociski_szamana.sinus > 0.7071 && szamani[i].pociski_szamana.sinus < 0.9848)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[9], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
				}
				// w lewo dol
				if (szamani[i].pociski_szamana.kierunek == 3)
				{
					if (szamani[i].pociski_szamana.sinus >= 0 && szamani[i].pociski_szamana.sinus <= 0.1736)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[3], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
					if (szamani[i].pociski_szamana.sinus >= 0.9848 && szamani[i].pociski_szamana.sinus <= 1)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[0], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
					if (szamani[i].pociski_szamana.sinus > 0.7071 && szamani[i].pociski_szamana.sinus <= 0.9848)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[4], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
					if (szamani[i].pociski_szamana.sinus > 0.1736 && szamani[i].pociski_szamana.sinus < 0.7071)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[5], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
				}
				//w prawo gore
				if (szamani[i].pociski_szamana.kierunek == 2)
				{
					if (szamani[i].pociski_szamana.sinus >= 0 && szamani[i].pociski_szamana.sinus <= 0.1736)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[2], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
					if (szamani[i].pociski_szamana.sinus >= 0.9848 && szamani[i].pociski_szamana.sinus <= 1)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[1], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
					if (szamani[i].pociski_szamana.sinus >= 0.7071 && szamani[i].pociski_szamana.sinus < 0.9848)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[10], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
					if (szamani[i].pociski_szamana.sinus > 0.1736 && szamani[i].pociski_szamana.sinus < 0.7071)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[11], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
				}
				// w prawo dol
				if (szamani[i].pociski_szamana.kierunek == 1)
				{
					if (szamani[i].pociski_szamana.sinus >= 0 && szamani[i].pociski_szamana.sinus <= 0.1736)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[2], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
					if (szamani[i].pociski_szamana.sinus >= 0.9848 && szamani[i].pociski_szamana.sinus <= 1)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[0], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
					if (szamani[i].pociski_szamana.sinus > 0.1736 && szamani[i].pociski_szamana.sinus <= 0.7071)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[6], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
					if (szamani[i].pociski_szamana.sinus > 0.7071 && szamani[i].pociski_szamana.sinus < 0.9848)
						al_draw_bitmap(szamani[i].pociski_szamana.pocisk_szamana[7], szamani[i].pociski_szamana.x, szamani[i].pociski_szamana.y, 0);
				}
			}
		}
	
}
void StartPociskSzamana(int ile, Szaman szamani[])
{
		for (int i = 0; i < ile; i++)
		{
			if (szamani[i].pociski_szamana.live == false)
			{
				
				szamani[i].pociski_szamana.kierunek = szamani[i].kierunek;
				szamani[i].pociski_szamana.live = true;
				szamani[i].pociski_szamana.x = szamani[i].x + 1;
				szamani[i].pociski_szamana.y = szamani[i].y + 1;
				szamani[i].pociski_szamana.odleglosc_x = sqrtf(pow(((szamani[i].pociski_szamana.x - mapa_x) - (pos_x - mapa_x)), 2));
				szamani[i].pociski_szamana.odleglosc_y = sqrtf(pow(((szamani[i].pociski_szamana.y - mapa_y) - (pos_y  - mapa_y)), 2));
				szamani[i].pociski_szamana.odleglosc_xy = sqrtf(pow(szamani[i].pociski_szamana.odleglosc_x, 2) + pow(szamani[i].pociski_szamana.odleglosc_y, 2));
				szamani[i].pociski_szamana.sinus = szamani[i].pociski_szamana.odleglosc_y / szamani[i].pociski_szamana.odleglosc_xy;
				szamani[i].pociski_szamana.cosinus = szamani[i].pociski_szamana.odleglosc_x / szamani[i].pociski_szamana.odleglosc_xy;
			
			}
		}
}
void RuchPociskSzamana(int ile, Szaman szamani[])
{
		for (int i = 0; i < ile; i++)
		{
			if (szamani[i].pociski_szamana.live==true)
			{
				if (szamani[i].pociski_szamana.x > szerokosc)
					szamani[i].pociski_szamana.live = false;
				if (szamani[i].pociski_szamana.x < 0)
					szamani[i].pociski_szamana.live = false;
				if (szamani[i].pociski_szamana.y > wysokosc)
					szamani[i].pociski_szamana.live = false;
				if (szamani[i].pociski_szamana.y < 0)
					szamani[i].pociski_szamana.live = false;
					
				KtoryXY(szamani[i].pociski_szamana.x + 10, szamani[i].pociski_szamana.y + 10, mapa_x, mapa_y, szamani[i].pociski_szamana.ktoryX_pocisk_szamana, szamani[i].pociski_szamana.ktoryY_pocisk_szamana);
				
				// kolizja przeszkody
				if (szamani[i].pociski_szamana.live==true)
				{
					// W
					if ((czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana][szamani[i].pociski_szamana.ktoryY_pocisk_szamana] == false) && (czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 1][szamani[i].pociski_szamana.ktoryY_pocisk_szamana] == false))
					{
						if ((szamani[i].pociski_szamana.x >= (szamani[i].pociski_szamana.ktoryX_pocisk_szamana * 50)) && (szamani[i].pociski_szamana.x <= ((szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 2) * 50)))
						{
							szamani[i].pociski_szamana.live = false;
						}
					}
					else if ((czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana][szamani[i].pociski_szamana.ktoryY_pocisk_szamana] == false))
					{
						if (szamani[i].pociski_szamana.x < (((szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 1) * 50)))
						{
							szamani[i].pociski_szamana.live = false;
						}
					}
					else if ((czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 1][szamani[i].pociski_szamana.ktoryY_pocisk_szamana] == false))
					{
						if ((szamani[i].pociski_szamana.x + 50 >(((szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 1) * 50))))
						{
							szamani[i].pociski_szamana.live = false;
						}
					}
					// S
					if ((czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 1][szamani[i].pociski_szamana.ktoryY_pocisk_szamana] == false) && (czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 1][szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 1] == false))
					{
						if ((szamani[i].pociski_szamana.y >= (szamani[i].pociski_szamana.ktoryY_pocisk_szamana * 50)) && (szamani[i].pociski_szamana.y <= ((szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 2) * 50)))
						{
							szamani[i].pociski_szamana.live = false;
						}
					}
					else if ((czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 1][szamani[i].pociski_szamana.ktoryY_pocisk_szamana] == false))
					{
						if (szamani[i].pociski_szamana.y < (((szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 1) * 50)))
						{
							szamani[i].pociski_szamana.live = false;
						}
					}
					else if ((czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 1][szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 1] == false))
					{
						if ((szamani[i].pociski_szamana.y + 50) >(((szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 1) * 50)))
						{
							szamani[i].pociski_szamana.live = false;
						}
					}
					// A
					if ((czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana][szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 1] == false) && (czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 1][szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 1] == false))
					{
						if ((szamani[i].pociski_szamana.x >= (szamani[i].pociski_szamana.ktoryX_pocisk_szamana * 50)) && (szamani[i].pociski_szamana.x <= ((szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 2) * 50)))
						{
							szamani[i].pociski_szamana.live = false;
						}
					}
					else if ((czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana][szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 1] == false))
					{
						if (szamani[i].pociski_szamana.x < (((szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 1) * 50)))
						{
							szamani[i].pociski_szamana.live = false;
						}
					}
					else if ((czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 1][szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 1] == false))
					{
						if ((szamani[i].pociski_szamana.x + 50) >(((szamani[i].pociski_szamana.ktoryX_pocisk_szamana + 1) * 50)))
						{
							szamani[i].pociski_szamana.live = false;
						}
					}
					// D
					if ((czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana][szamani[i].pociski_szamana.ktoryY_pocisk_szamana] == false) && (czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana][szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 1] == false))
					{
						if ((szamani[i].pociski_szamana.y >= (szamani[i].pociski_szamana.ktoryY_pocisk_szamana * 50)) && (szamani[i].pociski_szamana.y <= ((szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 2) * 50)))
						{
							szamani[i].pociski_szamana.live = false;
						}
					}
					if ((czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana][szamani[i].pociski_szamana.ktoryY_pocisk_szamana] == false))
					{
						if (szamani[i].pociski_szamana.y < (((szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 1) * 50)))
						{
							szamani[i].pociski_szamana.live = false;
						}

					}
					if ((czy_mozna_wejsc[szamani[i].pociski_szamana.ktoryX_pocisk_szamana][szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 1] == false))
					{
						if ((szamani[i].pociski_szamana.y + 50) >(((szamani[i].pociski_szamana.ktoryY_pocisk_szamana + 1) * 50)))
						{
							szamani[i].pociski_szamana.live = false;
						}
					}
				}
				

				// strzelanie w 360 stopniach
				if (szamani[i].pociski_szamana.live==true)
				{

					// w lewo gore
					if (szamani[i].pociski_szamana.kierunek == 4)
					{
						szamani[i].pociski_szamana.x -= szamani[i].pociski_szamana.cosinus*szamani[i].pociski_szamana.predkosc;
						szamani[i].pociski_szamana.y -= szamani[i].pociski_szamana.sinus*szamani[i].pociski_szamana.predkosc;
					}// w lewo dol
					if (szamani[i].pociski_szamana.kierunek == 3)
					{
						szamani[i].pociski_szamana.x -= szamani[i].pociski_szamana.cosinus*szamani[i].pociski_szamana.predkosc;
						szamani[i].pociski_szamana.y += szamani[i].pociski_szamana.sinus*szamani[i].pociski_szamana.predkosc;
					}
					//w prawo gore
					if (szamani[i].pociski_szamana.kierunek == 2)
					{
						szamani[i].pociski_szamana.x += szamani[i].pociski_szamana.cosinus*szamani[i].pociski_szamana.predkosc;
						szamani[i].pociski_szamana.y -= szamani[i].pociski_szamana.sinus*szamani[i].pociski_szamana.predkosc;
					}
					// w prawo dol
					if (szamani[i].pociski_szamana.kierunek == 1)
					{
						szamani[i].pociski_szamana.x += szamani[i].pociski_szamana.cosinus*szamani[i].pociski_szamana.predkosc;
						szamani[i].pociski_szamana.y += szamani[i].pociski_szamana.sinus*szamani[i].pociski_szamana.predkosc;
					}
				}
			}
		}
			
	
}