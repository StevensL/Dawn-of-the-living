/*
	OBJECT.H

  	The Object class

	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: 3/29/2001
	Description: The Object class is derived from Node and
			   represents an object in the game world.

*/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "object.h"

using namespace lcgl;

void Object::Draw(const Player& player) 
{
  glPushMatrix();
	   OnDraw(player);          
  glPopMatrix();
}
	 
void Object::ProcessCollisions(Object *obj) 
 {
      // if this object's bounding sphere collides with obj's sphere
      // and obj is not this object
      if (((obj->position() - position()).Length() <= (obj->size() + size())) &&
           (obj != ((Object*)this)))
        OnCollision(obj);        // perform this object's collision with obj
 }

