#ifndef __ENEMY_H
#define __ENEMY_H

/*********************************
	ENEMY.H

	The Enemy class

	Description: The Enemy class

	Adapted By: DOTL Dev Team

*************************************/

#include "entity.h"

using namespace lcgl;

class Player;
class Terrain;

enum AIState_t
{
	AI_UNCARING,	// enemy is not scared and does not care
	AI_SCARED,		// enemy is scared and running away
	AI_DEAD
};

class Enemy : public Entity
{
protected:
     int		hitPoints;		// hit points the enemy has left
     AIState_t	aiState;		// state of enemy thought
     Player* player;
	 float radiusOfAwareness_;   // how close you can get to an enemy before he notices you
	 int zigzagAngle_;         // how much the enemy will veer left or right when running

	 virtual void OnPrepare  () = 0;

     void OnCollision(Object *collisionObject)
     {
          // if this enemy collides with another enemy
          if (typeid(*collisionObject) == typeid(Enemy))
          {
			  //WSB:  I don't like that this makes reference to InstanceMD2
			  // -- inside enemy.h, which may not use MD2's.
			  // Make this virtual, and put it into Ogro & Sod?
               modelInstance_->setState (InstanceMD2::IDLE); stop();
          }
          // if this enemy collides with the terrain (always)
          else if (typeid(*collisionObject) == typeid(Terrain))
               setElevation(((Terrain*)collisionObject)->GetHeight(position().x(), position().z()) + size());
     }

public:
	void setAIState (AIState_t foo) { aiState = foo; }
 
	Enemy (Instance* instance,
				World* world, 
				Player* p, 
				float newRadOfAwareness, 
				int newZZAngle,
				float newSize = DEFAULT_ENTITY_SIZE, 
				const worldCoord& newPos = NULL_VECTOR, 
				const Direction& newDirection = Direction(0,0), 
				float newSpd = DEFAULT_RUNNING_SPEED,
				float newAnimationSpeed = DEFAULT_ANIMATION_SPEED 
				) 
		: Entity (instance, world, newSize, 
			 newPos, 
			 newDirection, 
			 newSpd,
			 newAnimationSpeed),
		radiusOfAwareness_ (newRadOfAwareness), 
		zigzagAngle_ (newZZAngle)
     {
          hitPoints = 100;    // start with 100 hit points
		  modelInstance_->setState(InstanceMD2::IDLE);    // idle state
          player = p;
     }

    ~Enemy() {}
	Enemy (const Enemy& other) : Entity (other) {}					//throws exception
	void operator= (const Enemy& other) { Entity::operator= (other); }//throws exception

	void OnProcessAI();
	void ProcessAI() { OnProcessAI(); }
};

#endif