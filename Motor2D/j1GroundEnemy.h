#ifndef __j1GROUNDENEMY_H__
#define __j1GROUNDENEMY_H__

#include "j1EntityManager.h"
#include "j1Entity.h"

#include "j1Enemy.h"

struct InfoGroEnemy;

class j1GroundEnemy : j1Enemy
{
public:

	j1GroundEnemy(Entity_TYPE type, InfoFlyEnemy GroundInfo);
	~j1GroundEnemy();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* coll1, Collider* coll2);

	void Path(const p2DynArray<iPoint>* path);

private:

	j1Animation			idle;
	j1Animation			move;

	float				triggerRadius;
};

#endif // !__j1GROUNDENEMY_H__