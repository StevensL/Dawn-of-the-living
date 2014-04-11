#ifndef __ROCKET_H
#define __ROCKET_H


/***********************
Acid.h
players primary weapon
adapted by Dotl Dev Team 

***********************/

#include "explosion.h"
#include "terrain.h"
#include "player.h"
#include "object.h"
#include "worldCoord.h"
#include "entity.h"
#include "texture.h"

using namespace lcgl;

class Acid : public Entity
{
private:
	void SetupExplosionTexture();

	void OnAnimate	(float deltaTime);
	void OnCollision(Object *collisionObject);
	void OnDraw		(const Player& camera);
	void OnPrepare	();

	float		_distanceTraveled;	// distance rocket has travelled
	bool		_isExploding;

	Texture2D*	_explosionTexture;
	Explosion*	_explosion;

	lcgl::Sound acidInitialSound;

public:
	Acid	(const ModelMD2& model, const Texture2D& explosionTexInit, World* world, const Direction& newDirection);
	~Acid	();
	Acid (const Acid&);			//intentionally not written
	void operator= (const Acid&); //intentionally not written

	void Load() {}
};

#endif