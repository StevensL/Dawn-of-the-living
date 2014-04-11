#ifndef __SIMPENGINE
#define __SIMPENGINE

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

/**************************************************************************
	SIMPENGINE.H

	The SimpEngine class
	Description: The SimpEngine class is the main interface for SimpEngine.
			   Derived from Engine, provides the camera and world objects.

			   Adapted By DOTL Dev Team

*****************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "engine.h"
#include "world.h"

using namespace lcgl;

class SimpleEngine : public Engine
{
public:
	SimpleEngine(int w, int h) : gameWorld_ (w, h)
	{
	}
	SimpleEngine (const SimpleEngine&);
	void operator= (const SimpleEngine&);

protected:
	World&  world	() { return gameWorld_; }

	void OnPrepare		();
	void OnMouseDownL	(float x, float y);
	void OnMouseMove	(int deltaX, int deltaY);
	void OnMouseMove	(int x, int y, int centerX, int centerY);
	void OnKeyDown		(int nVirtKey);

private:
	World	gameWorld_;

};


#endif