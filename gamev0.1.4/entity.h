#ifndef __ENTITY_H
#define __ENTITY_H

/******************************************************************
	ENTITY.H

	The Entity class

	Description: The Entity class is derived from CModelMD2 and
			   provides the functionality of an entity in the
			   game world. It holds the current MD2 animation
			   state, the entity angle heading, and the entity's
			   primary sound.

			   Adapted By DOTL Dev Team

*****************************************************************/

#include <typeinfo.h>
#include "modelmd2.h"
#include "object.h"
#include "sound3d.h"
#include "listener.h"
#include "terrain.h"
#include "direction.h"

using namespace lcgl;

#define DEFAULT_ENTITY_SIZE     6.25f
#define DEFAULT_RUNNING_SPEED  15.0f
#define DEFAULT_ENTITY_INTERPOLATION 3.0f
#define DEFAULT_ANIMATION_SPEED 7.0f

class Entity : public Object
{
protected:
	void OnAnimate	(float deltaTime);
	virtual void OnDraw		(const Player& player) = 0;
	void OnCollision(Object *collisionObject);

	//worldCoord forward_;
	Direction myDirection_;

	float speed_;

	//void verifyPolarAndCartesian () const;

protected:
	Instance* modelInstance_;

	virtual void OnPrepare	() {};
	void PlaySound	() { entitySound->play(); } 
	Sound* entitySound;	// the sound the entity makes
						// currently only supports one sound
						// per entity
						// Why a pointer?  Because it might ACTUALLY be
						// a Sound3D, which is a subclass of Sound

	float deltaT_;	//The last time OnAnimate was called ... how long had it
					//been since the previous time?
					//This is used later by OnDraw
public:
	const float animSpeed;
	const float RUNNING_SPEED; 

	Entity (Instance* ,
			World* world, float newSize = DEFAULT_ENTITY_SIZE, 
			const worldCoord& newPos = NULL_VECTOR, 
			const Direction& newDirection = Direction(0,0), 
			float newSpd = DEFAULT_RUNNING_SPEED,
			float newAnimationSpeed = DEFAULT_ANIMATION_SPEED);
	~Entity();

	Entity (const Entity& other);
	void operator= (const Entity& other);

	void stop () { speed_ = 0.0f; }
	void run  () { speed_ = RUNNING_SPEED; }
	
	void setDirection (const Direction& dir) { myDirection_ = dir; }
	void setYaw (float newDir) 	{ 		myDirection_.setYaw (newDir);	}

	float yaw   () const { return myDirection_.yaw(); }		// angle the entity is facing (in degrees)
	float pitch () const { return myDirection_.pitch(); }
	float speed () const { return speed_; }			//we are using direction, so we don't need
							//a velocity vector -- just a scalar

	void SetAudio (const Sound* foo){	entitySound = (Sound*) foo; }
};

#endif