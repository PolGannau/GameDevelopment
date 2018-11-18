#ifndef _j1PLAYER_
#define _j1PLAYER_

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Entity.h"

struct InfoPlayer;

class j1Player : public j1Entity
{
private:

	j1Animation			idle;
	j1Animation			run;
	j1Animation			jump;

	bool				direction[5];

	bool				hasJumped = false;

	unsigned int		jump_sfx=NULL;
	unsigned int		death_sfx=NULL;

	void CheckMHorizontalMovement(float);
	void CheckVerticalMovement(float);
	void CheckState(float);

	bool				god_mode = false;
	bool				god_mode_flying = false;

public:
	fPoint				last_position, saved_position; 
	float				jumpforce, max_jump_speed, threshold;
	int					down_counter = 5;
	bool				death = false;
	fPoint				speed, max_speed, acceleration;
	uint				offsetX, offsetY;
	j1Player(Entity_TYPE, InfoPlayer);	
	~j1Player();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

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

