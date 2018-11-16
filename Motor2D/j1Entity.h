#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Textures.h"
#include "j1CollisionManager.h"

enum class Entity_TYPE 
{
	PLAYER,
	FLYING_ENEMY,
	GROUND_ENEMY,
	unknown
};

class j1Entity : public j1Module
{
public:

	j1Entity(Entity_TYPE type);
	~j1Entity() {};

	virtual bool Awake();
	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	bool CleanUp();

	virtual void OnCollision(Collider*, Collider*) {}

public:

	fPoint			position_entity;
	fPoint			speed_entity;
	bool			sprite_flipY = false;
	bool			sprite_flipX = false;
	Entity_TYPE		type_entity;
	SDL_Texture*	texture_entity = nullptr;
	j1Animation*	currentAnimation_entity = nullptr;

};

#endif // !__j1ENTITY_H__
