#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include "worldCoord.h"
#include "lcglmath.h"
#include "ogro.h"
#include "world.h"
#include "Scratch.h"
#include "Entity.h"

using namespace lcgl;

#define SCRATCH_SPEED 50.0f

Scratch::Scratch(const ModelMD2& scratchModel, World* world, const Direction& newDirection) 
	//: Entity (new InstanceMD2 (scratchModel, DEFAULT_ENTITY_INTERPOLATION), world, 1.0f, NULL_VECTOR, newDirection, SCRATCH_SPEED)
{

}

Scratch::~Scratch(void)
{

}
