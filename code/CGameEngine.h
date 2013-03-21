#ifndef CGAMEENGINE_H
#define CGAMEENGINE_H

#include <allegro5\allegro.h>
#include <vector>

using namespace std;

class CGameState;

class CGameEngine
{
public:

	bool Init(const char* title, int width=620, int height=660, int bpp=0);
	void Cleanup();

	void ChangeState(CGameState* state);
	void PushState(CGameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() {
		return c_running;
	}
	void Quit() {
		c_running = false;
	}

	ALLEGRO_DISPLAY *display;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT_QUEUE *event_queue;

private:
	// the stack of states
	vector<CGameState*> states;

	bool c_running;
};

#endif
