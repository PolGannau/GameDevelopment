#ifndef _j1COLLISION_H_
#define _j1COLLISION_H_

#define MAX_COLLIDERS 500

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_PLATFORM,
	COLLIDER_PLAYER,
	COLLIDER_GROUND,
	COLLIDER_LIMIT,
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type = COLLIDER_NONE;
};

#endif // !_j1COLLISION_H_
