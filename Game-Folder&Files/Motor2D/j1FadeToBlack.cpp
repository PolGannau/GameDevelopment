#include <math.h>
#include "j1App.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"

#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

#include "p2Log.h"

j1FadeToBlack :: j1FadeToBlack() : j1Module() { name.create("fadetoblack"); }

j1FadeToBlack :: ~j1FadeToBlack() {}

bool j1FadeToBlack :: Start()
{
	uint		screen_width = 0;
	uint		screen_height = 0;
	LOG("Starting Fade Screen...");

	App->win.GetWindowSize(screen_width, screen_height);
}