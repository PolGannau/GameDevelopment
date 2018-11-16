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

bool j1EntityManager::Start()
{
	for (int i = 0; i < entities.count(); i++)
	{
		if (entities[i] != nullptr)
			entities[i]->Start();
	}

	return true;
}

bool j1EntityManager::PreUpdate()
{
	for (int i = 0; i < entities.count(); i++)
	{
		if (entities[i] != nullptr)
			entities[i]->PreUpdate();
	}

	return true;
}

bool j1EntityManager::Update(float dt)
{
	for (int i = 0; i < entities.count(); i++)
	{
		if (entities[i] != nullptr)
			entities[i]->Update(dt);
	}

	return true;
}

bool j1EntityManager::PostUpdate()
{
	for (int i = 0; i < entities.count(); i++)
	{
		if (entities[i] != nullptr)
			entities[i]->PostUpdate();
	}

	return true;
}

bool j1EntityManager::CleanUp()
{
	for (int i = 0; i < entities.count(); i++)
	{
		if (entities[i] != nullptr)
			entities[i]->Cleanup();
	}

	return true;
}

bool j1EntityManager::CreateEntity(Entity_TYPE type, int x, int y)
{
	switch (type)
	{
	case Entity_TYPE::PLAYER:
		player = new j1Player(type, PInfo);
		if (player != nullptr)
			entities.add(player);
		break;

	default:
		break;
	}
	return true;
}

void j1EntityManager::DestroyEntity(j1Entity* entity)
{
	for (int i = 0; i < entities.count(); i++)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}
}

void j1EntityManager::OnCollision(Collider* coll1, Collider* coll2)
{
	
}