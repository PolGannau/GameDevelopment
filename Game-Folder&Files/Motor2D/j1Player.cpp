#include "j1Player.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1Scene2.h"
#include "j1FadeToBlack.h"
#include "j1Collision.h"

#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"


j1Player::j1Player() :j1Module() { name.create("player"); }

j1Player::~j1Player() {}

bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	LOG("Awakening Player Module...");

	state = IDLE_STATE;

	path.create(config.child("path").child_value());

	return ret;
}

bool j1Player::Start()
{
	bool ret = true;

	LOG("Starting Module Player...");

	pugi::xml_parse_result			result = playerFile.load_file(path.GetString());
	pugi::xml_node					player_node = playerFile.child("player");

	if (result == NULL)
	{
		LOG("Error loading player -> Error: %s", result.description());
		ret = false;
	}

	else
	{
		playerSprites = App->tex->Load(player_node.child("image").attribute("source").value());

		if (playerSprites == nullptr)
		{
			LOG("Error loading loadin textures");
			ret = false;
		}

		else
		{
			idle_animation = LoadAnimation("idle");
			jump_animation = LoadAnimation("jump");
			run_animation = LoadAnimation("run");
			dead_animation = LoadAnimation("death");
			afterjump_animation = LoadAnimation("afterjump");

			player_collider = App->collision->AddCollider({ coll_rect.x, coll_rect.y, coll_rect.w, coll_rect.h }, coll_type, App->player);
		}
	}
	return ret;
}

bool j1Player::PreUpdate()
{
	DebugInputs();
	
	CheckKeyboardState();

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
	bool right_KEYDOWN = App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT;
	bool left_KEYDOWN = App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT;
	bool right_KEYUP = App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP;
	bool left_KEYUP = App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP;
	bool space_KEYDOWN = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;

	switch (state)
	{
	case IDLE_STATE:
		if (right_KEYDOWN || left_KEYDOWN)state = RUN_STATE;
		if (space_KEYDOWN)state = JUMP_STATE;
		break;
	case RUN_STATE:
		if (right_KEYUP || left_KEYUP)state = IDLE_STATE;
		if (space_KEYDOWN)state = JUMP_STATE;
		break;
	case JUMP_STATE:
		afterjump_animation.Reset();
		if (jump_animation.Finished())state = AFTERJUMP_STATE;
	case AFTERJUMP_STATE:
		jump_animation.Reset();
		break;
	case DEAD_STATE:
		velocity.x = velocity.y = 0;
		break;
	case GOD_STATE:
		if (!App->collision->GetGodModeState())state = AFTERJUMP_STATE;
		break;
	default:
		break;
	}
}

void j1Player::PlayerActions()
{
	switch (state)
	{
	case IDLE_STATE:
		velocity.x = (1 - acceleration.x) * velocity.x;
		velocity.y = (1 - acceleration.y) * velocity.y;
		current_animation = &idle_animation;
		App->collision->SetColliderSize(player_collider, 45, 49);
			break;

	case RUN_STATE:
		velocity.y = (1 - acceleration.y)*velocity.y;
		current_animation = &run_animation;
		break;

	case JUMP_STATE:
		velocity.y = jumpAcceleration * jumpMaxVelocity + (1 - acceleration.y)*velocity.y;
		current_animation = &jump_animation;
		break;

	case AFTERJUMP_STATE:
		velocity.y = acceleration.y*-maximVelocity.y + (1 - acceleration.y)*velocity.y;
		current_animation = &afterjump_animation;
		break;

	case DEAD_STATE:
		current_animation = &dead_animation;
		if (App->scene->IsEnabled())
		{
			App->scene->ReloadScene();
		}
		else if (App->scene2->IsEnabled())
		{
			App->scene2->ReloadScene();
		}
		break;

	case GOD_STATE:
		current_animation = &idle_animation;
		velocity.y = acceleration.y*-maximVelocity.y + (1 - acceleration.y)*velocity.y;
		break;

	default:

		break;
	}
}

void j1Player::OnCollision(Collider * c1, Collider * c2)
{
	uint		toLeftDirection = (position.x < c2->rect.x);
	uint		toRightDirection = (c2->rect.x + c2->rect.w < position.x);
	uint		toUpDirection = (position.y < c2->rect.y + c2->rect.h);
	uint		toDownDirection = (c2->rect.y + 5 < position.y);

	if (state != DEAD_STATE)
	{
		switch (c2->type)
		{
		case COLLIDER_PLATFORM:

			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			{
				state = AFTERJUMP_STATE;
				c2->type == COLLIDER_NONE;
			}

			else
			{
				if (velocity.y > 0)
				{
					if (position.y < c2->rect.y)
					{
						jump_animation.Reset();
						state = IDLE_STATE;
					}
				}
				else
				{
					if (toRightDirection &&toDownDirection)position.x += maximVelocity.x;

					else if (toLeftDirection&&toDownDirection)position.x -= maximVelocity.x;

					else
					{
						position.y = (float)c2->rect.y;
						jump_animation.Reset();

						if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE) && (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE) && (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) && (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE) || state == AFTERJUMP_STATE)state = IDLE_STATE;
						
					}
				}
			}
			break;

		case COLLIDER_GROUND:
			if (toRightDirection && toDownDirection) {
				position.x = (float)(c2->rect.x + c2->rect.w + c1->rect.w / 2);
			}
			//Check collision from left
			else if (toLeftDirection && toDownDirection)
			{
				position.x = (float)(c2->rect.x - c1->rect.w / 2);
			}
			//Check collision from up
			else if (toUpDirection)
			{
				position.y = (float)c2->rect.y;
				if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE) && (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE) && (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) && (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE) || state == AFTERJUMP_STATE)
				{
					state = IDLE_STATE;
				}
			}
			//Check collision from down
			else
			{
				velocity.y = 0;
				state = AFTERJUMP_STATE;
				position.y = (float)(c2->rect.y + c2->rect.h + c1->rect.h);
			}
			break;

		case COLLIDER_DEATH:
			state = DEAD_STATE;
			break;

		case COLLIDER_FINISH:
			if (App->scene->IsEnabled())App->fadeToBlack->FadeToBlack(App->scene, App->scene2, 0.5F);

			else if (App->scene2->IsEnabled())App->fadeToBlack->FadeToBlack(App->scene2, App->scene, 0.5F);
			break;

		default:

			break;
		}
	}
}

void j1Player::Draw()
{
	SDL_Rect rect = current_animation->GetCurrentFrame();

	App->render->Blit(playerSprites, position.x, position.y, &rect);
}

void j1Player::DebugInputs()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		if (App->scene->IsEnabled())App->scene->ReloadScene();
		else if (App->scene2->IsEnabled())App->fadeToBlack->FadeToBlack(App->scene2, App->scene, 0.0F);
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (App->scene->IsEnabled())App->scene->ReloadScene();
		else if (App->scene2->IsEnabled())App->scene2->ReloadScene();
	}

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		App->collision->GodMode(); 
		state = GOD_STATE;
	}
}

Animation j1Player::LoadAnimation(p2SString name) {
	SDL_Rect frames;
	Animation anim;
	for (pugi::xml_node frames_node = playerFile.child("player").child("animation").child(name.GetString()).child("frame"); frames_node; frames_node = frames_node.next_sibling("frame"))
	{
		LOG("Loading animation %i",frames_node);
		frames.x = frames_node.attribute("x").as_int();
		frames.y = frames_node.attribute("y").as_int();
		frames.h = frames_node.attribute("h").as_int();
		frames.w = frames_node.attribute("w").as_int();

		anim.PushBack({ frames.x, frames.y, frames.w, frames.h });
	}
	anim.speed= playerFile.child("player").child("animation").child(name.GetString()).attribute("speed").as_float();

	return anim;

}

bool j1Player::Load(pugi::xml_node& nodePlayer)
{

	LOG("Loading Player Info from XML...");

	position.x = nodePlayer.child("position").attribute("x").as_float();		//Load X Position
	position.y = nodePlayer.child("position").attribute("y").as_float();		//Load Y Position

	velocity.x = nodePlayer.child("velocity").attribute("x").as_float();		//Load X Velocity
	velocity.y = nodePlayer.child("velocity").attribute("y").as_float();		//Load Y Velocity

	coll_rect.x = nodePlayer.child("collider").attribute("x").as_int();			//Load Player Collider X
	coll_rect.y = nodePlayer.child("collider").attribute("y").as_int();			//Load Player Collider Y

	p2SString name_state = nodePlayer.child("state").attribute("value").as_string();

	if (name_state == "IDLE_STATE")state = IDLE_STATE;		//In next lines, we load the state of the character
															//when we last saved or when loading the map to start
	if (name_state == "RUN_STATE")state = RUN_STATE;

	if (name_state == "JUMP_STATE")state = JUMP_STATE;

	if (name_state == "AFTERJUMP_STATE")state = AFTERJUMP_STATE;

	if (name_state == "DEAD_STATE")state = DEAD_STATE;

	if (name_state == "GOD_STATE")state = GOD_STATE;

	return true;
}

bool j1Player::Save(pugi::xml_node& nodePlayer) const
{
	pugi::xml_node nodeState = nodePlayer.append_child("state");
	pugi::xml_node positionPlayer = nodePlayer.append_child("position");
	pugi::xml_node velocityPlayer = nodePlayer.append_child("velocity");
	pugi::xml_node coll = nodePlayer.append_child("collider");

	positionPlayer.append_attribute("x") = position.x;		//Save Position X
	positionPlayer.append_attribute("y") = position.y;		//Save Position Y

	velocityPlayer.append_attribute("x") = velocity.x;		//Save Velocity X
	velocityPlayer.append_attribute("y") = velocity.y;		//Save Velocity Y

	coll.append_attribute("x") = coll_rect.x;
	coll.append_attribute("y") = coll_rect.y;

	p2SString name_state;

	if (state == 0) name_state = "IDLE_STATE";				//In next lines, we save the state of the character

	if (state == 1) name_state = "RUN_STATE";

	if (state == 2) name_state = "JUMP_STATE";

	if (state == 3) name_state = "AFTERJUMP_STATE";

	if (state == 4) name_state = "DEAD_STATE";

	if (state == 5) name_state = "GOD_STATE";

	nodeState.append_attribute("value") = name_state.GetString();

	return true;
}