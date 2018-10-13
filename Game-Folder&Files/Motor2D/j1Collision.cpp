#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "p2Log.h"

j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)colliders[i] = nullptr;

	matrix[COLLIDER_PLAYER][COLLIDER_PLATFORM] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_FINISH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;

	matrix[COLLIDER_GROUND][COLLIDER_GROUND] = false;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_GROUND][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_GROUND][COLLIDER_DEATH] = false;
	matrix[COLLIDER_GROUND][COLLIDER_FINISH] = false;

	matrix[COLLIDER_PLATFORM][COLLIDER_GROUND] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PLATFORM][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_DEATH] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_FINISH] = false;

	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEATH][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_DEATH][COLLIDER_GROUND] = false;
	matrix[COLLIDER_DEATH][COLLIDER_FINISH] = false;

	matrix[COLLIDER_FINISH][COLLIDER_FINISH] = false;
	matrix[COLLIDER_FINISH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_FINISH][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_FINISH][COLLIDER_GROUND] = false;
	matrix[COLLIDER_FINISH][COLLIDER_DEATH] = false;
}

j1Collision :: ~j1Collision() {}


bool j1Collision::PreUpdate()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider*		coll1;
	Collider*		coll2;

	NegativeX_Distance.squaredDistance = 9999;
	NegativeY_Distance.squaredDistance = 9999;
	PositiveX_Distance.squaredDistance = 9999;
	PositiveY_Distance.squaredDistance = 9999;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)continue;

		coll1 = colliders[i];

		for (uint j = i + 1; j < MAX_COLLIDERS; ++j)
		{
			if (colliders[j] == nullptr)continue;

			coll2 = colliders[j];

			if (coll1->enable && coll2->enable)
			{
				if (coll1->CheckCollision(coll2->rect) == true)
				{
					if (matrix[coll1->type][coll2->type] && coll1->callback)
						coll1->callback->OnCollision(coll1, coll2);

					if (matrix[coll2->type][coll1->type] && coll2->callback)
						coll2->callback->OnCollision(coll2, coll1);
				}

				if (coll2->type == COLLIDER_PLAYER)
				{
					distance = coll2->ColliderDistance(coll1->rect, coll1->type);

					if (distance.posY && distance.squaredDistance < PositiveY_Distance.squaredDistance)
					{
						PositiveY_Distance.squaredDistance = distance.squaredDistance;
						PositiveY_Distance.typeNearColl = distance.typeNearColl;
					}

					else if (distance.negY && distance.squaredDistance < NegativeY_Distance.squaredDistance)
					{
						NegativeY_Distance.squaredDistance = distance.squaredDistance;
						NegativeY_Distance.typeNearColl = distance.typeNearColl;
					}

					else if (distance.posX && distance.squaredDistance < PositiveX_Distance.squaredDistance)
					{
						PositiveX_Distance.squaredDistance = distance.squaredDistance;
						PositiveX_Distance.typeNearColl = distance.typeNearColl;
					}

					else if (distance.negX && distance.squaredDistance < NegativeX_Distance.squaredDistance)
					{
						NegativeX_Distance.squaredDistance = distance.squaredDistance;
						NegativeX_Distance.typeNearColl = distance.typeNearColl;
					}
				}
			}
		}
	}
	return true;
}
