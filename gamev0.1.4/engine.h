#ifndef __ENGINE_H
#define __ENGINE_H

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

/************************************************************************/
/*  ENGINE.H

	The main engine class

	Description: Engine is the main engine class, derived from OGLWindow.
	It holds virtual functions GameCycle, OnPrepare, 
	and world. Engine is also the location of the main
	Windows message loop.

			Adapted By: DOTL Dev Team															*/
/************************************************************************/
	 



#include <windows.h>
#include <dinput.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "window.h"
#include "lcglexception.h"
#include "inputsystem.h"


using namespace lcgl;

class World;
typedef Window OGLWindow;

class Engine 
{
public:
	static const float DEFAULT_MOUSE_SENSITIVITY;

	class CantBeCopied: public LCGLException {};
	Engine() 
	{ 
		useDInput_ = true; mouseSensitivity_ = DEFAULT_MOUSE_SENSITIVITY;
	}
	void initialize (const OGLWindow& myWindow);

	Engine (const Engine& other) { *this = other; }
	void operator= (const Engine& other) { throw CantBeCopied (); }

	LRESULT EnterMessageLoop(const OGLWindow& myWindow);
	bool		useDInput () const { return useDInput_; }
	int			mouseX    () const { return mouseX_;    }
	int			mouseY    () const { return mouseY_;	}
	int&		mouseX    ()       { return mouseX_;    }
	int&		mouseY    ()       { return mouseY_;	}
	float		mouseSensitivity () const { return mouseSensitivity_; }
	virtual void GameCycle(float deltaTime);

	virtual void OnPrepare() {}					// setup OpenGL for frame
	virtual World& world() = 0;//{ return NULL; }

	virtual void CheckInput(float deltaTime);

	virtual void OnMouseDownL(float x, float y) { }
	virtual void OnMouseDownR(float x, float y) { }
	virtual void OnMouseUpL() { }
	virtual void OnMouseUpR() { }
	virtual void OnMouseMove(int x, int y, int centerX, int centerY) { }
	virtual void OnMouseMove(int deltaX, int deltaY) { }
	virtual void OnMouseDragL(int x, int y, int dx, int dy) { }
	virtual void OnMouseDragR(int x, int y, int dx, int dy) { }
	virtual void OnCommand(WORD wNotifyCode, WORD wID, HWND hWndCtrl) { }
	virtual void OnContextMenu(HWND hWnd, int x, int y) { }
	virtual void OnKeyUp(int nVirtKey) { }
	virtual void OnInitMenu(HMENU hMenu) { }
	virtual void OnKeyDown(int nVirtKey) { }
	virtual void OnChar(char c) { }
	void MouseMove(int x, int y);		// WM_MOUSEMOVE

	//these really shouldn't be members of anything:
	int Engine::GetMouseX(LPARAM lParam){	return LOWORD(lParam);}
	int Engine::GetMouseY(LPARAM lParam){	return HIWORD(lParam);}

	float GetNormalizedPosX(LPARAM lParam);	// get normalized mouse coordinates
	float GetNormalizedPosY(LPARAM lParam);	// between (-1.0 and 1.0)

private:
	bool useDInput_;		// true if dinput is enabled
	InputSystem inputSystem_;
protected:
	float mouseSensitivity_;
	int mouseX_;			//Why are these members, rather than things calculated in the moment? WSB
	int mouseY_;			//They dont' actually seem to be used, but I left 'em in for now.  maybe useful later?
};

#endif