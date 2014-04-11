#ifndef __PLAYER_H
#define __PLAYER_H

/*******************************************************

	PLAYER.H

	The Player class

	Description:The game user or camera view mac 4/5 14
	Adapted By: DOTL Dev Team

********************************************************/

#include <typeinfo.h>

#include "camera.h"
#include "object.h"
#include "terrain.h"
#include "sound.h"
#include "direction.h"

using namespace lcgl;

class SodEnemy; class OgroEnemy; class Terrain;

#define PLAYER_SIZE 7.0f

class World;

class Player : public Object
{
private:
	Terrain* terrain;
	Sound*   rocketSound;
	int health; 
protected:
	void OnCollision (Object *collisionObject);

	void OnPrepare ()	{ ProcessCollisions(this);   } //Seems like we should!

	//Camera   myCamera;
	//Camera*  camera ()  { return &myCamera; }
	lcgl::Direction _direction;
	//lcgl::Sound _pain;,_pain("sounds//2scream.wav")

public:
	Player(World* world = NULL) : Object (world, PLAYER_SIZE) { terrain = NULL; rocketSound = NULL; health=100; }
	~Player() {}
	Player (const Player&);
	inline int getHealth(){return health;};
	void operator= (const Player&);

	//float		direction ()	{ return camera()->yaw();   }
	worldCoord& position()		 { return position_; }
	const 
	worldCoord& position() const { return position_; }

	worldCoord& velocity()		 { return velocity_; }
	const
	worldCoord& velocity() const { return velocity_; }

	float yaw ()				{ return _direction.yaw(); }
	float pitch()				{ return _direction.pitch(); }
	void  setPitch (float p)    { _direction.setPitch (p); }
	void  setYaw   (float y)    { _direction.setYaw   (y); }
	
	void SetTerrain	 (Terrain *t) { terrain = t; }

	void FireWeapon	 ();

	void Animate (float deltaTime);
	void speedUp (float howMuch)
	{
		velocity_ += worldCoord(_direction) * howMuch;
	}
	void speedRight (float howMuch)
	{
		velocity_ += crossProduct(worldCoord(_direction), Y_NORMAL) * howMuch;
	}

};

#endif