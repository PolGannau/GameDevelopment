#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Textures.h"

enum class Entity_TYPE 
{
	PLAYER,
	FLYING_ENEMY,
	GROUND_ENEMY,
	unknown
};

enum class Entity_STATE
{
	IDLE,
	WALKING,
	CHASING,
	ATTACKING,
	unknown
};

class j1Entity : public j1Module
{
public:

	j1Entity(const char* name);

	~j1Entity() {};

	bool Start();

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Cleanup()
	{
		delete texture_entity;
		delete currentAnimation_entity;

		return true;
	}

	virtual void Draw() {};

public:

	fPoint			position_entity;
	Entity_TYPE		type_entity;
	SDL_Texture*	texture_entity;
	j1Animation*	currentAnimation_entity;

};

#endif // !__j1ENTITY_H__
