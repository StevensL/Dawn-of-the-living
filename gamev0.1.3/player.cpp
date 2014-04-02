/*

	PLAYER.H

	The Player class

	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: 3/30/2001
	Description:

*/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <cmath>
#include "player.h"
#include "world.h"
#include "Acid.h"
#include "ogro.h"
#include "lcglmath.h"

using namespace lcgl;
using namespace std;

void Player::Animate(float deltaTime)
{
	static const float MAX_SPEED = 45.0f; //make a member of player? (edited by mac to make player move faster for testing default = 30)
	static const float FRICTION = 1.5f; //make a member of player?
	static const float MAX_PITCH = 60.0f; //make a member of player?

	 if (_direction.pitch() >  MAX_PITCH)	_direction.setPitch( MAX_PITCH);
	 if (_direction.pitch() < -MAX_PITCH)   _direction.setPitch(-MAX_PITCH);
     
	 if (velocity().Length() > 0.0)
		 acceleration_ = -velocity() * FRICTION;

	 //Limit velocity to a maximum of 15.0f in magnitude?
	 if (velocity().Length() > MAX_SPEED)
		 velocity_ = velocity().normalized()*MAX_SPEED;

	 velocity_ += acceleration()* deltaTime;
	 position_ += velocity()    * deltaTime;

	 Camera myCamera (position(), position_ + worldCoord (_direction), Y_NORMAL);
	 myCamera.aim ();
}

void Player::FireWeapon()
{
	// use camera->lookAt vector and multiply
	// this lookAt vector by the ammo's speed
	// to get the ammo velocity vector.
	// create the new ammo and attach it to
	// the world. fire the ammo

	Direction dir (yaw(), pitch ());
	Acid* newRocket = new Acid (world_->RocketModel, world_->_explosionTexture, world_, dir); 

	newRocket->position_  = position  ();
	newRocket->SetAudio(&(world_->explosionSound()));

	world_->push_back (newRocket);
}

void Player::OnCollision(Object *collisionObject)
{
	if (typeid(*collisionObject) == typeid(Terrain))
	{
		position_.y() 
			= ((Terrain*)collisionObject)->GetHeight  (position().x(), 
														position().z()) 
					+ size();
		setElevation(position().y()); //player has *2* positions...fix this!
	}
	else if (typeid(*collisionObject) == typeid(SodEnemy) || typeid(*collisionObject) == typeid(OgroEnemy))
	{
		if (velocity().z() > 0.0) velocity_ = NULL_VECTOR; //Why Z only?

		//maybe we should also make the Enemy not walk through us!  Either here or in Entity::OnCollision
		//Currently left as an exercise to the student.  You'd need to set the position of the monster,
		// or the player, so that they aren't intersecting.
	}

	//Dang!  What happened to the collision code I had here before?
/*	if (typeid(*collisionObject) == typeid(SodEnemy))
		MessageBox(NULL, "He got me", "I'm hit!", MB_OK|MB_DEFBUTTON1);
	if (typeid(*collisionObject) == typeid(OgroEnemy))
		MessageBox(NULL, "He got me", "I'm hit!", MB_OK|MB_DEFBUTTON1);*/

}
