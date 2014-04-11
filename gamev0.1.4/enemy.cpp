/*****************************************
	ENEMY.H

	The Enemy class

	Description: The Enemy class

	Adapted By DOTL Dev Team

*******************************************/

#include "enemy.h"
#include "player.h"

void Enemy::OnProcessAI()
{
     if (aiState != AI_DEAD)
     {
		// calculate distance from player
		worldCoord diff = player->position() - position(); 

        // if the player is close enough, the enemy should become scared
        if (diff.magnitude() < radiusOfAwareness_)	aiState = AI_SCARED;
        else										aiState = AI_UNCARING;
     }
}

/*
void Enemy::OnPrepare() 
{
		//Here we keep track of how long it was since the last time we randomly changed direction.
		// This is so we don't do it in each frame -- it looks like the monster is having a fit!  WSB

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
		if (modelInstance_->nextFrame ()== stateStart()) die();	// time to kill the monster
	}

	// do prep for MD2 Model states
	Entity::OnPrepare();
}
*/