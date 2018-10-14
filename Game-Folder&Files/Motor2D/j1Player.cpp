#include "j1Player.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1FadeToBlack.h"
#include "j1Collision.h"

#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"


j1Player::j1Player() :j1Module()
{
	name.create("player");
}

j1Player::~j1Player()
{
}

bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	path.create(config.child("path").child_value());

	return ret;
}

bool j1Player::Start()
{
	bool ret = true;

	pugi::xml_parse_result result = player_file.load_file(path.GetString());
	pugi::xml_node	player_node = player_file.child("player");

	if (result == NULL)
		ret = false;

	else
	{
		playerSpriteSheet = App->tex->Load(player_node.child("image").attribute("source").value());

		if (playerSpriteSheet == nullptr)
			ret = false;

		else
		{
			idle_animation = LoadAnimation("idle");
			jump_animation = LoadAnimation("jump");
			run_animation = LoadAnimation("run");
			dead_animation = LoadAnimation("dead");
			air_animation = LoadAnimation("air");

			player_coll = App->collision->AddCollider({ coll_rect.x, coll_rect.y, coll_rect.w, coll_rect.h }, coll_type, App->player);
		}
	}
	return ret;
}

bool j1Player::PreUpdate()
{
	DebugInput();
	if (!godMode)CheckKeyboardState();

	return true;
}

bool j1Player::Update(float dt)
{

}