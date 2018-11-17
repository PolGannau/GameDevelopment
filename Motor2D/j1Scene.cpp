#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1SceneManager.h"
#include "j1Player.h"

j1Scene::j1Scene() : j1Module()
{
	id = 1;
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node&)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;
	
	switch (id) 
	{
	case 1:
		App->map->Load("MapScene1.tmx");
		break;
	case 2:
		App->map->Load("MapScene2.tmx");
		break;
	}

	App->entitymanager->player->position = { App->map->data.player_position.x, App->map->data.player_position.y };
	App->entitymanager->player->death = false;

	App->render->camera.x = App->map->data.camera_position.x;
	App->render->camera.y = App->map->data.camera_position.y;

	App->audio->PlayMusic("audio/music/maintheme.ogg", 0.1f);

	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		App->scenemanager->LoadScene(2);

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	App->render->UpdateCamera();

	if (App->entitymanager->player->position.x > App->map->data.win_position.x)
	{
		App->scenemanager->LoadScene(2);
	}

	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->map->CleanUp();
	return true;
}

