#ifndef __j1FLYINGENEMY_H__
#define __j1FLYINGENEMY_H__

#include "j1EntityManager.h"
#include "j1Entity.h"

#include "j1Enemy.h"

struct InfoFlyEnemy;

class j1FlyingEnemy : public j1Enemy
{
public:

	j1FlyingEnemy(Entity_TYPE type, InfoFlyEnemy FlyEnemy);
	~j1FlyingEnemy();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void Path(const p2DynArray<iPoint>* path);

	void OnCollision(Collider* coll1, Collider* coll2);

private:
	j1Animation			idle;
	j1Animation			move;

	float				triggerRadius;
};

#endif // !__j1FlyingEnemy_H__