#ifndef CMENUSTATE_H
#define CMENUSTATE_H

#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>

#include "CGameState.h"

class CMenuItem;

class CMenuState : public CGameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameEngine* game);
	void Update(CGameEngine* game);
	void Draw(CGameEngine* game);

	static CMenuState* Instance() {
		return &c_MenuState;
	}

protected:
	CMenuState() { }

private:
	static CMenuState c_MenuState;

	CMenuItem** items;
	int menuIndex;

	ALLEGRO_BITMAP* bg;

	void NextItem();
	void PrevItem();
};


typedef void (*CallbackFunc)(CGameEngine*);

class CMenuItem
{
public:
	void Init(char* t, int X, int Y, void (*e)(CGameEngine*));
	void Cleanup();

	void Select();
	void DeSelect();

	void Activate(CGameEngine* game);

	void Draw();

private:
	int x, y;

	ALLEGRO_FONT* font;
	ALLEGRO_COLOR color;

	char* text;
	CallbackFunc callback;
};

#endif