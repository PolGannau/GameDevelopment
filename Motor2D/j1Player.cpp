#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1CollisionManager.h"
#include "j1Map.h"
#include "j1SceneManager.h"
#include "j1Scene.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1EntityManager.h"

j1Player::j1Player(Entity_TYPE type, InfoPlayer PInfo) : j1Entity(type)
{
	LOG("Loading Player Characer...");

	offsetX = PInfo.offsetX;
	offsetY = PInfo.offsetY;

	speed = PInfo.speed;
	max_speed = PInfo.max_speed;
	max_jump_speed = PInfo.max_jump_speed;
	acceleration = PInfo.acceleration;

	jumpforce = PInfo.jumpforce;

	threshold = PInfo.threshold;

	idle = PInfo.idle;
	run = PInfo.run;
	jump = PInfo.jump;

	idle.speed = run.speed = jump.speed = 0.2F;
	idle.loop = run.loop = jump.loop = true;

	jump_sfx = App->audio->LoadFx("audio/fx/Jump.wav");
	death_sfx = App->audio->LoadFx("audio/fx/Hit_Hurt.wav");

	currentAnimation = &idle;
}

j1Player::~j1Player(){}

fPoint j1Player::GetPosition()
{
	return fPoint();
}

bool j1Player::Awake(pugi::xml_node& player)
{
	return true;
}

bool j1Player::Start()
{
	texture = App->tex->Load("textures/Character_Spritesheet.png");
	collider = App->collision->AddCollider({ 25,32,32,64 }, COLLIDER_PLAYER, this);
	return true;
}

bool j1Player::PreUpdate()
{

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)//if players presses A we go lef
	{
		current_movement = LEFT;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)//if key A releases we stop
	{
		current_movement = IDLE;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)//right
	{
		current_movement = RIGHT;

		if (last_movement == RIGHT) sprite_flipX = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		current_movement = IDLE;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)current_movement = IDLE;//if player hits both x axes movement keys we stop

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)//when space is pressed we check if the character state is on floor and also if he hasn't  so we can jump
	{
		if (!god_mode) //we only jump if the player isn't in godmode
		{
			if (current_state == ONFLOOR) {
				current_movement = UP;
				hasJumped = true;
				App->audio->PlayFx(jump_sfx);
			}
		}
		else if (god_mode && !god_mode_flying) // the godmode makes the player float, either up or down
		{
			god_mode_flying = true;
			speed.y = -1.F;
		}
		else
		{
			speed.y = 1.F;  
			god_mode_flying = false;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && App->collision->GetCloserColliderDownType() == COLLIDER_PLATFORM) {
		current_movement = DOWN;
		down_counter = 5;
	}
	//we save last movement to know when to flip the graphic
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)last_movement = LEFT;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)last_movement = RIGHT;
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) // A simple binary state changer
	{
		if (!god_mode)
		{
			god_mode = true;
		}
		else god_mode = false;
	}


	return true;
}

bool j1Player::Update(float dt)
{
	//we first check what will be the player horizontal movement so it has preference when for example you collide with ground it both axis at the same time
	CheckMHorizontalMovement(dt);
	position.x += speed.x;
	collider->rect.x = (position.x + offsetX);

	CheckVerticalMovement(dt);
	CheckState(dt);
	
	position.y += speed.y;
	collider->rect.y = (position.y + offsetY);

	App->render->Blit(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, sprite_flipX, sprite_flipY);


	return true;
}

bool j1Player::PostUpdate()
{
	return true;
}

bool j1Player::CleanUp()
{
	App->tex->UnLoad(texture); // the only thing the player loads is a texture, so we unload it
	texture = nullptr;

	return true;
}

void j1Player::CheckMHorizontalMovement(float dt)
{
	if (current_movement == IDLE)
	{
		speed.x = 0;
	}

	if (current_movement == LEFT)
	{
		sprite_flipX = false;
		speed.x = -max_speed.x*dt;
		if (!god_mode) // we won't check for collisions while we're in godmode
		{
			float distance = App->collision->CollisionCorrectionLeft();
			if (-distance > speed.x)
			{
				speed.x = -distance;
			}
		}
	}

	if (current_movement == RIGHT)
	{
		sprite_flipX = true;
		speed.x = max_speed.x*dt;
		if (!god_mode)// we won't check for collisions while we're in godmode
		{
			float distance = App->collision->CollisionCorrectionRight();
			if (distance < speed.x)
			{
				speed.x = distance;
			}
		}

	}
}

void j1Player::CheckVerticalMovement(float dt)
{
	if (current_movement == UP)
	{
		if (hasJumped)
		{
				current_state = AIR;
			speed.y = (jumpforce * -max_jump_speed + (1 - jumpforce) * speed.y);
			hasJumped = false;

		}
	}

	if (current_movement == DOWN)
	{
		if (down_counter > 0)
		{
			speed.y = (acceleration.y * max_speed.y + (1 - acceleration.y) * speed.y);
			if (fabs(speed.y) < threshold) speed.y = 0;
			down_counter--;
		}
		else current_movement = IDLE;
		
	}
}

void j1Player::CheckState(float dt)
{
	if (!god_mode) // Setting the state to air will make us load the jump animation
		// and we don't want that in godmode
	{
		current_state = AIR;
	}
	// we won't check for collisions while we're in godmode
	if (current_movement != DOWN && !god_mode)
	{
		speed.y = acceleration.y * max_speed.y + (1 - acceleration.y) * speed.y;
		if (fabs(speed.y) < threshold) speed.y = 0;
		if (speed.y > 0)
		{

			float distance = App->collision->CollisionCorrectionDown();
			if (distance < speed.y)
			{
				speed.y = distance;
				if (distance == 0)current_state = ONFLOOR;
			}
		}
		else if (speed.y < 0)
		{
			float distance = App->collision->CollisionCorrectionUp();
			if (-distance > speed.y)
			{
				speed.y = -distance;
			}
		}
	}
	currentAnimation = &jump;
	
	if (current_state == ONFLOOR)
	{
		if (speed.x == 0)currentAnimation = &idle;
		else currentAnimation = &run;
	}
}

// Load player State
bool j1Player::Load(pugi::xml_node& data)
{
	position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int() -1;

	saved_position.x = data.child("player").attribute("x").as_int();
	saved_position.y = data.child("player").attribute("y").as_int() -1;

	return true;
}

// Save player State
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("player");

	cam.append_attribute("x") = position.x;
	cam.append_attribute("y") = position.y;

	return true;
}


void j1Player::SetPosition(const float & x, const float & y)
{
	position.x = x;
	position.y = y;
}

//On collision triggers when a collider overlaps with player collider
void j1Player::OnCollision( Collider * collider1,  Collider * collider2)
{
	switch (collider2->type)
	{
	case COLLIDER_LIMIT: Die();//whenever the player collides with map limit he dies.
		break;
	default:
		break;
	}
	
}

void j1Player::Die()
{
	if (!god_mode) // in godmode we don't want the player to die
	{
		if (!death)
		{
			App->audio->PlayFx(death_sfx);
			death = true;
			current_movement = IDLE;
		}
		App->scenemanager->ReloadScene();
	}
}

