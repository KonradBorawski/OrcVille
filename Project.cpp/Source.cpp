#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_image.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>


enum wsad { W, S, A, D };
ALLEGRO_FONT *font24 = al_load_ttf_font("arial.ttf", 24, 0);

void WhereXY(float pos_x, float pos_y, int mapa_x, int mapa_y, int & whereX, int & whereY);
void RuchPostaciW(float pos_x, float & pos_y, int whereX, int whereY, int predkosc_linia_prosta);
void RuchPostaciS(float pos_x, float & pos_y, int whereX, int whereY, int predkosc_linia_prosta);
void RuchPostaciA(float & pos_x, float pos_y, int whereX, int whereY, int predkosc_linia_prosta);
void RuchPostaciD(float & pos_x, float pos_y, int whereX, int whereY, int predkosc_linia_prosta);



bool can[30][30];

int SiatkaX[30];
int SiatkaY[30];
int whereX=14;
int whereY=14;

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
10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 7, 7,
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
int main(void) {
	// okno 
	int szerokosc = 1250;
	int wysokosc = 700;

	// postac
	float pos_x = (szerokosc/2)-125; //500
	float pos_y = (wysokosc/2)-125; //225
	float predkosc_linia_prosta = 4;

	


	const float FPS = 60.0;
	int ilosc_klatek = 0;

	char ostatnia_pozycja;
	bool wsad[4] = { false,false,false,false };

	bool redraw = true;
	bool done = false;

	// przewijanie mapy

	int mapa_x = -500;
	int mapa_y = -500;

	



	int szerokosc_mapy = 30;  // ilosc w jednej linii kolumn 50x50
	int rozmiar_mapy = 900;   // lacznie ile jest kolumn 50x50
	int rozmiar_pola = 50;


	// 0 trawa 1 wieza 2 domek 3,4,5,6 hall 7 krzak 8 suchedrzewo 9 drzewo 10 woda 11 most
	
	
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
			if (mapa[pom]!=0)
			{
				can[j][i] = false;
			}
			else
			{
				can[j][i] = true;
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

	// bitmapy 


	
		ALLEGRO_BITMAP *Ork[48];
		Ork[0] = al_load_bitmap("Ork/Ork w dol.png");
		Ork[1] = al_load_bitmap("Ork/Ork w dol 1.png");
		Ork[2] = al_load_bitmap("Ork/Ork w dol 2.png");
		Ork[3] = al_load_bitmap("Ork/Ork w dol.png");
		Ork[4] = al_load_bitmap("Ork/Ork w dol 3.png");
		Ork[5] = al_load_bitmap("Ork/Ork w dol 4.png");

		Ork[6] = al_load_bitmap("Ork/Ork w gore.png");
		Ork[7] = al_load_bitmap("Ork/Ork w gore 1.png");
		Ork[8] = al_load_bitmap("Ork/Ork w gore 2.png");
		Ork[9] = al_load_bitmap("Ork/Ork w gore.png");
		Ork[10] = al_load_bitmap("Ork/Ork w gore 3.png");
		Ork[11] = al_load_bitmap("Ork/Ork w gore 4.png");

		Ork[12] = al_load_bitmap("Ork/Ork w prawo.png");
		Ork[13] = al_load_bitmap("Ork/Ork w prawo 1.png");
		Ork[14] = al_load_bitmap("Ork/Ork w prawo 2.png");
		Ork[15] = al_load_bitmap("Ork/Ork w prawo.png");
		Ork[16] = al_load_bitmap("Ork/Ork w prawo 3.png");
		Ork[17] = al_load_bitmap("Ork/Ork w prawo 4.png");

		Ork[18] = al_load_bitmap("Ork/Ork w lewo.png");
		Ork[19] = al_load_bitmap("Ork/Ork w lewo 1.png");
		Ork[20] = al_load_bitmap("Ork/Ork w lewo 2.png");
		Ork[21] = al_load_bitmap("Ork/Ork w lewo.png");
		Ork[22] = al_load_bitmap("Ork/Ork w lewo 3.png");
		Ork[23] = al_load_bitmap("Ork/Ork w lewo 4.png");

		Ork[24] = al_load_bitmap("Ork/Ork w dol prawo.png");
		Ork[25] = al_load_bitmap("Ork/Ork w dol prawo 1.png");
		Ork[26] = al_load_bitmap("Ork/Ork w dol prawo 2.png");
		Ork[27] = al_load_bitmap("Ork/Ork w dol prawo.png");
		Ork[28] = al_load_bitmap("Ork/Ork w dol prawo 3.png");
		Ork[29] = al_load_bitmap("Ork/Ork w dol prawo 4.png");

		Ork[30] = al_load_bitmap("Ork/Ork w dol lewo.png");
		Ork[31] = al_load_bitmap("Ork/Ork w dol lewo 1.png");
		Ork[32] = al_load_bitmap("Ork/Ork w dol lewo 2.png");
		Ork[33] = al_load_bitmap("Ork/Ork w dol lewo.png");
		Ork[34] = al_load_bitmap("Ork/Ork w dol lewo 3.png");
		Ork[35] = al_load_bitmap("Ork/Ork w dol lewo 4.png");

		Ork[36] = al_load_bitmap("Ork/Ork w gore prawo.png");
		Ork[37] = al_load_bitmap("Ork/Ork w gore prawo 1.png");
		Ork[38] = al_load_bitmap("Ork/Ork w gore prawo 2.png");
		Ork[39] = al_load_bitmap("Ork/Ork w gore prawo.png");
		Ork[40] = al_load_bitmap("Ork/Ork w gore prawo 3.png");
		Ork[41] = al_load_bitmap("Ork/Ork w gore prawo 4.png");

		Ork[42] = al_load_bitmap("Ork/Ork w gore lewo.png");
		Ork[43] = al_load_bitmap("Ork/Ork w gore lewo 1.png");
		Ork[44] = al_load_bitmap("Ork/Ork w gore lewo 2.png");
		Ork[45] = al_load_bitmap("Ork/Ork w gore lewo.png");
		Ork[46] = al_load_bitmap("Ork/Ork w gore lewo 3.png");
		Ork[47] = al_load_bitmap("Ork/Ork w gore lewo 4.png");

		for (int i = 0; i < 48; i++)
		{
			al_convert_mask_to_alpha(Ork[i], al_map_rgb(255, 100, 255));
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
			mapa_x -= wsad[D] * 2;
			mapa_x += wsad[A] * 2;
			mapa_y -= wsad[S] * 2;
			mapa_y += wsad[W] * 2;

			if (mapa_x < -250 || mapa_y < -800 || mapa_x > 0 || mapa_y > 0)
			{/*
				pos_x += wsad[D] * (predkosc_linia_prosta / 2);
				pos_x -= wsad[A] * (predkosc_linia_prosta / 2);  // mapa zablowana
				pos_y += wsad[S] * (predkosc_linia_prosta / 2);
				pos_y -= wsad[W] * (predkosc_linia_prosta / 2);*/
			}
			else
			{
				pos_x -= wsad[D] * (2);
				pos_x += wsad[A] * (2);  // mapa ruchoma
				pos_y -= wsad[S] * (2);
				pos_y += wsad[W] * (2);
			}

		
			if (wsad[W] == true)
			{
				WhereXY(pos_x, pos_y, mapa_x, mapa_y, whereX, whereY);
				RuchPostaciW(pos_x, pos_y, whereX, whereY, predkosc_linia_prosta);
			}
			if (wsad[S] == true)
			{
				WhereXY(pos_x, pos_y, mapa_x, mapa_y, whereX, whereY);
				RuchPostaciS( pos_x, pos_y, whereX, whereY,predkosc_linia_prosta);
			}
			if (wsad[A] == true)
			{
				WhereXY(pos_x, pos_y, mapa_x, mapa_y, whereX, whereY);
				RuchPostaciA(  pos_x,  pos_y,   whereX,  whereY,  predkosc_linia_prosta);

			}
			if (wsad[D] == true)
			{
				WhereXY(pos_x, pos_y, mapa_x, mapa_y, whereX, whereY);
				RuchPostaciD(pos_x, pos_y,whereX,whereY, predkosc_linia_prosta);
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
						al_draw_bitmap(Ork[6], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 0 && ilosc_klatek <= 10)
					{
						al_draw_bitmap(Ork[7], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 10 && ilosc_klatek <= 20)
					{
						al_draw_bitmap(Ork[8], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 20 && ilosc_klatek <= 30)
					{
						al_draw_bitmap(Ork[9], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 30 && ilosc_klatek <= 40)
					{
						al_draw_bitmap(Ork[10], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 40 && ilosc_klatek <= 50)
					{
						al_draw_bitmap(Ork[11], pos_x, pos_y, 0);
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
						al_draw_bitmap(Ork[0], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 0 && ilosc_klatek <= 10)
					{
						al_draw_bitmap(Ork[1], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 10 && ilosc_klatek <= 20)
					{
						al_draw_bitmap(Ork[2], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 20 && ilosc_klatek <= 30)
					{
						al_draw_bitmap(Ork[3], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 30 && ilosc_klatek <= 40)
					{
						al_draw_bitmap(Ork[4], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 40 && ilosc_klatek <= 50)
					{
						al_draw_bitmap(Ork[5], pos_x, pos_y, 0);
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
						al_draw_bitmap(Ork[12], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 0 && ilosc_klatek <= 10)
					{
						al_draw_bitmap(Ork[13], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 10 && ilosc_klatek <= 20)
					{
						al_draw_bitmap(Ork[14], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 20 && ilosc_klatek <= 30)
					{
						al_draw_bitmap(Ork[15], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 30 && ilosc_klatek <= 40)
					{
						al_draw_bitmap(Ork[16], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 40 && ilosc_klatek <= 50)
					{
						al_draw_bitmap(Ork[17], pos_x, pos_y, 0);
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
						al_draw_bitmap(Ork[18], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 0 && ilosc_klatek <= 10)
					{
						al_draw_bitmap(Ork[19], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 10 && ilosc_klatek <= 20)
					{
						al_draw_bitmap(Ork[20], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 20 && ilosc_klatek <= 30)
					{
						al_draw_bitmap(Ork[21], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 30 && ilosc_klatek <= 40)
					{
						al_draw_bitmap(Ork[22], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 40 && ilosc_klatek <= 50)
					{
						al_draw_bitmap(Ork[23], pos_x, pos_y, 0);
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
						al_draw_bitmap(Ork[24], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 0 && ilosc_klatek <= 10)
					{
						al_draw_bitmap(Ork[25], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 10 && ilosc_klatek <= 20)
					{
						al_draw_bitmap(Ork[26], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 20 && ilosc_klatek <= 30)
					{
						al_draw_bitmap(Ork[27], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 30 && ilosc_klatek <= 40)
					{
						al_draw_bitmap(Ork[28], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 40 && ilosc_klatek <= 50)
					{
						al_draw_bitmap(Ork[29], pos_x, pos_y, 0);
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
						al_draw_bitmap(Ork[30], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 0 && ilosc_klatek <= 10)
					{
						al_draw_bitmap(Ork[31], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 10 && ilosc_klatek <= 20)
					{
						al_draw_bitmap(Ork[32], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 20 && ilosc_klatek <= 30)
					{
						al_draw_bitmap(Ork[33], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 30 && ilosc_klatek <= 40)
					{
						al_draw_bitmap(Ork[34], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 40 && ilosc_klatek <= 50)
					{
						al_draw_bitmap(Ork[35], pos_x, pos_y, 0);
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
						al_draw_bitmap(Ork[36], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 0 && ilosc_klatek <= 10)
					{
						al_draw_bitmap(Ork[37], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 10 && ilosc_klatek <= 20)
					{
						al_draw_bitmap(Ork[38], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 20 && ilosc_klatek <= 30)
					{
						al_draw_bitmap(Ork[39], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 30 && ilosc_klatek <= 40)
					{
						al_draw_bitmap(Ork[40], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 40 && ilosc_klatek <= 50)
					{
						al_draw_bitmap(Ork[41], pos_x, pos_y, 0);
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
						al_draw_bitmap(Ork[42], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 0 && ilosc_klatek <= 10)
					{
						al_draw_bitmap(Ork[43], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 10 && ilosc_klatek <= 20)
					{
						al_draw_bitmap(Ork[44], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 20 && ilosc_klatek <= 30)
					{
						al_draw_bitmap(Ork[45], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 30 && ilosc_klatek <= 40)
					{
						al_draw_bitmap(Ork[46], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 40 && ilosc_klatek <= 50)
					{
						al_draw_bitmap(Ork[47], pos_x, pos_y, 0);
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
						al_draw_bitmap(Ork[0], pos_x, pos_y, 0);
					if (ostatnia_pozycja == W)
						al_draw_bitmap(Ork[6], pos_x, pos_y, 0);
					if (ostatnia_pozycja == S)
						al_draw_bitmap(Ork[0], pos_x, pos_y, 0);
					if (ostatnia_pozycja == A)
						al_draw_bitmap(Ork[18], pos_x, pos_y, 0);
					if (ostatnia_pozycja == D)
						al_draw_bitmap(Ork[12], pos_x, pos_y, 0);
				}
				if (wsad[W] == true && wsad[S] == true && wsad[D] == false && wsad[A] == false)
				{
					//predkosc_linia_prosta = 3;
					if (ostatnia_pozycja != S && ostatnia_pozycja != W && ostatnia_pozycja != D && ostatnia_pozycja != A)
						al_draw_bitmap(Ork[0], pos_x, pos_y, 0);
					if (ostatnia_pozycja == W)
						al_draw_bitmap(Ork[6], pos_x, pos_y, 0);
					if (ostatnia_pozycja == S)
						al_draw_bitmap(Ork[0], pos_x, pos_y, 0);
					if (ostatnia_pozycja == A)
						al_draw_bitmap(Ork[18], pos_x, pos_y, 0);
					if (ostatnia_pozycja == D)
						al_draw_bitmap(Ork[12], pos_x, pos_y, 0);
				}
				if (wsad[W] == false && wsad[S] == false && wsad[D] == true && wsad[A] == true)
				{
					//predkosc_linia_prosta = 3;
					if (ostatnia_pozycja != S && ostatnia_pozycja != W && ostatnia_pozycja != D && ostatnia_pozycja != A)
						al_draw_bitmap(Ork[0], pos_x, pos_y, 0);
					if (ostatnia_pozycja == W)
						al_draw_bitmap(Ork[6], pos_x, pos_y, 0);
					if (ostatnia_pozycja == S)
						al_draw_bitmap(Ork[0], pos_x, pos_y, 0);
					if (ostatnia_pozycja == A)
						al_draw_bitmap(Ork[18], pos_x, pos_y, 0);
					if (ostatnia_pozycja == D)
						al_draw_bitmap(Ork[12], pos_x, pos_y, 0);
				}
				if (wsad[W] == true && wsad[S] == true && wsad[D] == true && wsad[A] == false)
				{
					ilosc_klatek++;
					ostatnia_pozycja = D;
					//predkosc_linia_prosta = 2;
					if (ilosc_klatek == 0)
					{
						al_draw_bitmap(Ork[12], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 0 && ilosc_klatek <= 10)
					{
						al_draw_bitmap(Ork[13], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 10 && ilosc_klatek <= 20)
					{
						al_draw_bitmap(Ork[14], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 20 && ilosc_klatek <= 30)
					{
						al_draw_bitmap(Ork[15], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 30 && ilosc_klatek <= 40)
					{
						al_draw_bitmap(Ork[16], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 40 && ilosc_klatek <= 50)
					{
						al_draw_bitmap(Ork[17], pos_x, pos_y, 0);
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
						al_draw_bitmap(Ork[18], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 0 && ilosc_klatek <= 10)
					{
						al_draw_bitmap(Ork[19], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 10 && ilosc_klatek <= 20)
					{
						al_draw_bitmap(Ork[20], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 20 && ilosc_klatek <= 30)
					{
						al_draw_bitmap(Ork[21], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 30 && ilosc_klatek <= 40)
					{
						al_draw_bitmap(Ork[22], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 40 && ilosc_klatek <= 50)
					{
						al_draw_bitmap(Ork[23], pos_x, pos_y, 0);
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
						al_draw_bitmap(Ork[6], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 0 && ilosc_klatek <= 10)
					{
						al_draw_bitmap(Ork[7], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 10 && ilosc_klatek <= 20)
					{
						al_draw_bitmap(Ork[8], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 20 && ilosc_klatek <= 30)
					{
						al_draw_bitmap(Ork[9], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 30 && ilosc_klatek <= 40)
					{
						al_draw_bitmap(Ork[10], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 40 && ilosc_klatek <= 50)
					{
						al_draw_bitmap(Ork[11], pos_x, pos_y, 0);
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
						al_draw_bitmap(Ork[0], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 0 && ilosc_klatek <= 10)
					{
						al_draw_bitmap(Ork[1], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 10 && ilosc_klatek <= 20)
					{
						al_draw_bitmap(Ork[2], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 20 && ilosc_klatek <= 30)
					{
						al_draw_bitmap(Ork[3], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 30 && ilosc_klatek <= 40)
					{
						al_draw_bitmap(Ork[4], pos_x, pos_y, 0);
					}
					if (ilosc_klatek > 40 && ilosc_klatek <= 50)
					{
						al_draw_bitmap(Ork[5], pos_x, pos_y, 0);
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
						al_draw_bitmap(Ork[0], pos_x, pos_y, 0);
					if (ostatnia_pozycja == W)
						al_draw_bitmap(Ork[6], pos_x, pos_y, 0);
					if (ostatnia_pozycja == S)
						al_draw_bitmap(Ork[0], pos_x, pos_y, 0);
					if (ostatnia_pozycja == A)
						al_draw_bitmap(Ork[18], pos_x, pos_y, 0);
					if (ostatnia_pozycja == D)
						al_draw_bitmap(Ork[12], pos_x, pos_y, 0);
				}

			}
		}
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
		
			
			
			//pozycja postaci na mapie
			float x = pos_x - mapa_x;
			float y = pos_y - mapa_y;
			al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 20, ALLEGRO_ALIGN_LEFT,
				"X postaci: %.0f", x);
			al_draw_textf(font24, al_map_rgb(255, 255, 255), 400, 50, ALLEGRO_ALIGN_RIGHT,
				"Y postaci: %.0f", y);


			al_draw_textf(font24, al_map_rgb(255, 255, 255), 500, 100, ALLEGRO_ALIGN_LEFT,
				"X postaci: %i", whereX);
			al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 150, ALLEGRO_ALIGN_RIGHT,
				"Y postaci: %i", whereY);


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
		al_destroy_bitmap(Ork[i]);
	}
	
	al_destroy_bitmap(bgImage);
	return 0;
}

void WhereXY(float pos_x, float pos_y, int mapa_x, int mapa_y,int & whereX,int & whereY)
{
	
	

	for (int i = 0; i < 30; i++)
	{
		int pozycja = pos_x - mapa_x;
		if ((pozycja > SiatkaX[i]) && (pozycja < SiatkaX[i + 1]))
		{
			whereX = i; //w jakiej pozycji siatki jestem na X
			break;
		}
	}

	for (int i = 0; i < 30; i++)
	{
		int pozycja = pos_y - mapa_y;
		if ((pozycja >(SiatkaY[i])) && (pozycja < (SiatkaY[i + 1])))
		{
			whereY = i; //w jakiej pozycji siatki jestem na Y
			break;
		}
	}
}

void RuchPostaciW(float pos_x, float & pos_y,int whereX, int whereY, int predkosc_linia_prosta)
{
	float gdzie_bedeY = pos_y - predkosc_linia_prosta / 2;
	bool obszar_mapy = ((gdzie_bedeY) >0) ? 1 : 0;

	if ((can[whereX][whereY] == false) && (can[whereX + 1][whereY] == false) && (obszar_mapy == true))
	{
		if ((pos_x >= (whereX * 50)) && (pos_x <= ((whereX + 2) * 50)))
		{
			pos_y += 0;
		}/*
		else
		{
			pos_y -= predkosc_linia_prosta / 2;
		}*/
	}
	else if ((can[whereX][whereY] == false) && (obszar_mapy == true))
	{
		if (pos_x < (((whereX + 1) * 50)))
			pos_y += 0;
		else
			pos_y -= predkosc_linia_prosta / 2; // tutaj
	}
	else if ((can[whereX + 1][whereY] == false) && (obszar_mapy == true))
	{
		if ((pos_x + 50 > (((whereX + 1) * 50))))
			pos_y += 0;
		/*else
			pos_y -= predkosc_linia_prosta / 2;*/
    }
else if (obszar_mapy == true)
pos_y -= predkosc_linia_prosta / 2;
else if (obszar_mapy == false)
pos_y = 0;
}


void RuchPostaciS(float pos_x, float & pos_y, int whereX, int whereY, int predkosc_linia_prosta)
{
	float gdzie_bedeY = pos_y + predkosc_linia_prosta/2;
	bool obszar_mapy = ((gdzie_bedeY) < (1500 - 50 + 1)) ? 1 : 0;

	if ((can[whereX][whereY + 1] == false) && (can[whereX + 1][whereY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_x >= (whereX * 50)) && (pos_x <= ((whereX + 2) * 50)))
			pos_y -= 0;
		/*else
			pos_y += predkosc_linia_prosta / 2;*/
	}
	else if ((can[whereX][whereY + 1] == false) && (obszar_mapy == true))
	{
		if (pos_x < (((whereX + 1) * 50)))
			pos_y -= 0;
		else
			pos_y += predkosc_linia_prosta / 2;

	}
	else if ((can[whereX + 1][whereY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_x + 50) >(((whereX + 1) * 50)))
			pos_y -= 0;
		/*else
			pos_y  += predkosc_linia_prosta / 2;*/
	}
	else if (obszar_mapy == true)
		pos_y += predkosc_linia_prosta / 2;
	else if (obszar_mapy == false)
		pos_y = 1500 - 50 + 1;
}

void RuchPostaciA(float & pos_x, float pos_y, int whereX, int whereY, int predkosc_linia_prosta)
{
	float gdzie_bedeY = pos_x - predkosc_linia_prosta / 2;
	bool obszar_mapy = ((gdzie_bedeY) > 0) ? 1 : 0;

	if ((can[whereX][whereY] == false) && (can[whereX][whereY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_y >= (whereY * 50)) && (pos_y <= ((whereY + 2) * 50)))
			pos_x += 0;
		/*else
			pos_x -= predkosc_linia_prosta / 2;*/
	}
	else if ((can[whereX][whereY] == false) && (obszar_mapy == true))
	{
		if (pos_y < (((whereY + 1) * 50)))
			pos_x += 0;
		else
			pos_x -= predkosc_linia_prosta / 2;

	}
	else if ((can[whereX][whereY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_y + 50) >(((whereY + 1) * 50)))
			pos_x += 0;
		/*else
			pos_x -= predkosc_linia_prosta / 2;*/
	}
	else if (obszar_mapy == true)
		pos_x -= predkosc_linia_prosta / 2;
	else if (obszar_mapy == false)
		pos_x = 0;
}
void RuchPostaciD(float & pos_x, float pos_y, int whereX, int whereY, int predkosc_linia_prosta)
{
	float gdzie_bedeY = pos_x + predkosc_linia_prosta / 2;
	bool obszar_mapy = ((gdzie_bedeY) < (1500 - 50 + 1)) ? 1 : 0;

	if ((can[whereX + 1][whereY] == false) && (can[whereX + 1][whereY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_y >= (whereY * 50)) && (pos_y <= ((whereY + 2) * 50)))
			pos_x -= 0;
		/*else
			pos_x += predkosc_linia_prosta / 2;*/
	}
	else if ((can[whereX + 1][whereY] == false) && (obszar_mapy == true))
	{
		if (pos_y< (((whereY + 1) * 50) ))
			pos_x -= 0;
		else
			pos_x += predkosc_linia_prosta / 2;

	}
	else if ((can[whereX + 1][whereY + 1] == false) && (obszar_mapy == true))
	{
		if ((pos_y + 50) >(((whereY + 1) * 50)))
			pos_x -= 0;
		/*else
			pos_x += predkosc_linia_prosta / 2;*/
	}
	else if (obszar_mapy == true)
		pos_x += predkosc_linia_prosta / 2;
	else if (obszar_mapy == false)
		pos_x = 1500 - 50 + 1;
}
/*

int gdzie_bedeY = tank.start_X + tank.speed;
if ((can_i_do_this[tank.where_X + 1][tank.where_Y] == false) && (can_i_do_this[tank.where_X + 1][tank.where_Y + 1] == false) && (obszar_mapy == true))
{
if ((tank.start_Y >= (tank.where_Y * 60)) && (tank.start_Y <= ((tank.where_Y + 2) * 60)))
tank.start_X -= 0;
else
tank.start_X += tank.speed;
}


bool Kolizja(int x1, int y1, int x2, int y2)
{
if (x2 <= x1 + 50 && x2 > x1 && y2 >= y1 && y2 <= y1 + 50)
return true;
else if (x2 <= x1 + 50 && x2 > x1 && y2 + 50 >= y1 && y2 + 50 <= y1 + 50)
return true;
else if (x2 + 50 <= x1 + 50 && x2 + 50 > x1 && y2 >= y1 && y2 <= y1 + 50)
return true;
else if (x2 + 50 <= x1 + 50 && x2 + 50 > x1 && y2 + 50 >= y1 && y2 + 50 <= y1 + 50)
return true;
else
return false;
};*/