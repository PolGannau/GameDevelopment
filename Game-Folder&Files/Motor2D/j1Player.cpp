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

	LOG("Awakening Player Module...");

	path.create(config.child("path").child_value());

	return ret;
}

bool j1Player::Start()
{
	bool ret = true;

	LOG("Starting Player Module...");

	pugi::xml_parse_result result = player_file.load_file(path.GetString());
	pugi::xml_node	player_node = player_file.child("player");

	if (result == NULL)
		ret = false;

	else
	{
		playerSprites = App->tex->Load(player_node.child("image").attribute("source").value());

		if (playerSprites == nullptr)
			ret = false;

		else
		{
			idle_animation = LoadAnimation("idle");
			jump_animation = LoadAnimation("jump");
			run_animation = LoadAnimation("run");
			dead_animation = LoadAnimation("dead");
			air_animation = LoadAnimation("air");

			player_collider = App->collision->AddCollider({ coll_rect.x, coll_rect.y, coll_rect.w, coll_rect.h }, coll_type, App->player);
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
	PlayerActions();

	return true;
}

bool j1Player::PostUpdate()
{
	Draw();

	return true;
}

bool j1Player::CleanUp()
{
	if (playerSprites != nullptr)
	{
		App->tex->UnLoad(playerSprites);
		playerSprites = nullptr;
	}
	if (player_collider != nullptr)player_collider->to_delete = true;

	return true;
}

void j1Player::CheckKeyboardState()
{

}

void j1Player::Draw()
{
	
}

bool j1Player::Load(pugi::xml_node& nodePlayer)
{

	LOG("Loading Player Info from XML...");

	position.x = nodePlayer.child("position").attribute("x").as_float();		//Load X Position
	position.y = nodePlayer.child("position").attribute("y").as_float();		//Load Y Position

	velocity.x = nodePlayer.child("velocity").attribute("x").as_float();		//Load X Velocity
	velocity.y = nodePlayer.child("velocity").attribute("y").as_float();		//Load Y Velocity

	p2SString name_state = nodePlayer.child("state").attribute("value").as_string();

	if (name_state == "IDLE_STATE")state = IDLE_STATE;		//In next lines, we load the state of the character
															//when we last saved or when loading the map to start
	if (name_state == "RUN_STATE")state = RUN_STATE;

	if (name_state == "JUMP_STATE")state = JUMP_STATE;

	if (name_state == "AIR_STATE")state = AIR_STATE;

	if (name_state == "DEATH_STATE")state = DEATH_STATE;

	if (name_state == "GOD_STATE")state = GOD_STATE;

	return true;
}

bool j1Player::Save(pugi::xml_node& nodePlayer) const
{
	pugi::xml_node nodeState = nodePlayer.append_child("state");
	pugi::xml_node positionPlayer = nodePlayer.append_child("position");
	pugi::xml_node velocityPlayer = nodePlayer.append_child("velocity");

	positionPlayer.append_attribute("x") = position.x;		//Save Position X
	positionPlayer.append_attribute("y") = position.y;		//Save Position Y

	velocityPlayer.append_attribute("x") = velocity.x;		//Save Velocity X
	velocityPlayer.append_attribute("y") = velocity.y;		//Save Velocity Y

	p2SString name_state;

	if (state == 0) name_state = "IDLE_STATE";				//In next lines, we save the state of the character

	if (state == 1) name_state = "RUN_STATE";

	if (state == 2) name_state = "JUMP_STATE";

	if (state == 3) name_state = "AIR_STATE";

	if (state == 4) name_state = "DEATH_STATE";

	if (state == 5) name_state = "GOD_STATE";

	nodeState.append_attribute("value") = name_state.GetString();
}