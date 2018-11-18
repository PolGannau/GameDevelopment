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
	uchar* idata = nullptr;

	switch (id) 
	{
	case 1:
		App->map->Load("MapScene1.tmx");
		int w, h;
		if (App->map->WalkabilityMap(w, h, &idata))
			RELEASE_ARRAY(idata);
		break;
	case 2:
		App->map->Load("MapScene2.tmx");
		break;
	}

	
	for (int i = 0; i < App->map->data.flyingEnemies_Position.count(); i++)
	{
		App->entitymanager->CreateEntity(Entity_TYPE::GROUND_ENEMY);
		j1Entity* ground_enemy = App->entitymanager->entities.end->data;
		ground_enemy->position = App->map->data.flyingEnemies_Position.At(i)->data;
	}

	for (int i = 0; i < App->map->data.groundEnemies_Position.count(); i++)
	{
		App->entitymanager->CreateEntity(Entity_TYPE::FLYING_ENEMY);
		j1Entity* flying_enemy = App->entitymanager->entities.end->data;
		flying_enemy->position = App->map->data.groundEnemies_Position.At(i)->data;
	}

	App->entitymanager->player->position = { App->map->data.player_position.x, App->map->data.player_position.y };
	App->entitymanager->player->death = false;

	App->render->camera.x = App->map->data.camera_position.x;
	App->render->camera.y = App->map->data.camera_position.y;

	App->audio->PlayMusic("audio/music/maintheme.ogg", 0.1F);

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

