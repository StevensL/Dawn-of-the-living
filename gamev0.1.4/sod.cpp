/**************************************
	SOD.CPP

	The SodEnemy class implementation
	Adapted By: DOTL DEV Team

***************************************/

#include <windows.h>
#include <gl/gl.h>
#include "sod.h"
#include "Acid.h"
#include "lcglmath.h"
#include "ogro.h"

class MS3DMonster;

using namespace lcgl;

void SodEnemy::OnCollision(Object *collisionObject)
{
		// as long as the model is not dying, it can collide with other objects
	if (modelInstance_->state() != InstanceMD2::DIE)
	{
		// if this enemy collides with another enemy
		if ((typeid(*collisionObject) == typeid(SodEnemy)) ||
			(typeid(*collisionObject) == typeid(OgroEnemy)) ||
			(typeid(*collisionObject) == typeid(MS3DMonster)))
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
			// kill the sod
			_death.play();//play the sound of the enemy dieing mac
			setAIState  (AI_DEAD);	stop();
		}
		else if (typeid(*collisionObject) == typeid(Player))//for collisions with the player mac 4/3/14
		{
			
			position_.z() = position().z()-5;//bump the model back
			setAIState(AI_UNCARING);//pauses the model
		}
	}
}

//OgroEnemy::OnPrepare and SodEnemy::OnPrepare are identical . . .
// . . .but probably should be made different, to make them distinct enemies.
void SodEnemy::OnPrepare() 
{
		
	worldCoord whereToGo =  player->position()- position() ;//goes towards the player mac 4/3/14
	_direction = whereToGo;//better as a private member mac 4/3/14

	ProcessAI();
	// now do prep
	// set _state based on AIstate
	// if AI_SCARED, then RUN
	switch (aiState)
	{
	case AI_SCARED:
		{
			setDirection (_direction);
			setYaw (yaw());		// set the direction of the enemy
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
		//if (modelInstance_->nextFrame ()== stateStart()) die();	// time to kill the monster
		if (((InstanceMD2*) modelInstance_)->nextFrame ()== stateStart()) die();	// time to kill the monster
	}

	//Now set starting and ending frames for the ModelMD2
	switch (modelInstance_->state())
	{
	case InstanceMD2::IDLE:		stateStart_ = 0;	stateEnd_ = 39;		break;
	case InstanceMD2::CROUCH:	break;
	case InstanceMD2::RUN:		stateStart_ = 40;	stateEnd_ = 46;		speed_ = RUNNING_SPEED;					break;
	case InstanceMD2::JUMP:		stateStart_ = 67;	stateEnd_ = 73;		break;
	case InstanceMD2::DIE:		stateStart_ = 178;	stateEnd_ = 184;	break;
	default:		stateStart_ = 0;	stateEnd_ = 1;		break;
	}
}

void SodEnemy::OnDraw(const Player&)
{
	glTranslatefv(position());
	glRotatefv(-yaw(),  Y_NORMAL);
	glRotatefv(90.0f, -X_NORMAL);
	glScalef(0.25, 0.25, 0.25);
		
	((InstanceMD2*)modelInstance_)->InstanceMD2::animate (stateStart_, stateEnd_, deltaT_*animSpeed);
}
