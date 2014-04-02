/*
	MS3DMonster.CPP

	The MS3DMonster class implementation

	1-2010 by Will Briggs

	Much copied from
	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: 3/30/2001
	Description:

*/

#include <windows.h>
#include <gl/gl.h>
//#include <ostream> //for printing with log
#include "ms3dMonster.h"
#include "Acid.h"
#include "lcglmath.h"

using namespace lcgl;


void MS3DMonster::OnCollision(Object *collisionObject)
{
	// as long as the model is not dying, it can collide with other objects
	if (modelInstance_->state() != InstanceMD2::DIE)
	{
		// if this enemy collides with another enemy
		if ((typeid(*collisionObject) == typeid(SodEnemy)) ||
			(typeid(*collisionObject) == typeid(OgroEnemy)))
			setAIState (AI_UNCARING);

		// if this enemy collides with the terrain (always)
		else	if (typeid(*collisionObject) == typeid(Terrain))
		{
			setElevation(((Terrain*)collisionObject)->GetHeight(position().x(), position().z()) + size());
	
			if (position().x() <= ((Terrain*)collisionObject)->edgeWidth())
				position_.x() = ((Terrain*)collisionObject)->edgeWidth();
			if (position().x() >= ((Terrain*)collisionObject)->GetWidth()*((Terrain*)collisionObject)->GetMul() - ((Terrain*)collisionObject)->edgeWidth())
				position_.x() = ((Terrain*)collisionObject)->GetWidth()*((Terrain*)collisionObject)->GetMul() - ((Terrain*)collisionObject)->edgeWidth();
			if (position().z() <= ((Terrain*)collisionObject)->edgeWidth())
				position_.z() = ((Terrain*)collisionObject)->edgeWidth();
			if (position().z() >= ((Terrain*)collisionObject)->GetWidth()*((Terrain*)collisionObject)->GetMul() - ((Terrain*)collisionObject)->edgeWidth())
				position_.z() = ((Terrain*)collisionObject)->GetWidth()*((Terrain*)collisionObject)->GetMul() - ((Terrain*)collisionObject)->edgeWidth();
		}
		else if (typeid(*collisionObject) == typeid(Acid))
		{
			// kill the monster
			setAIState (AI_DEAD);	stop();
		}
	}
}

//MS3DMonster::OnPrepare and SodEnemy::OnPrepare are identical . . .
// . . .but probably should be made different, to make them distinct enemies.
void MS3DMonster::OnPrepare() 
{
		//Here we keep track of how long it was since the last time we randomly changed direction.
		// This is so we don't do it in each frame -- it looks like the monster is having a fit!  WSB

/*	static const float TIME_BETWEEN_DIRECTION_CHANGES = 1.5f; //1.5 seconds, and the monster shd change direction
	static float timeSinceLastDirectionChange = 0.0f;
	timeSinceLastDirectionChange += deltaT_;

	bool timeForAnotherDirectionChange = false;
	if (timeSinceLastDirectionChange > TIME_BETWEEN_DIRECTION_CHANGES)
	{
		timeForAnotherDirectionChange = true;
		timeSinceLastDirectionChange -= TIME_BETWEEN_DIRECTION_CHANGES;
			//maybe I should set it to 0.  But I'm going to try keeping time between direction changes pretty constant.
	}
	*/

		//Now:  what's the direction away from the player?  This is used for running.  Negate it
		// and the monster runs TOWARD the player.
	worldCoord whereToGo = position() - player->position();
	whereToGo.y() = 0.0f;
	Direction myNewDir (whereToGo);

	ProcessAI();
	// now do Ogro prep
	// set _state based on AIstate
	// if AI_SCARED, then RUN
	switch (aiState)
	{
	case AI_SCARED:
		{
			static float randomAngle = 0.0f;
			//if (timeForAnotherDirectionChange) 
			//	randomAngle = float ((rand()%zigzagAngle_)-zigzagAngle_/2);
			setDirection (myNewDir);
			setYaw (yaw() + randomAngle);		// set the direction of the enemy
			modelInstance_->setState (InstanceMD2::RUN);
			Entity::run ();
		}
		break;
	case AI_UNCARING:
		{
			static float randomAngle = 0.0f;
			static bool isIdle = true;
			//if (timeForAnotherDirectionChange) { randomAngle = float (rand()%360); isIdle = ((rand()%3) != 0); }
			setYaw(randomAngle);
			if (isIdle)
			{
				modelInstance_->setState (InstanceMD2::IDLE);			stop();
			}
			else
			{
				modelInstance_->setState (InstanceMD2::RUN);			Entity::run ();
			}
		}
		break;
	case AI_DEAD:
		modelInstance_->setState (InstanceMD2::DIE);
		stop();
		die(); //better to make him go thru his death animation first.
		//WSB: make him die() if we're done with the death animation.
		// somehow
		//if (modelInstance_->nextFrame ()== stateStart()) die();	// time to kill the monster
		//if (((InstanceMD2*) modelInstance_)->nextFrame ()== stateStart()) die();	// time to kill the monster
	}

/*	//Now set starting and ending frames for the ModelMD2
	switch (modelInstance_->state())
	{
	case InstanceMD2::IDLE:		stateStart_ = 0;	stateEnd_ = 39;		break;
	case InstanceMD2::CROUCH:	break;
	case InstanceMD2::RUN:		stateStart_ = 40;	stateEnd_ = 46;		speed_ = RUNNING_SPEED;					break;
	case InstanceMD2::JUMP:		stateStart_ = 67;	stateEnd_ = 73;		break;
	case InstanceMD2::DIE:		stateStart_ = 178;	stateEnd_ = 184;	break;
	default:		stateStart_ = 0;	stateEnd_ = 1;		break;
	}
	*/
	//stateStart_ = 0;	stateEnd_ = 39;
}

void MS3DMonster::OnDraw(const Player&)
{
	glTranslatefv(position()-worldCoord(0,5.5f,0)); 
	//he's placed too high --move him down.  The Y-value is a guess
	glRotatefv(-yaw()-90.0f,  Y_NORMAL); //need to rotate him, for some reason.
	//glRotatefv(90.0f, -X_NORMAL);
	glScalef(0.15f, 0.15f, 0.15f);
		
	((InstanceMS3D*)modelInstance_)->InstanceMS3D::animate
		(deltaT_*animSpeed, 
		 2.0f/30, 14.0f/30);
	//How did I get these #'s?  2-14 are the frames for the dwarf's "walking"
	// animation.  The dwarf is at 30 frames per second (I know this by running
	// testMS3D, debugging it, and finding its frames per second with the fps()
	// member function).
}
