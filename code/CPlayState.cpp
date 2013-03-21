#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_ttf.h>

#include "CGameEngine.h"
#include "CGrid.h"
#include "CDisplay.h"
#include "CGameState.h"
#include "CMenuState.h"
#include "CPlayState.h"

CPlayState CPlayState::c_PlayState;

void CPlayState::Init()
{
	bg = al_load_bitmap("Tetris Background.bmp");
	if(!bg) {
		bg = al_load_bitmap("C:\\Users\\Chase\\Documents\\Visual Studio 2010\\Projects\\Tetris\\Tetris Background.bmp");

		if(!bg)
			fprintf(stderr, "failed to load Tetris Background.bmp\n");
	}

	font = al_load_ttf_font("RevoPop.ttf", 20, 0);
	if(!font) {
		font = al_load_ttf_font("C:\\Users\\Chase\\Documents\\Visual Studio 2010\\Projects\\Tetris\\RevoPop.ttf", 20, 0);

		if(!font)
			fprintf(stderr, "Could not load font");
	}

	gAlpha = 0;
	fadeTime = 1.0;

	grid = new CGrid();
	grid->Init(290, 10, 10, 320, 640);

	display = new CDisplay();
	display->Init();

	printf("CPlayState Init\n");
}

void CPlayState::Cleanup()
{
	al_destroy_bitmap(bg);

	printf("CPlayState Cleanup\n");
}

void CPlayState::Pause()
{
	printf("CPlayState Pause\n");
}

void CPlayState::Resume()
{
	printf("CPlayState Resume\n");
}

void CPlayState::HandleEvents(CGameEngine* game)
{
	ALLEGRO_EVENT ev;
	
	// wait for an event (mouse movement, key press, etc.)
	al_wait_for_event(game->event_queue, &ev);

	switch(ev.type) {
	case ALLEGRO_EVENT_DISPLAY_CLOSE:
		game->Quit();
		break;

	case ALLEGRO_EVENT_KEY_DOWN:
		if(ev.keyboard.keycode == ALLEGRO_KEY_LEFT)
			grid->MoveBlockLeft();

		else if(ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			grid->MoveBlockRight();

		else if(ev.keyboard.keycode == ALLEGRO_KEY_UP)
			grid->RotateBlock();

		else if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN)
			grid->AccelerateBlock();

		else if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
			grid->DropBlock();

		else if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			Quit(game);

		break;

	case ALLEGRO_EVENT_KEY_UP:
		if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN)
			grid->DecelerateBlock();

		break;
	}
}

void CPlayState::Update(CGameEngine* game)
{
	if(grid->GameOver()) {
		double dt = al_get_time() - time;
		gAlpha = (255 * dt) / fadeTime;

		if(dt >= 5)
			Quit(game);
	} else {
		time = al_get_time();
		grid->Update(display);

	}
}

void CPlayState::Quit(CGameEngine* game)
{
	grid->Cleanup();

	display->UpdateHighScores();
	display->Cleanup();

	game->PopState();
}

void CPlayState::Draw(CGameEngine* game)
{
	al_draw_bitmap(bg, 0, 0, 0);

	// draw the grid
	grid->Draw();

	// draw the display
	display->Draw();

	if(gAlpha != 0) {
		al_draw_filled_rounded_rectangle(369.5, 289.5, 529.5, 369.5, 5, 5, al_map_rgb(212, 212, 212));
		al_draw_rounded_rectangle(369.5, 289.5, 529.5, 369.5, 5, 5, al_map_rgb(32, 32, 32), 2);
		al_draw_text(font, al_map_rgb(0, 0, 0), 392, 312, 0, "Game Over");
	}

	al_flip_display();
}