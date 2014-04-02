#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include "Acid.h"
#include "worldCoord.h"
#include "lcglmath.h"
#include "ogro.h"
#include "world.h"

using namespace lcgl;

class MS3DMonster;

#define ACID_SPEED 75.0f

Acid::Acid (const ModelMD2& rocketModel, const Texture2D& explosionTexInit, World* world, const Direction& newDirection) 
	: Entity (new InstanceMD2 (rocketModel, DEFAULT_ENTITY_INTERPOLATION), world, 1.0f, NULL_VECTOR, newDirection, ACID_SPEED)
{
	_explosionTexture = (Texture2D*) &explosionTexInit;
	Entity::run();
	_distanceTraveled = 0.0;
	_isExploding = false;
	_explosion = NULL;

	Load();
}

Acid::~Acid()
{
	if (_explosion)
	{
		_explosion->KillSystem();
		delete _explosion; 
		_explosion = NULL;
	}
}

void Acid::OnAnimate(float deltaTime)
{
	float cosYaw   = (float)cos(Deg2Rad(yaw()));
	float sinYaw   = (float)sin(Deg2Rad(yaw())); 
	float sinPitch = (float)sin(Deg2Rad(pitch()));

	// added line
	float cosPitch = (float)cos(Deg2Rad(pitch()));

	float amountTraveled = speed() * deltaTime;

	position_.x() += float(cosYaw)*cosPitch*amountTraveled;
	position_.y() += float(sinPitch)*amountTraveled;
	position_.z() += float(sinYaw)*cosPitch*amountTraveled;

	_distanceTraveled += position().Length();

	if (_isExploding)		_explosion->Update(deltaTime);

	else if (_distanceTraveled >= 500000.0f)	//explode the rocket if it goes too far
											//I don't know why we would do this
											//but ok
	{
		_isExploding = true;
		stop();
		_explosion = new Explosion(10, position(), 0.1f, *_explosionTexture);
	}
}

void Acid::OnCollision(Object *collisionObject)
{
	
	glDisable(GL_LIGHTING);				//do this so the Skybox does not mess up mac 3/26/14 

	if (!_isExploding && entitySound)	//shouldn't need both...but something is resetting _isExploding
										 //maybe this will make it not explode twice!
	{
		bool makeItExplode = false;
		
		if (typeid(*collisionObject) == typeid(Terrain))
		{
			if (((Terrain*)collisionObject)->GetHeight(position().x(), 
														position().z()) + size ()
								>= position().y())
				makeItExplode = true;
		}
		else if (typeid(*collisionObject) == typeid(SodEnemy )) makeItExplode = true;
		else if (typeid(*collisionObject) == typeid(OgroEnemy)) makeItExplode = true;
		else if (typeid(*collisionObject) == typeid(MS3DMonster)) makeItExplode=true;

		if (makeItExplode)
		{
			_isExploding = true;
			stop ();
			_explosion = new Explosion(500, position(), 8.0, *_explosionTexture);
			PlaySound();
			entitySound = NULL; //make it so we can't play the sound twice
		}
	}
}

void Acid::OnDraw(const Player&)
{
	if (!_isExploding)	// if the rocket has not yet exploded, draw the rocket
	{
		glEnable		(GL_TEXTURE_2D);
		glColor3fv		(GREEN);
		glTranslatefv	(position());
		glRotatefv		(-yaw(), Y_NORMAL);
		glRotatefv		(pitch(),      Z_NORMAL);
		glScalef		(0.025f, 0.025f, 0.025f);
		((InstanceMD2*)modelInstance_)->render			();
		glDisable		(GL_TEXTURE_2D);
	}
	else	// draw _explosion
	{
		glDisable(GL_FOG);		_explosion->Render();		glEnable(GL_FOG);
	}
}

void Acid::OnPrepare()
{
	// perform collision detection from this entity with all other objects in world
	if (!_isExploding)		
		world_->ProcessCollisions (this);
	else if (_explosion->IsDead()) //&& ! entitySound->isPlaying())
	{
		_explosion->KillSystem();
		delete _explosion;
		_explosion = NULL;
		_isExploding = false;

		die ();
	}
}
