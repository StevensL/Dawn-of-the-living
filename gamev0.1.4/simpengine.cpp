#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include "simpengine.h"

/***************************************
	SIMPENGINE.CPP

	The SimpEngine implementation
	Adapted By DOTL DEV Team

****************************************/

using namespace lcgl;

void SimpleEngine::OnPrepare()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
}

void SimpleEngine::OnMouseDownL(float x, float y) 
{
	if (!gameWorld_.IsGameDone())	gameWorld_.player.FireWeapon();
}

void SimpleEngine::OnKeyDown(int nVirtKey)
{
	static const float MOUSE_SENSITIVITY_INCREMENT = 0.05f;
	static const float MOUSE_SENSITIVITY_MINIMUM   = 0.05f;

	if (!gameWorld_.IsGameDone())
	{
		switch (nVirtKey)
		{
		case VK_UP:			gameWorld_.player.speedUp   ( 2.0f);	break;
		case VK_DOWN:		gameWorld_.player.speedUp   (-2.0f);	break;
		case VK_RIGHT:		gameWorld_.player.speedRight( 1.0f);	break;
		case VK_LEFT:		gameWorld_.player.speedRight(-1.0f);	break;
		case VK_ESCAPE:		PostQuitMessage(0);									break;
		case VK_ADD:		mouseSensitivity_ += MOUSE_SENSITIVITY_INCREMENT;	break;
		case VK_SUBTRACT:	mouseSensitivity_ -= MOUSE_SENSITIVITY_INCREMENT;
							if (mouseSensitivity () < 0.05)	
								mouseSensitivity_ = MOUSE_SENSITIVITY_MINIMUM;
							break;
		}
	}
	else if ((nVirtKey) == VK_ESCAPE)	PostQuitMessage(0);
}

void SimpleEngine::OnMouseMove(int deltaX, int deltaY)
{
	if (!gameWorld_.IsGameDone())
	{
		gameWorld_.player.setYaw  (gameWorld_.player.yaw()   +  deltaX*mouseSensitivity ());
		gameWorld_.player.setPitch(gameWorld_.player.pitch() -  deltaY*mouseSensitivity ());
	}
}

void SimpleEngine::OnMouseMove(int x, int y, int centerX, int centerY)
{
	static const float MOUSE_SENSITIVITY_FACTOR = 0.2f; 
							//Why?  I guess 1.0 was too much!
	static float oldX;
	static float oldY;

	float mX, mY;

	mX = (float)x;
	mY = (float)y;

	if (mX < centerX/2) gameWorld_.player.setYaw (gameWorld_.player.yaw()  - MOUSE_SENSITIVITY_FACTOR*mouseSensitivity ());
	if (mX > centerX/2)	gameWorld_.player.setYaw (gameWorld_.player.yaw()  + MOUSE_SENSITIVITY_FACTOR*mouseSensitivity ());

	if (mY < centerY/2)	gameWorld_.player.setPitch (gameWorld_.player.pitch()+  MOUSE_SENSITIVITY_FACTOR*mouseSensitivity ());
	if (mY > centerY/2)	gameWorld_.player.setPitch (gameWorld_.player.pitch()- MOUSE_SENSITIVITY_FACTOR*mouseSensitivity ());

	oldX = mX;
	oldY = mY;
}