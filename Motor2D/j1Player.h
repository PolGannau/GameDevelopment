#ifndef _j1PLAYER_
#define _j1PLAYER_

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Textures.h"
#include "j1CollisionManager.h"

enum MOVEMENT { IDLE, RIGHT, LEFT, UP, DOWN };
enum STATE {ONFLOOR, AIR, DEATH};

class j1Player :
	public j1Module
{
private:

	j1Animation* current_animation = nullptr;
	j1Animation idle;
	j1Animation run;
	j1Animation jump;

	bool direction[5];

	bool hasJumped = false;
	bool flip_x, flip_y = false;
	SDL_Texture* player_texture=nullptr;

	unsigned int jump_sfx=NULL;
	unsigned int death_sfx=NULL;

	void CheckMHorizontalMovement();
	void CheckVerticalMovement();
	void CheckState();

	bool god_mode = false;
	bool god_mode_flying = false;



public:
	Collider* collider = nullptr;
	fPoint position, last_position, saved_position; 
	float jumpforce, max_jump_speed, threshold;
	int down_counter = 5;
	bool death = false;
	fPoint speed, max_speed, acceleration;
	uint offsetX, offsetY;
	j1Player();	
	j1Player(const float &x, const float &y);
	~j1Player();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	STATE current_state;
	MOVEMENT last_movement;
	MOVEMENT current_movement = IDLE;

	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
	fPoint GetPosition();
	void SetPosition(const float &x, const float &y); 
	void OnCollision( Collider* collider1,  Collider* collider2);
	void Die();

};

#endif // !_j1Player_
