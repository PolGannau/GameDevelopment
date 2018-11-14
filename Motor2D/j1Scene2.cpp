#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1SceneManager.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Player.h"

j1Scene2::j1Scene2() : j1Module()
{
	id = 2;
}

// Destructor
j1Scene2::~j1Scene2()
{}

// Called before render is available
bool j1Scene2::Awake()
{
	LOG("Loading Scene 2");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene2::Start()
{
	bool ret = true;

	App->map->Load("MapScene2.tmx");
	App->player->position = { App->map->data.player_position.x, App->map->data.player_position.y };
	App->player->death = false;

	App->render->camera.x = App->map->data.camera_position.x;
	App->render->camera.y = App->map->data.camera_position.y;

	App->audio->PlayMusic("audio/music/maintheme.ogg", 0.1f);

	return ret;
}

// Called each loop iteration
bool j1Scene2::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		App->scenemanager->LoadScene(this, App->scene);

	return true;
}

// Called each loop iteration
bool j1Scene2::Update(float dt)
{
	App->render->UpdateCamera();

	if (App->player->position.x > App->map->data.win_position.x)
		App->scenemanager->LoadScene(this, App->scene);

	App->map->Draw();

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene2::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene2::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
