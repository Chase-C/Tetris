#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

#include "CGameEngine.h"
#include "CGameState.h"
#include "CInitState.h"
#include "CMenuState.h"

CInitState CInitState::c_InitState;

void CInitState::Init()
{
	bg = al_load_bitmap("Logo.bmp");
	if(!bg) {
		bg = al_load_bitmap("C:\\Users\\Chase\\Documents\\Visual Studio 2010\\Projects\\Tetris\\Logo.bmp");
		if(!bg)
			fprintf(stderr, "failed to load Logo.bmp\n");
	}

	// start off transparent
	alpha = 0;

	time = al_get_time();
	fadeTime = 1.0;

	printf("CInitState Init\n");
}

void CInitState::Cleanup()
{
	al_destroy_bitmap(bg);

	printf("CInitState Cleanup\n");
}

void CInitState::Pause()
{
	printf("CInitState Pause\n");
}

void CInitState::Resume()
{
	printf("CInitState Resume\n");
}

void CInitState::HandleEvents(CGameEngine* game)
{
	ALLEGRO_EVENT ev;
	
	// wait for an event (mouse movement, key press, etc.)
	al_wait_for_event(game->event_queue, &ev);

	switch(ev.type) {
	case ALLEGRO_EVENT_DISPLAY_CLOSE:
		game->Quit();
		break;

	case ALLEGRO_EVENT_KEY_DOWN:
		game->ChangeState(CMenuState::Instance());
		break;
	}
}

void CInitState::Update(CGameEngine* game) 
{
	double dt = al_get_time() - time;
	
	if(dt <= fadeTime) 
		alpha = (255 * dt) / fadeTime;

	else if(dt >= 5 - fadeTime) 
		alpha = 255 - ((255 * (dt - (5 - fadeTime))) / fadeTime);

	else 
		alpha = 255;


	if (alpha > 255) alpha = 255;

	if(dt > 5)
		game->ChangeState(CMenuState::Instance());
}

void CInitState::Draw(CGameEngine* game) 
{
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_draw_tinted_bitmap(bg, al_map_rgba(255, 255, 255, alpha), 200, 187, 0);

	al_flip_display();
}
