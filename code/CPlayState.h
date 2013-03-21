#ifndef CPLAYSTATE_H
#define CPLAYSTATE_H

#include <allegro5\allegro.h>
#include "CGameState.h"

class CGrid;
class CDisplay;

class CPlayState : public CGameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void Quit(CGameEngine* game);

	void HandleEvents(CGameEngine* game);
	void Update(CGameEngine* game);
	void Draw(CGameEngine* game);

	static CPlayState* Instance() {
		return &c_PlayState;
	}

protected:
	CPlayState() { }

private:
	static CPlayState c_PlayState;

	ALLEGRO_FONT* font;

	double gAlpha;
	double time, fadeTime;

	ALLEGRO_BITMAP* bg;
	CGrid* grid;
	CDisplay* display;
};

#endif