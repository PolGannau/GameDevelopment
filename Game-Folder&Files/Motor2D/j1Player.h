#ifndef __J1PLAYER_H__
#define __J1PLAYER_H__

#include "j1Module.h"
#include "p2List.h"
#include "Animation.h"
#include "j1Collision.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct SDL_Rect;

enum PlayerState
{
	NO_STATE = -1,
	IDLE_STATE,
	RUN_STATE,
	JUMP_STATE,
	AFTERJUMP_STATE,
	DEAD_STATE,
	GOD_STATE
};

class j1Player :public j1Module
{
public:
	j1Player();

	virtual ~j1Player();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	void CheckKeyboardState();

	void PlayerActions();

	void OnCollision(Collider* c1, Collider* c2);

private:

	void Draw();
	void DebugInputs();
	Animation LoadAnimation(p2SString name);

public:

	fPoint position;
	fPoint velocity;
	fPoint maximVelocity;
	fPoint acceleration;
	float jumpMaxVelocity;
	float jumpAcceleration;

	SDL_Rect		coll_rect;
	Collider*		player_collider = nullptr;
	COLLIDER_TYPE	coll_type = COLLIDER_NONE;
	PlayerState		state = NO_STATE;
	bool			godMode = false;

private:

	p2SString				path;
	Animation*				current_animation = nullptr;
	Animation				idle_animation;
	Animation				run_animation;
	Animation				jump_animation;
	Animation				afterjump_animation;
	Animation				dead_animation;

	pugi::xml_document		playerFile;

	SDL_Texture*			playerSprites = nullptr;
	SDL_RendererFlip		flipX = SDL_FLIP_NONE;
};
#endif // !__J1PLAYER_H__
