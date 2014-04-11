#ifndef MS3DMONSTER_H
#define MS3DMONSTER_H

/*********************************************************
	OGRO.H

	The SodEnemy class

	Description: The OgroEnemy represents the Ogro model
			   enemy. This enemy is considered "dumb",
			   but has a thicker coat of armor.
			   Adapted by: DOTL Dev Team

*********************************************************/

#include "enemy.h"
#include "modelms3d.h"

using namespace lcgl;

//class ostream;

#define MS3DMONSTER_RADIUS_OF_AWARENESS 100.0f //how close you can get to a monster before he notices you
#define MS3DMONSTER_ZIGZAG_ANGLE         90 //how much monster will veer left or right when running away
#define MS3DMONSTER_DEFAULT_ANIMATION_SPEED 0.5f //how fast animation moves

class MS3DMonster : public Enemy
{
protected:
	void OnCollision(Object *collisionObject);
	float stateStart_, stateEnd_;	// state keyframe start/end
	float stateStart () const { return stateStart_; } //used by Ogro and Sod
	lcgl::Direction _direction;//private direction mac 4/3/14
	lcgl::Sound _death;

public:
	MS3DMonster(const ModelMS3D& model,
				World* world, Player* p, 
				float x=0.0f, 
				float z=0.0f) 
		: Enemy (new InstanceMS3D(model), 
				 world, 
				 p, 
				 MS3DMONSTER_RADIUS_OF_AWARENESS, 
				 MS3DMONSTER_ZIGZAG_ANGLE,
				 DEFAULT_ENTITY_SIZE,
				 worldCoord (x,0,z),
				 Direction(0,0),
				 DEFAULT_RUNNING_SPEED,
				 MS3DMONSTER_DEFAULT_ANIMATION_SPEED
				 ), _death("sounds//dyinggroan.wav")
	{ 
		stateStart_ = 0.0;
		stateEnd_ =(float)model.totalTime(); //our state is ALL moves
					//This is wrong, but I'm just trying to get
					// it to work at all.  WSB 1-27-2010
	}
	~MS3DMonster() {}
	MS3DMonster (const MS3DMonster&);		//intentionally not written
	void operator=(const MS3DMonster&);	//intentionally not written

	void OnPrepare();
	void OnDraw(const Player&);
	//void print (ostream&) const;
};

#endif
