#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

#include "CGameState.h"
#include "CGameEngine.h"

#define FRAMERATE		60

bool CGameEngine::Init(const char* title, int width, int height, int bpp)
{
	int flags = 0;

	// initialize allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro\n");
		return false;
	}

	// load allegro image addon
	if(!al_init_image_addon()) {
		fprintf(stderr, "failed to initialize allegro image addon\n");
		return false;
	}

	// load allegro primitives addon
	if(!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize allegro primitives addon\n");
		return false;
	}

	// load allegro font addon
	al_init_font_addon(); // this addon is invinsible, it requires no fail-safe

	// load allegro ttf addon, allows for .ttf font support (why the fuck is this a seperate addon from font?)
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to initialize allegro ttf addon\n");
		return false;
	}

	// install mouse
	if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize the mouse\n");
		return false;
	}
	
	// install keyboard
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard\n");
		return false;
	}

	//enabling multisampling 
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_SUGGEST);

	// create the display
	display = al_create_display(620, 660);
	if(!display) {
		fprintf(stderr, "failed to create display\n");
		return false;
	}

	// create the timer
	timer = al_create_timer(1.0 / FRAMERATE);
	if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return false;
	}

	// create the event queue
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return false;
	}

	// tie the display to the event queue
	al_register_event_source(event_queue, al_get_display_event_source(display));

	// tie the timer to the event queue
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// tie the mouse to the event queue
	al_register_event_source(event_queue, al_get_mouse_event_source());

	// tie the keyboard to the event queue
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	// set the title bar text
	al_set_window_title(display, title);

	c_running = true;

	// start the timer
	al_start_timer(timer);

	printf("CGameEngine Init\n");
	return true;
}

void CGameEngine::Cleanup()
{
	// cleanup the all states
	while (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	printf("CGameEngine Cleanup\n");
	al_destroy_display(display);
}

void CGameEngine::ChangeState(CGameState* state) 
{
	// cleanup the current state
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void CGameEngine::PushState(CGameState* state)
{
	// pause current state
	if (!states.empty()) {
		states.back()->Pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void CGameEngine::PopState()
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if ( !states.empty() ) {
		states.back()->Resume();
	}
}


void CGameEngine::HandleEvents() 
{
	states.back()->HandleEvents(this);
}

void CGameEngine::Update() 
{
	states.back()->Update(this);
}

void CGameEngine::Draw() 
{
	states.back()->Draw(this);
}
