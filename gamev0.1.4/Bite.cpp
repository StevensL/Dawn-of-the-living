#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include "Bite.h"
#include "worldCoord.h"
#include "lcglmath.h"
#include "ogro.h"
#include "world.h"
#include "gui.h"

using namespace lcgl;

class MS3DMonster;

#define BITE_SPEED 15.0f

Bite::Bite(const ModelMD2& biteModel, const Texture2D& explosionTexInit, World* world, const Direction& newDirection) 
	//: Entity (new InstanceMD2 (biteModel, DEFAULT_ENTITY_INTERPOLATION), world, 1.0f, NULL_VECTOR, newDirection, BITE_SPEED)) )
{
	_biteTexture = (Texture2D*) &explosionTexInit;
	Entity::run();S
	_distancetraveled = 0.0;
	_isJawsClosed = false;
	_JawsClosed = NULL;

	Load();
}

Bite::~Bite(void)
{
	if (_isJawsClosed)
	{
		_JawsClosed->KillSystem();
		delete _JawsClosed;
		_JawsClosed = NULL;
	}
}

void Bite::OnAnimate(float deltaTime)
{
	float cosYaw = (float)cos(Deg2Rad(yaw()));
	float sinYaw   = (float)sin(Deg2Rad(yaw())); 
	float sinPitch = (float)sin(Deg2Rad(pitch()));
	float cosPitch = (float)cos(Deg2Rad(pitch()));

	float amountTraveled = speed() * deltaTime;

	position_.x() += float(cosYaw)*cosPitch*amountTraveled;
	position_.y() += float(sinPitch)*amountTraveled;
	position_.z() += float(sinYaw)*cosPitch*amountTraveled;

	_distanceTraveled += position().Length();

	if (_isJawsClosed)		_JawsClosed->Update(deltaTime);

	else if (_distanceTraveled >= 5.0f)	//close the jaws if it goes too far
											//I don't know why we would do this
											//but ok
	{
		_isJawsClosed = true;
		stop();
		_JawsClosed = new JawsClosed(10, position(), 0.1f, *_explosionTexture);
	}
}
