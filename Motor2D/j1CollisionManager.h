#ifndef _j1COLLISIONMANAGER_
#define _j1COLLISIONMANAGER_

#define MAX_COLLIDERS 1000

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"

enum COLLIDER_TYPE {
	COLLIDER_NONE = -1,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_GROUND,
	COLLIDER_LIMIT,
	COLLIDER_PLATFORM,
	COLLIDER_MAX,
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;
	bool enable = true;

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

	
};

class j1CollisionManager :
	public j1Module
{
public:
	j1CollisionManager();
	~j1CollisionManager();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	float CollisionCorrectionDown(SDL_Rect);
	float CollisionCorrectionRight(SDL_Rect);
	float CollisionCorrectionLeft(SDL_Rect);
	float CollisionCorrectionUp(SDL_Rect);

	//returns the type of the collider u are on platform or ground
	COLLIDER_TYPE GetCloserColliderDownType();
	void DebugDraw();

private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};


#endif // !_j1COLLISIONMANAGER_