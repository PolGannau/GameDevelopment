#ifndef __j1FLYINGENEMY_H__
#define __j1FLYINGENEMY_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Entity.h"

enum STATE { ONFLOOR, AIR, DEATH };

struct InfoEnemy;

class j1FlyingEnemy : j1Entity
{
private:
	j1Animation			idle;
	j1Animation			move;
};

#endif // !__j1FlyingEnemy_H__