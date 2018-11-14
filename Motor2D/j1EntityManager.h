#ifndef __j1ENTITYMANAGER_H__
#define __j1ENTITYMANAGER_H__

#include "j1Module.h"
#include "p2List.h"
#include "j1Entity.h"

class j1EntityManager : public j1Module
{
public:

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	j1Entity* CreateEntity(Entity_TYPE type, int x, int y);

	void DestroyEntity(j1Entity entity);

private:

	p2List<j1Entity*> entities;
};

#endif // !__J1ENTITYMANAGER_H__
