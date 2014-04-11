#define WIN32_MEAN_AND_LEAN
#define WIN32_EXTRA_LEAN

/*
	WINMAIN.CPP

	Contains WinMain, the main windows function

*/

#include <windows.h>
#include <stdlib.h>			//for srand
#include <time.h>			//for time()
#include <sstream>
#include "window.h"		
#include "engine.h"			
#include "simpengine.h"
#include "listener.h"

using namespace lcgl;

SoundSystem mySoundSystem; //This must be done before creating any Sounds
Listener	myListener (mySoundSystem);


enum {SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600, BIT_DEPTH = 16 };

Engine* myEngine = NULL;
OGLWindow myWindow ("OpenGL Game");
bool fullScreen = false;
const int centerX = (SCREEN_WIDTH/2) - (SCREEN_WIDTH/2);
const int centerY = 100;



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{

	//srand ((unsigned int) time(NULL));
	srand(200); // do this here to keep the terrain the same each time we start the program mac 3/15/14
	
	CoInitialize(NULL);

	SimpleEngine *engine = NULL;

	ostringstream str;
	str << "Welcome to Dawn of the Living v 0.1.4 (Alpha) \n\nKeys:\nW - Forward\nS - Backward\nA - Left\nD - Right";
	str << "\nLeft Mouse button shoots acid!!\nMove the mouse to look around\n\n";
	str << "Features in version: 0.1.4 \n";
	str<< "-Particle system\n -Skybox \n -Billboarded objects\n-Acid spitting\n -Dynamic HUD\n-Acid model\n -General bug fixes\n";
	str<<"-Enemies run towards player\n-Collisions with enemies\n -Collisions with player\n-New Soundtrack";
	str<<"\n\n Enjoy!!\n";
	str << "\nContributers: Mike the Monk && BeardZilla\n";
	
	MessageBox(NULL, str.str().c_str(), "Welcome!", MB_OK);

	str.clear();

	

	try
	{	
		myWindow.initialize (hInst, SCREEN_WIDTH, SCREEN_HEIGHT, 100, 100, BIT_DEPTH, fullScreen);
		SetWindowText(myWindow.hWND(), "Dawn Of The Living v 0.1.4"); 
		engine = new SimpleEngine (SCREEN_WIDTH, SCREEN_HEIGHT); //WSB: definitely remove redundant params in these 2 lines!
		engine->initialize (myWindow);
		myEngine = engine; //for use in WndProc -- for now WSB
		int loopRet = engine->EnterMessageLoop(myWindow);
		delete engine;
		return loopRet;
	}
	catch(char *sz)
	{	
		MessageBox(NULL, sz, 0, 0);
		delete engine;
	}
	catch (const OGLWindow::CantRegister&)
	{
		MessageBox(NULL, "Failed to register window class", "Error", MB_OK);
		return -1;
	}

	CoUninitialize();

	return -1;
}

LRESULT APIENTRY WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	OGLWindow *glWindow = (OGLWindow*)GetWindowLong(hWnd, GWL_USERDATA);
	static bool bLMB = false;		// left mouse button
	static bool bRMB = false;		// right mouse button
	static LPARAM Drag;			// mouse drag


	// make sure window has been created
	if ((glWindow == NULL) && (uMsg != WM_CREATE))
	{	
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	// dispatch messages
	switch (uMsg)
	{	
		case WM_CREATE:			
		{
			HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
			glWindow = (OGLWindow*)(((LPCREATESTRUCT)lParam)->lpCreateParams);

			SetWindowLong(hWnd, GWL_USERDATA, (LONG)glWindow);
			glWindow->createContexts(hWnd);
			return 0;
		}
		case WM_QUIT:
		case WM_CLOSE:				
		case WM_DESTROY:			
			glWindow->destruct();
			PostQuitMessage(0);
			return 0;

		case WM_ACTIVATEAPP:		// activate app
			
			if (wParam)
			{	
				if (glWindow->isFullScreen ()) glWindow->beginFullScreen();

				ShowWindow(hWnd, SW_RESTORE);
				UpdateWindow(hWnd);
			}
			else
			{	
				ShowWindow(hWnd, SW_MINIMIZE);
				UpdateWindow(hWnd);
				
				if (glWindow->isFullScreen ()) glWindow->endFullScreen();
			}
			return 0;

		case WM_PALETTECHANGED:	glWindow->makePaletteChanged(wParam);	return 0;
		case WM_QUERYNEWPALETTE:return glWindow->queryNewPalette();
		case WM_PAINT:			glWindow->paint();					return 0;

		case WM_SIZE:	if (wParam != SIZE_MINIMIZED) glWindow->resize (LOWORD (lParam), HIWORD (lParam));
			return 0;

		case WM_LBUTTONDOWN:		// left mouse button
			if (!myEngine->useDInput ())
			{
				SetCapture(hWnd);
				Drag = lParam;
				bLMB = true;
				myEngine->OnMouseDownL(myEngine->GetNormalizedPosX(lParam), myEngine->GetNormalizedPosY(lParam));
			}
			break;

		case WM_RBUTTONDOWN:		// right mouse button
			if (!myEngine->useDInput ())
			{
				SetCapture(hWnd);
				Drag = lParam;
				bRMB = true;
				myEngine->OnMouseDownR(myEngine->GetNormalizedPosX(lParam), myEngine->GetNormalizedPosY(lParam));
			}
			break;

		case WM_MOUSEMOVE:			// mouse movement
		{	
			if (!myEngine->useDInput ())
			{
				int x  = myEngine->mouseX () = myEngine->GetMouseX(lParam); 
				int y  = myEngine->mouseY () = myEngine->GetMouseY(lParam);
				int dx = x - myEngine->GetMouseX(Drag);
				int dy = y - myEngine->GetMouseY(Drag);

				myEngine->OnMouseMove(x,y, glWindow->width (), glWindow->height ());

				if (GetCapture() == hWnd)
				{
					// left mouse button
					if (bLMB) myEngine->OnMouseDragL(x,y, dx,dy);
					
					// right mouse button
					if (bRMB) myEngine->OnMouseDragR(x,y, dx,dy);
					
					Drag = lParam;
				}
			}
			break;
		}

		case WM_LBUTTONUP:			// left button release
			if (!myEngine->useDInput ())
			{
				if ((GetCapture() == hWnd) && !bRMB)
					ReleaseCapture();

				bLMB = false;
				myEngine->OnMouseUpL();
			}
			break;

		case WM_RBUTTONUP:			// right button release
			if (!myEngine->useDInput ())
			{
				if ((GetCapture() == hWnd) && !bLMB)
				{
					ReleaseCapture();
				}
				bRMB = false;
				myEngine->OnMouseUpR();
			}
			break;

		case WM_KEYUP:   if (!myEngine->useDInput ())	myEngine->OnKeyUp  ((int)wParam); 		return 0;
		case WM_KEYDOWN: if (!myEngine->useDInput ())	myEngine->OnKeyDown((int)wParam);		return 0;
		default:		 break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

