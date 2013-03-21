#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

#include "CGameState.h"
#include "CMenuState.h"
#include "CScoreState.h"

CScoreState CScoreState::c_ScoreState;

void Reset(CGameEngine* game);
void Exit(CGameEngine* game);

void CScoreState::Init()
{
	font1 = al_load_font("RevoPop.ttf", 16, 0);
	if(!font1) {
		font1 = al_load_font("C:\\Users\\Chase\\Documents\\Visual Studio 2010\\Projects\\Tetris\\RevoPop.ttf", 16, 0);

		if(!font1)
			fprintf(stderr, "Could not load font\n");
	}

	font2 = al_load_font("RevoPop.ttf", 28, 0);
	if(!font2) {
		font2 = al_load_font("C:\\Users\\Chase\\Documents\\Visual Studio 2010\\Projects\\Tetris\\RevoPop.ttf", 28, 0);

		if(!font2)
			fprintf(stderr, "Could not load font\n");
	}

	items = new CMenuItem*[2];
	menuIndex = 0;

	for(int i = 0; i < 2; i++)
		items[i] = new CMenuItem();

	items[0]->Init("Reset", 150, 600, Reset);
	items[1]->Init("Main Menu", 400, 600, Exit);

	items[menuIndex]->Select();

	ReadFile();

	printf("CScoreState init\n");
}

void CScoreState::Cleanup()
{
	for(int i = 0; i < numScores; i++)
		delete scores[i];
	delete[] scores;

	printf("CScoreState cleaned up\n");
}

void CScoreState::Pause()
{
	printf("CScoreState paused\n");
}

void CScoreState::Resume()
{
	printf("CScoreState resumed\n");
}

void CScoreState::Update(CGameEngine* game)
{

}

void CScoreState::HandleEvents(CGameEngine* game)
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
			game->PopState();

		else if(ev.keyboard.keycode == ALLEGRO_KEY_LEFT || ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			SwitchItem();

		if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE || ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
			items[menuIndex]->Activate(game);

		break;
	}
}

void CScoreState::SwitchItem()
{
	items[menuIndex]->DeSelect();

	if(menuIndex == 1)
		menuIndex = 0;
	else
		menuIndex = 1;

	items[menuIndex]->Select();
}

void CScoreState::Draw(CGameEngine* game)
{
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_draw_text(font2, al_map_rgb(0, 0, 0), 230, 40, 0, "High Scores");

	for(int i = 0; i < numScores; i++) {
		al_draw_text(font1, al_map_rgb(0, 0, 0), 200, 148 + (38 * i), 0, itoa(i + 1, new char[2], 10));
		al_draw_text(font1, al_map_rgb(0, 0, 0), 236, 148 + (38 * i), 0, scores[i]);
	}

	for(int i = 0; i < 2; i++)
		items[i]->Draw();

	al_flip_display();
}

void CScoreState::ReadFile()
{
	file = al_fopen("C:\\Users\\Chase\\Documents\\Visual Studio 2010\\Projects\\Tetris\\scores.scr", "rb");
	if(!file)
		printf("File scores.scr could not be opened\n");

	numScores = al_fsize(file) / 8;
	scores = new char*[numScores];

	for(int i = 0; i < numScores; i++) {
		long* temp = new long;
		al_fread(file, temp, 8);
		scores[i] = ltoa(*temp, new char[16], 10);
	}

	al_fclose(file);

	printf("File read from, scores: %d\n",numScores);
}

void CScoreState::WriteToFile()
{
	file = al_fopen("C:\\Users\\Chase\\Documents\\Visual Studio 2010\\Projects\\Tetris\\scores.scr", "w+b");
	if(!file)
		printf("File scores.scr could not be opened\n");

	int size = 0;
	for(int i = 0; i < 10; i++) {
		long temp = 0;
		size += al_fwrite(file, &temp, 8);
	}

	al_fclose(file);

	printf("File written to, size: %d\n", size);
}

void Reset(CGameEngine* game)
{
	CScoreState::Instance()->WriteToFile();
	CScoreState::Instance()->ReadFile();
}

void Exit(CGameEngine* game)
{
	game->PopState();
}