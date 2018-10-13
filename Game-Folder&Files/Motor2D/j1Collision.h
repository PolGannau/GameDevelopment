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
	SDL_Rect			rect;
	bool				to_delete = false;
	COLLIDER_TYPE		type = COLLIDER_NONE;
	j1Module*			callback = nullptr;
	bool				enable = true;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	void SetShape(int w, int h)
	{
		rect.w = w;
		rect.h = h;
	}

	void SetType(COLLIDER_TYPE type)
	{
		this->type = type;
	}
	bool CheckCollision(const SDL_Rect& r) const;

	class j1Collision : public j1Module
	{
	private:
		Collider* colliders[MAX_COLLIDERS];
	};
};

#endif // !_j1COLLISION_H_
