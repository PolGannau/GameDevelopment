#include "j1Entity.h"

j1Entity::j1Entity(Entity_TYPE type):type_entity(type)
{
}

j1Entity::~j1Entity() {}

bool j1Entity::Awake()
{
	return false;
}

bool j1Entity::Start()
{
	return false;
}

bool j1Entity::PreUpdate()
{
	return false;
}

bool j1Entity::Update(float dt)
{
	return false;
}

bool j1Entity::PostUpdate()
{
	return false;
}

bool j1Entity::CleanUp()
{
	return false;
}

fPoint j1Entity::GetPosition()
{
	return fPoint();
}

void j1Entity::SetPosition(const float & x, const float & y)
{
}

bool j1Entity::Save(pugi::xml_node &)
{
	return false;
}

bool j1Entity::Load(pugi::xml_node &)
{
	return false;
}

void j1Entity::OnCollision(Collider *, Collider *)
{
}

