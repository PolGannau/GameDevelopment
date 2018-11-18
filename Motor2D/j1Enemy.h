#ifndef __j1ENEMY_H__
#define __j1ENEMY_H__

#include "j1Entity.h"
#include"p2Point.h"
#include "p2DynArray.h"
#include "j1Animation.h"

struct InfoFlyEnemy;

class j1Enemy : public j1Entity
{
public:

	j1Enemy(Entity_TYPE type);
	~j1Enemy();

	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();

	virtual void Path(const p2DynArray<iPoint>* path);

	virtual void OnCollision(Collider* coll1, Collider* coll2);
};

#endif // !__j1ENEMY_H__