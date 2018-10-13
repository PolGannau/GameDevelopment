#ifndef _j1COLLISION_H_
#define _j1COLLISION_H_

#define MAX_COLLIDERS 500

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_PLATFORM,			//Platforms
	COLLIDER_PLAYER,			//Player
	COLLIDER_DEATH,				//Instant Death Collider, as when play falls into void
	COLLIDER_GROUND,			//Ground
	COLLIDER_FINISH,			//End Level
	COLLIDER_MAX,
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

	ColliderDistance ColliderDistance(SDL_Rect& collider_rect, COLLIDER_TYPE& collider_type) const;
};

struct ColliderDistance							//Used to advance collisions
{
	COLLIDER_TYPE			type;

	int						squaredDistance;
	bool					posX = false;		//Positive X
	bool					negX = false;		//Negative X
	bool					posY = false;		//Positive Y
	bool					negY = false;		//Negative Y
};




class j1Collision : public j1Module
{

private:

	Collider*				colliders[MAX_COLLIDERS];
	bool					matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool					debug = false;

public:

	j1Collision();

	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	~j1Collision();

	Collider* AddCollider(SDL_Rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	p2List<Collider*> GetColliderByType(COLLIDER_TYPE type);
	void CollisionCorrection(Collider* const collider1, fPoint & speed, fPoint &position);
	void GodMode();
	void DebugDraw();
	bool god = false, god_used = false;

	ColliderDistance			distance;
	ColliderDistance			NegativeX_Distance;
	ColliderDistance			PositiveX_Distance;
	ColliderDistance			NegativeY_Distance;
	ColliderDistance			PositiveY_Distance;
	COLLIDER_TYPE				type;

};


#endif // !_j1COLLISION_H_
