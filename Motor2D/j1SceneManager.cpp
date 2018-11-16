#include <math.h>
#include "j1App.h"
#include "j1SceneManager.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Input.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

j1SceneManager::j1SceneManager()
{
	name.create("scene");
}

j1SceneManager::~j1SceneManager()
{}

// Load assets
bool j1SceneManager::Start()
{
	screen = { 0, 0, App->win->GetScreenWidth(),  App->win->GetScreenHeight() };
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1SceneManager::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		ReloadScene(current_scene);

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		LoadFirstScene(current_scene);

	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
		case fade_step::fade_to_black:
		{
			if (now >= total_time)
			{
				module_disable->Disable();
				App->map->CleanUp();
				module_enable->Enable();
				current_scene = module_enable;
				total_time += total_time;
				start_time = SDL_GetTicks();
				current_step = fade_step::fade_from_black;
			}
		} break;

		case fade_step::fade_from_black:
		{
			normalized = 1.0F - normalized;

			if (now >= total_time)
				current_step = fade_step::none;
		} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0F));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1SceneManager::LoadScene(j1Module* module_off, j1Module* module_on, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		module_disable = module_off;
		module_enable = module_on;
		ret = true;
	}

	return ret;
}

//faster loading scene, without transition
bool j1SceneManager::LoadSceneWithoutFade(j1Module* module_off, j1Module* module_on) 
{
	module_off->Disable();
	App->map->CleanUp();
	module_on->Enable();
	current_scene = module_on;
	
	App->entitymanager->player->SetPosition(App->entitymanager->player->saved_position.x, App->entitymanager->player->saved_position.y);

	return true;
}

void j1SceneManager::ReloadScene(j1Module* module)
{
	LoadScene(module, module, 1.0f);
}

void j1SceneManager::LoadFirstScene(j1Module* module_off)
{
	LoadScene(module_off, App->scene);
}

// Load Scene State
bool j1SceneManager::Load(pugi::xml_node& scene)
{
	id = scene.attribute("id").as_int();

	if (id != current_scene->id)
	{
		switch (current_scene->id)
		{
		case 1: App->scenemanager->LoadSceneWithoutFade(current_scene, App->scene2);
			break;

		case 2: App->scenemanager->LoadSceneWithoutFade(current_scene, App->scene); break;
		}
	}
	return true;
}

// Save Scene State
bool j1SceneManager::Save(pugi::xml_node& scene) const
{
	scene.append_attribute("id") = current_scene->id;
	return true;
}