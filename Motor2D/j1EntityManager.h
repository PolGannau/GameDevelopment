#ifndef __j1ENTITYMANAGER_H__
#define __j1ENTITYMANAGER_H__

#include "j1Module.h"
#include "j1Animation.h"
#include "j1Entity.h"

class j1Player;

struct InfoPlayer
{
	j1Animation		idle, jump, run;
	int				offsetX, offsetY;
	fPoint			speed, max_speed, acceleration;
	float			max_jump_speed, jumpforce;
	float			threshold;
};

struct InfoFlyEnemy
{
	j1Animation		idle, move;
	fPoint			speed, max_speed, acceleration;
};

struct InfoGroEnemy
{
	j1Animation		idle, move;
	fPoint			speed, max_speed, acceleration;
};

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	bool CreateEntity(Entity_TYPE type);

	void DestroyEntity(j1Entity* entity);

	void OnCollision(Collider* coll1, Collider* coll2);

	p2List<j1Entity*>			entities;
	pugi::xml_node				pathNode;
	j1Player*					player;

private:

	InfoPlayer					PInfo;

	InfoGroEnemy					GroundEnemy;
	InfoFlyEnemy					FlyEnemy
};

#endif // !__J1ENTITYMANAGER_H__
