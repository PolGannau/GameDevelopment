#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1Module.h"

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

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Cleanup()
	{
		return true;
	}

	virtual void Draw() {};

private:


};

#endif // !__j1ENTITY_H__
