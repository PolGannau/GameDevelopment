#include "j1Enemy.h"
#include "j1EntityManager.h"
#include "j1App.h"

j1Enemy::j1Enemy(Entity_TYPE type) :j1Entity(type)
{
	this->type_entity = type;
}

j1Enemy::~j1Enemy()
{
}

bool j1Enemy::Start()
{
	return false;
}

bool j1Enemy::PreUpdate()
{
	return false;
}

bool j1Enemy::Update(float dt)
{
	return false;
}

bool j1Enemy::PostUpdate()
{
	return false;
}

bool j1Enemy::CleanUp()
{
	return false;
}

void j1Enemy::Path(const p2DynArray<iPoint>* path)
{
}

void j1Enemy::OnCollision(Collider * coll1, Collider * coll2)
{
}
