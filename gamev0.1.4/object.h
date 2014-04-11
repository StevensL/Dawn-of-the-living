#ifndef __OBJECT_H
#define __OBJECT_H

#include "worldCoord.h"
#include "objectSet.h"
#include "lcglexception.h"

/**************************************************************
	OBJECT.H

  	The Object class

	Description: The Object class is derived from Node and
			   represents an object in the game world.
			   Adapted By: DOTL Dev Team

**************************************************************/

using namespace lcgl;

class Player; class World;

class Object
{
protected:

     // perform basic physics on the object
     virtual void OnAnimate(float deltaTime) 
     {
          position_ += velocity()     * deltaTime;
          velocity_ += acceleration() * deltaTime;
     }

     // draw the object given the player's position
	 //Why do we need this?  Because some objects may be shown depending
	 //on camera's location -- things in the sky like the sun, for example
     virtual void OnDraw(const Player& player) {} 
     virtual void OnCollision(Object *collisionObject)	{}
     virtual void OnPrepare()							{}

	World* world_;			//What World do I belong to?

//private:
    float size_;           // size of bounding sphere (radius)
    bool isDead_;
    worldCoord acceleration_;    
    worldCoord velocity_;        
	
public:
	class CantBeCopied: public LCGLException {};

    worldCoord position_;        

    Object (World* world, float newSize = 1.0f,
		     const worldCoord& newPos = NULL_VECTOR,
			 const worldCoord& newVel = NULL_VECTOR,
			 const worldCoord& newAcc = NULL_VECTOR) 
			 :
		world_ (world), size_ (newSize), position_ (newPos), velocity_ (newVel), acceleration_ (newAcc)
	 { 
		 isDead_ = false; 
	 }
    ~Object() {}

	Object (const Object& other)	{ *this = other;		 }	//throw an exception
	void operator= (const Object&)	{ throw CantBeCopied (); }	//throw an exception

    virtual void load  () {}
    virtual void Unload() {}

	void die		()			{ isDead_ = true; }
	bool isDead	()			{ return isDead_; }
	float size		() const	{ return size_; }

	const worldCoord& position     () const { return position_    ; }
	const worldCoord& velocity     () const { return velocity_    ; }
	const worldCoord& acceleration () const { return acceleration_; }

	void moveTo (const worldCoord& newPos) { position_ = newPos; }
	void move   (const worldCoord& extra)  { position_+= extra;  }

	void setElevation (float yNew) { position_.y() = yNew; }

	void SetWorld (World* world) { world_ = world; }

	void Draw(const Player& player) ;
	void animate(float deltaTime) 
	{
		OnAnimate(deltaTime);  if (isDead ())  delete this; //Is this the best place to do this?
	}

    void ProcessCollisions (Object *obj) ;
	void Prepare() {      OnPrepare();    }
};


#endif