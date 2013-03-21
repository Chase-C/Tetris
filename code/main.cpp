#include "CGameEngine.h"
#include "CInitState.h"

int main(int argc, char *argv[])
{
	CGameEngine game;

	// initialize the engine
	if(!game.Init("Tetris v0.1")) {
		return -1;
	}

	// load the intro
	game.ChangeState(CInitState::Instance());

	// main loop
	while(game.Running())
	{
		game.HandleEvents();
		game.Update();
		if(game.Running())
			game.Draw();
	}

	// cleanup the engine
	game.Cleanup();

	return 0;
}