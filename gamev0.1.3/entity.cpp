#include <math.h>
#include <windows.h>
#include <gl/gl.h>
#include "entity.h"
#include "lcglmath.h"
#include "world.h"

using namespace lcgl;

Entity::Entity (Instance* modelInstance,
				World* world, 
				float newSize, 
				const worldCoord& newPos, 
				const Direction& newDirection, 
				float newSpd,
				float newAnimationSpeed) : 
		Object (world, newSize, newPos), 
		modelInstance_ (modelInstance),
		animSpeed (newAnimationSpeed), 
		RUNNING_SPEED (newSpd),
		myDirection_ (newDirection)
{
	entitySound = NULL;
}

Entity::~Entity()
{
	if (entitySound != NULL) {	delete entitySound;	entitySound = NULL;	}
	if (modelInstance_ != NULL) { delete modelInstance_; modelInstance_= NULL; }
}

void Entity::OnAnimate(float deltaTime)
{
	float cosYaw = (float)cos(Deg2Rad(yaw()));
	float sinYaw = (float)sin(Deg2Rad(yaw())); 

	float amountTraveled = speed() * deltaTime;

	position_.x() += cosYaw*amountTraveled;
	position_.z() += sinYaw*amountTraveled;

	deltaT_ = deltaTime;	// used for interpolation, in OnDraw
}

void Entity::OnCollision(Object *collisionObject) 
{
	if (typeid (*collisionObject) == typeid (Entity))
	{
			  //WSB:  I don't like that this makes reference to InstanceMD2
			  // -- inside entity.cpp, which may not use MD2's.
			  // Make this virtual, and put it into Ogro & Sod?
 		modelInstance_->setState (InstanceMD2::IDLE);		stop();
	}
	else if (typeid(*collisionObject) == typeid(Terrain))
		setElevation(((Terrain*)collisionObject)->GetHeight(position().x(), position().z()) + size());
}

