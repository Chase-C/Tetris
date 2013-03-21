#ifndef CSCORESTATE_H
#define CSCORESTATE_H

#include <stdio.h>
#include <allegro5\allegro.h>

#include "CGameState.h"
#include "CMenuState.h"

class CScoreState : public CGameState
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

	void ReadFile();
	void WriteToFile();

	static CScoreState* Instance() {
		return &c_ScoreState;
	}

protected:
	CScoreState() { }

private:
	static CScoreState c_ScoreState;

	ALLEGRO_FILE* file;

	ALLEGRO_FONT* font1;
	ALLEGRO_FONT* font2;

	char** scores;
	int numScores;

	CMenuItem** items;
	int menuIndex;

	void SwitchItem();
};

#endif