#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_image.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <iostream>


enum wsad { W, S, A, D };
enum RuchOrka {Gora,Dol,Lewo,Prawo,LewoGora,LewoDol,PrawoGora,PrawoDol};
enum przeciwnik { PRZECIWNIK };


RuchOrka PozycjaOrka[8] = { Gora,Gora,Gora,Gora,Gora,Gora,Gora,Gora };

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
	ALLEGRO_BITMAP *OrkSprite[48];
};

ALLEGRO_FONT *font24 = al_load_ttf_font("arial.ttf", 24, 0);

void KtoryXY(float pos_x, float pos_y, float mapa_x, float mapa_y, int & ktoryX, int & ktoryY);
void RuchPostaciW(float pos_x, float & pos_y, int ktoryX, int ktoryY, float predkosc_postaci);
void RuchPostaciS(float pos_x, float & pos_y, int ktoryX, int ktoryY, float predkosc_postaci);
void RuchPostaciA(float & pos_x, float pos_y, int ktoryX, int ktoryY, float predkosc_postaci);
void RuchPostaciD(float & pos_x, float pos_y, int ktoryX, int ktoryY, float predkosc_postaci);

void InitOrk(Ork orkowie[], int ile);
void DrawOrk(Ork orkowie[], int ile,int pomocnicza_do_sprite_orka);
void StartOrk(Ork orkowie[], int ile);
void RuchOrk(Ork orkowie[], int ile);
void Sciezka(float pos_x, float pos_y, float &  przeciwnik_x, float & przeciwnik_y, float predkosc_orka, int ktoryX_ork, int ktoryY_ork, int ile);
// okno 
int szerokosc = 1250;
int wysokosc = 700;

bool czy_mozna_wejsc[30][30];

int SiatkaX[30];
int SiatkaY[30];
int ktoryX = 14;
int ktoryY = 14;




const int ilosc_orkow = 20;
char ostatni_ruch_orka;
int pomocnicza_do_sprite_orka = 0;


bool ruch_mapy = false;
float mapa_x = -500;
float mapa_y = -500;
// 0 trawa 1 wieza 2 domek 3,4,5,6 hall 7 krzak 8 suchedrzewo 9 drzewo 10 woda 11 most
int mapa[] =
{
	9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
	9, 3, 4, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 3, 4, 8, 8,
	7, 5, 6, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 6, 8, 8,
	9, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8,
	9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8,
	10, 1, 0, 0, 0, 0, 0, 0, 0, 9, 9, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 9, 9,
	10, 0, 0, 0, 0, 0, 0, 0, 0, 9, 7, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 9, 9,
	10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9,
	10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8,
	10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
	10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
	10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 7, 7,
	10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 7, 7,
	9, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0    , 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 7, 7,
	9, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 7, 7, 7,
	9, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 7, 7, 7,
	9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 7, 7, 9,
	7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
	7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
	9, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
	9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	2, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 4, 0, 2,
	2, 0, 5, 6, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 2,
	9, 2, 0, 0, 0, 8, 8, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 8, 0, 0, 0, 0, 2,
	9, 9, 9, 9, 9, 9, 9, 9, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 9, 9, 9, 9, 2, 2, 2,
};






float pos_x = (szerokosc / 2) - 125; //500
float pos_y = (wysokosc / 2) - 125; //225



int main(void) {
	

	// postac

	float predkosc_postaci = 2;
	float predkosc_mapy = 2;




	const float FPS = 60.0;
	int ilosc_klatek = 0;
	//przeciwnicy
	Ork orkowie[ilosc_orkow];



	char ostatnia_pozycja;
	bool wsad[4] = { false,false,false,false };

	bool redraw = true;
	bool done = false;

	// przewijanie mapy

	





	int szerokosc_mapy = 30;  // ilosc w jednej linii kolumn 50x50
	int rozmiar_mapy = 900;   // lacznie ile jest kolumn 50x50
	int rozmiar_pola = 50;




	// warunek przeszkody

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


	if (!al_init())
		return -1;

	// timer
	timer = al_create_timer(1.0 / FPS);
	srand(time(NULL));



	InitOrk(orkowie, ilosc_orkow);
	// bitmapy 



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


	bgImage = al_load_bitmap("background.png");
	ALLEGRO_BITMAP *Ekran_startowy = al_load_bitmap("Ekran startowy.png");



	// okno
	okno = al_create_display(szerokosc, wysokosc);

	if (!okno)
		return -1;


	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(okno));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));


	ALLEGRO_FONT *font24 = al_load_ttf_font("arial.ttf", 24, 0);
	al_start_timer(timer);
	/*
	al_draw_bitmap(Ekran_startowy, 0, 0, 0);
	al_flip_display();
	al_rest(1.5);
	*/
	while (!done)
	{


		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		/*
		al_draw_textf(font24, al_map_rgb(255, 255, 255), 100, 20, ALLEGRO_ALIGN_CENTRE,
		"Klatki: %i", ilosc_klatek);
		*/

		
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

			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			pomocnicza_do_sprite_orka++;
			StartOrk(orkowie, ilosc_orkow);
			RuchOrk(orkowie, ilosc_orkow);

			if (pomocnicza_do_sprite_orka == 50)
				pomocnicza_do_sprite_orka = 0;

			mapa_x -= wsad[D] * predkosc_mapy;
			mapa_x += wsad[A] * predkosc_mapy;
			mapa_y -= wsad[S] * predkosc_mapy;
			mapa_y += wsad[W] * predkosc_mapy;

			if (mapa_x < -250 || mapa_y < -800 || mapa_x > 0 || mapa_y > 0)
			{/*
			 pos_x += wsad[D] * (predkosc_postaci / 2);
			 pos_x -= wsad[A] * (predkosc_postaci / 2);  // mapa zablowana
			 pos_y += wsad[S] * (predkosc_postaci / 2);
			 pos_y -= wsad[W] * (predkosc_postaci / 2);*/
			}
			else
			{
				pos_x -= wsad[D] * (predkosc_postaci);
				pos_x += wsad[A] * (predkosc_postaci );  // mapa ruchoma
				pos_y -= wsad[S] * (predkosc_postaci);
				pos_y += wsad[W] * (predkosc_postaci );
			}
			
			if (wsad[W] == true)
			{
				KtoryXY(pos_x, pos_y, mapa_x, mapa_y, ktoryX, ktoryY);
				RuchPostaciW(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
			}
			if (wsad[S] == true)
			{
				KtoryXY(pos_x, pos_y, mapa_x, mapa_y, ktoryX, ktoryY);
				RuchPostaciS(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
			}
			if (wsad[A] == true)
			{
				KtoryXY(pos_x, pos_y, mapa_x, mapa_y, ktoryX, ktoryY);
				RuchPostaciA(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);

			}
			if (wsad[D] == true)
			{
				KtoryXY(pos_x, pos_y, mapa_x, mapa_y, ktoryX, ktoryY);
				RuchPostaciD(pos_x, pos_y, ktoryX, ktoryY, predkosc_postaci);
			}
			
			
			// warunki przewijania mapy
			if (mapa_x < -250)
				mapa_x = -250;
			if (mapa_y < -800)
				mapa_y = -800;
			if (mapa_x > 0)
				mapa_x = 0;
			if (mapa_y > 0)
				mapa_y = 0;

			// postac nie moze wyjsc poza obszar okna
			if (pos_x < 0)
				pos_x = 0;
			if (pos_y < 0)
				pos_y = 0;
			if (pos_x > 1200)
				pos_x = 1200;
			if (pos_y > 650)
				pos_y = 650;

			redraw = true;

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
			/*
			float ork_x = orkowie[0].x - mapa_x;
			float ork_y = orkowie[0].y - mapa_y;
			al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 220, ALLEGRO_ALIGN_LEFT,
				"X orka: %.0f", ork_x);
			al_draw_textf(font24, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_RIGHT,
				"Y orka: %.0f", ork_y);
			*/
			//pozycja postaci na mapie
			float x = pos_x - mapa_x;
			float y = pos_y - mapa_y;
			al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 20, ALLEGRO_ALIGN_LEFT,
			"X postaci: %.0f", x);
			al_draw_textf(font24, al_map_rgb(255, 255, 255), 400, 50, ALLEGRO_ALIGN_RIGHT,
			"Y postaci: %.0f", y);
			/*
			al_draw_textf(font24, al_map_rgb(255, 255, 255), 500, 100, ALLEGRO_ALIGN_LEFT,
			"X postaci: %i", ktoryX);
			al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 150, ALLEGRO_ALIGN_RIGHT,
			"Y postaci: %i", ktoryY);
			al_draw_textf(font24, al_map_rgb(255, 255, 255), 500, 400, ALLEGRO_ALIGN_LEFT,
				"X mapy: %.0f", mapa_x);
			al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 450, ALLEGRO_ALIGN_RIGHT,
				"Y mapy: %.0f", mapa_y);*/
			DrawOrk(orkowie, ilosc_orkow, pomocnicza_do_sprite_orka);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
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



	for (int i = 0; i < 30; i++)
	{
		int pozycja = pos_x - mapa_x;
		if ((pozycja > SiatkaX[i]) && (pozycja < SiatkaX[i + 1]))
		{
			ktoryX = i; //w jakiej pozycji siatki jestem na X
			break;
		}
	}

	for (int i = 0; i < 30; i++)
	{
		int pozycja = pos_y - mapa_y;
		if ((pozycja >(SiatkaY[i])) && (pozycja < (SiatkaY[i + 1])))
		{
			ktoryY = i; //w jakiej pozycji siatki jestem na Y
			break;
		}
	}
}

void RuchPostaciW(float pos_x, float & pos_y, int ktoryX, int ktoryY, float predkosc_postaci)
{
	float gdzie_bedeY = pos_y - predkosc_postaci ;
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
		pos_y -= predkosc_postaci ;
	}
	else if (obszar_mapy == false)
	{
		pos_y = 0;
	}
}


void RuchPostaciS(float pos_x, float & pos_y, int ktoryX, int ktoryY, float predkosc_postaci)
{
	float gdzie_bedeY = pos_y + predkosc_postaci ;
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
			pos_y += predkosc_postaci ;
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
		pos_y += predkosc_postaci ;
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
		pos_x -= predkosc_postaci ;
	}
	else if (obszar_mapy == false)
		pos_x = 0;
}
void RuchPostaciD(float & pos_x, float pos_y, int ktoryX, int ktoryY, float predkosc_postaci)
{
	float gdzie_bedeY = pos_x + predkosc_postaci ;
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
			pos_x += predkosc_postaci ;
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
		pos_x += predkosc_postaci ;
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

		for (int j = 0; j < 48; j++)
		{
				al_convert_mask_to_alpha(orkowie[i].OrkSprite[j], al_map_rgb(255, 100, 255));
		}
	}
}
void DrawOrk(Ork orkowie[], int ile,int pomocnicza_do_sprite_orka)
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
			}/*
			if (PozycjaOrka[i] == Dol)
			{
				al_draw_bitmap(orkowie[i].OrkSprite[0], orkowie[i].x, orkowie[i].y, 0);
			}*/
			
		
		}
	}
	
}

void StartOrk(Ork orkowie[], int ile)
{
	for (int i = 0; i < ile; i++)
	{
		if (!orkowie[i].live)
		{
			if (rand() % 500 == 0)
			{
				if (orkowie[i].a == 0)
				{
					orkowie[i].live = true;
					orkowie[i].x = 200;
					orkowie[i].y = 25;
					KtoryXY(orkowie[i].x, orkowie[i].y, mapa_x, mapa_y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork);




					/*
						orkowie[i].x = ((rand() % 3 + 23) * 50);
						orkowie[i].y = ((rand() % 3 + 8) * 50)*(-1);
						std::cout << "Ork X" << orkowie[i].x << std::endl;
						std::cout << "Ork Y" << orkowie[i].y << std::endl;
						KtoryXY(orkowie[i].x, orkowie[i].y, mapa_x, mapa_y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork);
						//if (czy_mozna_wejsc[orkowie[i].ktoryX_ork][orkowie[i].ktoryY_ork]==true)
							//break;
						std::cout << "Ork KtoryX" << orkowie[i].ktoryX_ork << std::endl;
						std::cout << "Ork KtoryY" << orkowie[i].ktoryY_ork << std::endl;
					*/
				}
				else if (orkowie[i].a == 1)
				{
					orkowie[i].live = true;
					orkowie[i].x = 800;
					orkowie[i].y = 25;
					KtoryXY(orkowie[i].x, orkowie[i].y, mapa_x, mapa_y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork);
				}
				else if (orkowie[i].a == 2)
				{
					orkowie[i].live = true;
					orkowie[i].x = 200;
					orkowie[i].y = 425;
					KtoryXY(orkowie[i].x, orkowie[i].y, mapa_x, mapa_y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork);
				}
				else if (orkowie[i].a == 3)
				{
					orkowie[i].live = true;
					orkowie[i].x = 800;
					orkowie[i].y = 425; 
					KtoryXY(orkowie[i].x, orkowie[i].y, mapa_x, mapa_y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork);
				}
				break;
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

			Sciezka(pos_x, pos_y, orkowie[i].x, orkowie[i].y, orkowie[i].predkosc, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork,i);
			
			KtoryXY(orkowie[i].x, orkowie[i].y, mapa_x, mapa_y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork);
			RuchPostaciW(orkowie[i].x, orkowie[i].y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork, orkowie[i].predkosc);
			RuchPostaciS(orkowie[i].x, orkowie[i].y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork, orkowie[i].predkosc);
			RuchPostaciA(orkowie[i].x, orkowie[i].y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork, orkowie[i].predkosc);
			RuchPostaciD(orkowie[i].x, orkowie[i].y, orkowie[i].ktoryX_ork, orkowie[i].ktoryY_ork, orkowie[i].predkosc);
			

			/*
			if (orkowie[i].x < -50)
				orkowie[i].live = false;
			if (orkowie[i].x > 1250)
				orkowie[i].live = false;
			if (orkowie[i].y < -50)
				orkowie[i].live = false;
			if (orkowie[i].y > 700)
				orkowie[i].live = false;*/
		}
	}
}	

void Sciezka(float pos_x, float pos_y,float &  przeciwnik_x,float & przeciwnik_y, float predkosc_orka,int ktoryX_ork,int ktoryY_ork,int ile)
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