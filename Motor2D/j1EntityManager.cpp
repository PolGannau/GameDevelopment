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
	pugi::xml_node player = node.child("player");
	PInfo.speed.x = player.child("speed").attribute("x").as_float();
	PInfo.speed.y = player.child("speed").attribute("y").as_float();
	PInfo.acceleration.x = player.child("acceleration").attribute("x").as_float();
	PInfo.acceleration.y = player.child("acceleration").attribute("y").as_float();
	PInfo.max_speed.x = player.child("maxSpeed").attribute("x").as_float();
	PInfo.max_speed.y = player.child("maxSpeed").attribute("y").as_float();
	PInfo.max_jump_speed = player.child("jump").attribute("maxJumpSpeed").as_float();
	PInfo.jumpforce = player.child("jump").attribute("jumpforce").as_float();
	PInfo.threshold = player.child("threshold").attribute("value").as_float();
	PInfo.offsetX = player.child("offset").attribute("x").as_int();
	PInfo.offsetY = player.child("offset").attribute("y").as_int();

	for (pugi::xml_node rect = player.child("animation").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		if (rect.attribute("id").as_int() == 1)
		{
			PInfo.idle.PushBack({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), rect.attribute("w").as_int(), rect.attribute("h").as_int() });
		}
		else if (rect.attribute("id").as_int() == 2)
		{
			PInfo.jump.PushBack({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), rect.attribute("w").as_int(), rect.attribute("h").as_int() });
		}
		else if (rect.attribute("id").as_int() == 3)
		{
			PInfo.run.PushBack({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), rect.attribute("w").as_int(), rect.attribute("h").as_int() });
		}
	}

	CreateEntity(Entity_TYPE::PLAYER);


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
			entities[i]->CleanUp();
	}

	return true;
}

bool j1EntityManager::CreateEntity(Entity_TYPE type)
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