#ifndef __SOD_H
#define __SOD_H

/********************************************************
	SOD.H

	The SodEnemy class
	Description: The SodEnemy represents the Sod model
			   enemy. This enemy is considered smarter,
			   but has weaker armor than the Ogre.
			   Adapted By DOTL Dev Team

*********************************************************/

#include "enemy.h"

using namespace lcgl;

#define SOD_RADIUS_OF_AWARENESS 125.0f  //How close you can get to a Sod before he notices you
#define SOD_ZIGZAG_ANGLE 30 //how much the Sod will veer left or right when running

class SodEnemy : public Enemy
{
protected:
	void OnCollision(Object *collisionObject);
	int stateStart_, stateEnd_;	// state keyframe start/end
	int stateStart () const { return stateStart_; } //used by Ogro and Sod
	lcgl::Direction _direction;//private direction mac 4/3/14
	lcgl::Sound _death; //noise the model makes when it dies mac 4/3/14

public:
	SodEnemy (const ModelMD2& model,
				World* world, Player* p, float x=0.0f, float z=0.0f) 
		: Enemy (new InstanceMD2 (model, DEFAULT_ENTITY_INTERPOLATION), 
		world, p, SOD_RADIUS_OF_AWARENESS, 
		SOD_ZIGZAG_ANGLE,
		DEFAULT_ENTITY_SIZE,
		worldCoord (x, 0, z)),_death("sounds//dyinggroan.wav")
	{ 
		stateStart_ = 0;
		stateEnd_ = 39;
	}
	~SodEnemy() {}

	SodEnemy (const SodEnemy&);			//intentionally not written
	void operator= (const SodEnemy&);	//intentionally not written

	void OnPrepare();
	void OnDraw(const Player&);
};


#endif