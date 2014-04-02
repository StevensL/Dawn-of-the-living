#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <stdlib.h>

#include "engine.h"
#include "timer.h"
#include "world.h"
#include "skybox.h"
#include "worldCoord.h"
#include <ctime>

using namespace lcgl;

extern OGLWindow myWindow;

const float Engine::DEFAULT_MOUSE_SENSITIVITY = 2.0f;

float Engine::GetNormalizedPosX(LPARAM lParam)
{	
	return mouseSensitivity () * (float)((short)LOWORD(lParam) - myWindow.width()/2) / myWindow.aspect ();
}

float Engine::GetNormalizedPosY(LPARAM lParam)
{
	return mouseSensitivity () * (float)((short)HIWORD(lParam) - myWindow.height()/2) / myWindow.aspect ();
}

void Engine::initialize (const OGLWindow& myWindow)
{
	try
	{
		SetFocus (myWindow.hWND ()); //maybe this will force focus here, so that we won't need to catch exceptions?
									 //HEY, IT WORKS!  If you're doing something else while this starts,
									 // it grabs focus and does not then fail to initialize.
		inputSystem_.Initialize(myWindow.hWND(), (HINSTANCE)GetModuleHandle(NULL), true, IS_USEKEYBOARD | IS_USEMOUSE);
		inputSystem_.Acquire();
		inputSystem_.Update();
	}
	//Probably a better solution here is to grab focus, or wait until we get focus.
	//  Or maybe it would be good to do this only on a WM_ACTIVATE, because then you know you've got focus!
	catch (InputSystem::CantCreate&)
	{
		MessageBox (myWindow.hWND(), "Can't create InputSystem.  Try making sure this program has focus when you start it.",
					"Fatal error", MB_OK);
		exit (-1);
	}
	catch (Keyboard::CantCreate&)
	{
		MessageBox (myWindow.hWND(), "Can't create Keyboard handler.  Try making sure this program has focus when you start it.",
					"Fatal error", MB_OK);
		exit (-1);
	}
	catch (Mouse::CantCreate&)
	{
		MessageBox (myWindow.hWND(), "Can't create Mouse handler.  Try making sure this program has focus when you start it.",
					"Fatal error", MB_OK);
		exit (-1);
	}
}

void Engine::CheckInput(float deltaTime)
{
	static float buttonDelta = 0.0f;

	int mouseDeltaX, mouseDeltaY;	// changes in the mouse position

	// decrease amount of time until next possible recognized button pressing
	buttonDelta -= deltaTime;

	if (buttonDelta < 0.0f)		buttonDelta = 0.0f;

	//Brandon Johnson fix 1-28-2010:  this prevents crashing on change of focus,
	// either for Alt-Tab or from breaking in the debugger.
	try 
	{
		inputSystem_.Update();

		// retrieve the latest mouse movements
		inputSystem_.GetMouseMovement(mouseDeltaX, mouseDeltaY);
	
		OnMouseMove(mouseDeltaX, mouseDeltaY);

		if (inputSystem_.KeyDown(DIK_W))		OnKeyDown(VK_UP);
		if (inputSystem_.KeyDown(DIK_S))		OnKeyDown(VK_DOWN);
		if (inputSystem_.KeyDown(DIK_A))		OnKeyDown(VK_LEFT);
		if (inputSystem_.KeyDown(DIK_D))		OnKeyDown(VK_RIGHT);
		if (inputSystem_.KeyDown(DIK_ADD))		OnKeyDown(VK_ADD);
		if (inputSystem_.KeyDown(DIK_SUBTRACT))	OnKeyDown(VK_SUBTRACT);
		if (inputSystem_.KeyDown(DIK_ESCAPE))	OnKeyDown(VK_ESCAPE);
		if (inputSystem_.ButtonDown(Mouse::LEFT))
		{
			if (buttonDelta == 0.0f)
			{
				OnMouseDownL(0,0);
				buttonDelta = 0.5f;
			}
		}
	}
	catch(...)
	{
	}
}

void Engine::GameCycle(float deltaTime)
{
	

	if (useDInput_)	CheckInput(deltaTime);



	OnPrepare();				// setup opengl for frame (clear, identity)

	world().Prepare();// prepare objects and perform collisions
	
	
	
	world().AnimateParticles(deltaTime);//set up particle system mac 3/15/14

	

	world().Animate(deltaTime);	// move/orient objects

	
	world().Draw();				// draw objects


}

LRESULT Engine::EnterMessageLoop(const OGLWindow& myWindow)
{
	MSG msg;
	Timer timer;

	timer.Init();

	for (;;)
	{	
		GameCycle(timer.GetElapsedSeconds()); SwapBuffers(myWindow.hDC());			

		while (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			// we always update if there are any events, even if we're paused
			if (!GetMessage (&msg, NULL, 0, 0))	return msg.wParam;

			TranslateMessage (&msg);
   			DispatchMessage (&msg);
		}
	}

	return msg.wParam;
}