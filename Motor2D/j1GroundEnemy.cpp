#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Enemy.h"
#include "j1GroundEnemy.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1PathFinding.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Brofiler/Brofiler.h"

j1GroundEnemy::j1GroundEnemy(Entity_TYPE type, InfoGroEnemy GroundEInfo) :j1Enemy(type)
{
	LOG("Loading Ground Enemies...");
	speed_entity = GroundEInfo.speed;
	acceleration_entity = GroundEInfo.acceleration;
	idle = GroundEInfo.idle;
	move = GroundEInfo.idle;
	triggerRadius = GroundEInfo.trigger;

	idle.speed = move.speed = 0.4F;
	idle.loop = move.loop = true;

	currentAnimation = &idle;
	texture = App->tex->Load("textures/rat.png");

	collider = App->collision->AddCollider({ (int)position.x,(int)position.y,32,32 }, COLLIDER_ENEMY, this);
}

j1GroundEnemy::~j1GroundEnemy()
{
}

bool j1GroundEnemy::Start()
{
	return true;
}

bool j1GroundEnemy::PreUpdate() //Calling to pathfinding function
{
	iPoint iPos = { (int)position.x,(int)position.y };

	float dis = position.DistanceManhattan(App->entitymanager->player->position);

	if (dis < triggerRadius)
	{
		iPoint iPlayer = { (int)App->entitymanager->player->position.x,(int)App->entitymanager->player->position.y };
		App->pathfinding->CreatePath(iPos, iPlayer);
		Path(App->pathfinding->GetLastPath());
	}

	else currMove = IDLE;
	currMove = LEFT;
	currentAnimation = &move;

	return true;
}

bool j1GroundEnemy::Update(float dt)
{
	if (currMove == IDLE) speed_entity.x = 0;

	if (currMove == LEFT)
	{
		speed_entity.x = -4;
		speed_entity.x = floor(speed_entity.x*dt);
		sprite_flipX = false;

		float dist = App->collision->CollisionCorrectionLeft(collider->rect);

		if (-dist > speed_entity.x) speed_entity.x = -dist;
	}

	if (currMove == RIGHT)
	{
		speed_entity.x = 4;
		speed_entity.x = floor(speed_entity.x*dt);
		sprite_flipX = true;

		float dist = App->collision->CollisionCorrectionRight(collider->rect);

		if (dist < speed_entity.x) speed_entity.x = dist;
	}

	position.x += speed_entity.x;
	collider->rect.x = position.x;

	speed_entity.y += floor(acceleration_entity.y*dt);

	if (speed_entity.y > 0)
	{
		float dist = App->collision->CollisionCorrectionDown(collider->rect);

		if (dist <= 1) currState = ONFLOOR;
	}

	App->render->Blit(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, sprite_flipX, sprite_flipY);

	position.y += speed_entity.y;
	collider->rect.y = position.y;

	return true;
}

bool j1GroundEnemy::PostUpdate()
{
	return true;
}

bool j1GroundEnemy::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}

void j1GroundEnemy::OnCollision(Collider * coll1, Collider * coll2)
{
	switch (coll1->type)
	{
	case COLLIDER_PLAYER:
		break;
	default:
		break;
	}
}

void j1GroundEnemy::Path(const p2DynArray<iPoint>* path)
{
	if (path->Count() > 0)
	{
		for (int i = 0; i < path->Count(); ++i)
		{
			iPoint Pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			App->render->Blit(App->scene->PathTexture, Pos.x, Pos.y);
		}
	}
}
