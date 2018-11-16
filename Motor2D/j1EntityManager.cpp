#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "p2Log.h"

j1EntityManager::j1EntityManager()
{
	name.create("manager");
}

bool j1EntityManager::Awake(pugi::xml_node& node)
{
	return true;
}

bool j1EntityManager::Update(float dt)
{

	return true;
}

bool j1EntityManager::CleanUp()
{
	return true;
}

j1Entity * j1EntityManager::CreateEntity(Entity_TYPE type, int x, int y)
{
	return nullptr;
}

void j1EntityManager::DestroyEntity(j1Entity* entity)
{

}