#ifndef CINTROSTATE_H
#define CINTROSTATE_H

#include <allegro5\allegro.h>
#include "CGameState.h"

class CInitState : public CGameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameEngine* game);
	void Update(CGameEngine* game);
	void Draw(CGameEngine* game);

	static CInitState* Instance() {
		return &c_InitState;
	}

protected:
	CInitState() { }

private:
	static CInitState c_InitState;

	ALLEGRO_BITMAP* bg;
	double alpha;

	double time;
	double fadeTime;
};

#endif