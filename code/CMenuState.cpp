#include <stdio.h>
#include <string.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

#include "CGameEngine.h"
#include "CGameState.h"
#include "CPlayState.h"
#include "CScoreState.h"
#include "CMenuState.h"

CMenuState CMenuState::c_MenuState;

// pass these to the menu items
void Play(CGameEngine* game);
void HighScores(CGameEngine* game);
void Quit(CGameEngine* game);

void CMenuState::Init()
{
	bg = al_load_bitmap("Splash.bmp");
	if(!bg) {
		bg = al_load_bitmap("C:\\Users\\Chase\\Documents\\Visual Studio 2010\\Projects\\Tetris\\Splash.bmp");
		if(!bg)
			fprintf(stderr, "failed to load Splash.bmp\n");
	}

	items = new CMenuItem*[3];
	menuIndex = 0;

	for(int i = 0; i < 3; i++)
		items[i] = new CMenuItem();

	items[0]->Init("Play!", 100, 500, Play);
	items[1]->Init("High Scores!", 250, 500, HighScores);
	items[2]->Init("Exit :(", 450, 500, Quit);

	items[menuIndex]->Select();

	printf("CMenuState Init\n");
}

void CMenuState::Cleanup()
{
	al_destroy_bitmap(bg);

	for(int i = 0; i < 3; i++) {
		items[i]->Cleanup();
		delete items[i];
	}
	delete items;

	printf("CMenuState Cleanup\n");
}

void CMenuState::Pause()
{
	printf("CMenuState Pause\n");
}

void CMenuState::Resume()
{
	printf("CMenuState Resume\n");
}

void CMenuState::HandleEvents(CGameEngine* game)
{
	ALLEGRO_EVENT ev;
	
	// wait for an event (mouse movement, key press, etc.)
	al_wait_for_event(game->event_queue, &ev);

	switch(ev.type) {
	case ALLEGRO_EVENT_DISPLAY_CLOSE:
		game->Quit();
		break;

	case ALLEGRO_EVENT_KEY_DOWN:
		if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			game->Quit();

		else if(ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			NextItem();

		else if(ev.keyboard.keycode == ALLEGRO_KEY_LEFT)
			PrevItem();

		else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
			items[menuIndex]->Activate(game);

		break;
	}
}

void CMenuState::Update(CGameEngine* game) 
{
	
}

void CMenuState::NextItem()
{
	items[menuIndex]->DeSelect();

	menuIndex++;

	if(menuIndex > 2)
		menuIndex = 0;

	items[menuIndex]->Select();
}

void CMenuState::PrevItem()
{
	items[menuIndex]->DeSelect();

	menuIndex--;
	
	if(menuIndex < 0)
		menuIndex = 2;

	items[menuIndex]->Select();
}

void CMenuState::Draw(CGameEngine* game) 
{
	al_clear_to_color(al_map_rgb(255, 255, 255));
	//al_draw_bitmap(bg, 0, 0, 0);

	// draw menu items
	for(int i = 0; i < 3; i++)
		items[i]->Draw();

	al_flip_display();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//
// Start of the CMenuItem class
//
//-------------------------------------------------------------------
//-------------------------------------------------------------------

void CMenuItem::Init(char* t, int X, int Y, void (*e)(CGameEngine* game))
{
	text = new char[32];
	strcpy(text, t);

	callback = e;

	x = X;
	y = Y;

	font = al_load_font("RevoPop.ttf",14,0);
	if(!font) {
		font = al_load_font("C:\\Users\\Chase\\Documents\\Visual Studio 2010\\Projects\\Tetris\\RevoPop.ttf",14,0);

		if(!font)
			fprintf(stderr, "Could not load font");
	}

	color = al_map_rgb(0, 0, 0);
}

void CMenuItem::Cleanup()
{
	delete[] text;

	printf("Items cleaned up");
}

void CMenuItem::Select()
{
	color = al_map_rgb(64, 64, 64);
	y -= 10;
}

void CMenuItem::DeSelect()
{
	color = al_map_rgb(0, 0, 0);
	y += 10;
}

void CMenuItem::Activate(CGameEngine* game)
{
	callback(game);
}

void CMenuItem::Draw()
{
	al_draw_text(font, color, x, y, 0, text);
}


// the callback functions for each menu item

void Play(CGameEngine* game)
{
	game->PushState(CPlayState::Instance());
}

void HighScores(CGameEngine* game)
{
	game->PushState(CScoreState::Instance());
}

void Quit(CGameEngine* game)
{
	game->Quit();
}