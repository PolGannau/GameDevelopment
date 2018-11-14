
#ifndef __j1SCENEMANAGER_H__
#define __j1SCENEMANAGER_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1SceneManager : public j1Module
{
public:
	j1SceneManager();
	~j1SceneManager();

	bool Start();
	bool Update(float dt);
	bool LoadScene(j1Module* module_off, j1Module* module_on, float time = 2.0f);
	bool LoadSceneWithoutFade(j1Module* module_off, j1Module* module_on);
	void ReloadScene(j1Module* module);
	void LoadFirstScene(j1Module* module_off);

	//current active scene
	j1Module* current_scene = nullptr;

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
	j1Module* module_enable = nullptr;
	j1Module* module_disable = nullptr;

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
};

#endif //__j1SCENEMANAGER_H__