#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1CollisionManager.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Player.h"
#include "p2Log.h"


j1CollisionManager::j1CollisionManager()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_GROUND] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_LIMIT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
}

// Destructor
j1CollisionManager::~j1CollisionManager()
{}

bool j1CollisionManager::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	// Calculate collisions
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->enable && c2->enable) {
				if (c1->CheckCollision(c2->rect) == true)
				{
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (matrix[c2->type][c1->type] && c2->callback)
						c2->callback->OnCollision(c2, c1);
				}
			}
		}
	}

	return true;
}

// Called before render is available
bool j1CollisionManager::Update(float dt)
{
	return true;
}

bool j1CollisionManager::PostUpdate()
{
	DebugDraw();

	return true;
}

COLLIDER_TYPE j1CollisionManager::GetCloserColliderDownType()
{
	COLLIDER_TYPE type;
	float closer = 1000.F;
	SDL_Rect playerRect = App->entitymanager->player->collider->rect;

	for (int i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->type && (colliders[i]->type == COLLIDER_GROUND || colliders[i]->type == COLLIDER_PLATFORM))//We only want to collide this way with ground and platform colliders
		{
			if (playerRect.x < colliders[i]->rect.x + colliders[i]->rect.w  && playerRect.x + playerRect.w > colliders[i]->rect.x)//We need to filter other colliders that aren't in our x axis
			{
				float distance = fabs(colliders[i]->rect.y - (playerRect.y + playerRect.h));
				if (closer > distance)
				{
					closer = distance;
					type = colliders[i]->type;
				}
			}
		}
	}

	return type;
}

void j1CollisionManager::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: 
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_PLAYER: 
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha, true);
			break;
		case COLLIDER_GROUND: 
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_LIMIT:
			App->render->DrawQuad(colliders[i]->rect, 100, 200, 10, alpha);
			break;
		case COLLIDER_PLATFORM: 
			App->render->DrawQuad(colliders[i]->rect, 100, 200, 100, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1CollisionManager::CleanUp()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1CollisionManager::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

//We check for incoming collisions, looking for the closer collider and getting the distance between it and the player. We'll set the distance as
//the player speed if the distance is smaller so at one point distance will be 0 and the player never collides.

//DOWN
float j1CollisionManager::CollisionCorrectionDown()
{
	float closer = 1000.F;
	SDL_Rect playerRect = App->entitymanager->player->collider->rect;
	for (int i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->type && (colliders[i]->type == COLLIDER_GROUND || colliders[i]->type == COLLIDER_PLATFORM))//We only want to collide this way with ground and platform colliders
		{ 
			if (playerRect.x < colliders[i]->rect.x + colliders[i]->rect.w  && playerRect.x + playerRect.w > colliders[i]->rect.x)//We need to filter other colliders that aren't in our x axis
			{
				float distance = fabs(colliders[i]->rect.y - (playerRect.y + playerRect.h));
				if (closer > distance)
				{
					closer = distance;
				}
			}
		}

	}
	return closer;
}

//UP
float j1CollisionManager::CollisionCorrectionUp()
{
	float closer = 1000.F;
	SDL_Rect playerRect = App->entitymanager->player->collider->rect;
	for (int i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->type == COLLIDER_GROUND)
		{
			if ((playerRect.x < colliders[i]->rect.x + colliders[i]->rect.w  && playerRect.x + playerRect.w > colliders[i]->rect.x))
			{
				float distance = fabs((colliders[i]->rect.y+colliders[i]->rect.h)-playerRect.y);
				if (closer > distance)
				{
					closer = distance;
				}
			}
		}

	}
	return closer;
}

//LEFT
float j1CollisionManager::CollisionCorrectionLeft()
{
	float closer = 1000.F;
	SDL_Rect playerRect = App->entitymanager->player->collider->rect;

	for (int i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->type == COLLIDER_GROUND)
		{
			if (playerRect.y < colliders[i]->rect.y + colliders[i]->rect.h  && playerRect.y + playerRect.h > colliders[i]->rect.y)
			{
				float distance = fabs((colliders[i]->rect.x+colliders[i]->rect.w) - playerRect.x);
				if (closer > distance)
				{
					closer = distance;
				}
			}
		}

	}
	return closer;
}

//RIGHT
float j1CollisionManager::CollisionCorrectionRight()
{
	float closer = 1000.F;
	SDL_Rect playerRect = App->entitymanager->player->collider->rect;

	for (int i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->type == COLLIDER_GROUND)
		{
			if (playerRect.y < colliders[i]->rect.y + colliders[i]->rect.h  && playerRect.y + playerRect.h > colliders[i]->rect.y)
			{
				float distance = fabs(colliders[i]->rect.x - (playerRect.x + playerRect.w));
				if (closer > distance)
				{
					closer = distance;
				}
			}
		}

	}
	return closer;
}


// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	if (enable) {
		if ((r.x < rect.x + rect.w) && (rect.x < r.x + r.w)
			&& (r.y < rect.y + rect.h) && (rect.y < r.y + r.h)) {
			return true;
		}
		return false;
	}
	else return false;
}