/************************************************

  window class:  take care of getting your windows code started.
  See class examples for how it's used.

  Note these global constants you can use:

  WSB 1-06; major revisions 1-13

*************************************************/

#ifndef __OGLWINDOW_H__
#define __OGLWINDOW_H__

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <string>
#include "LCGLException.h"
#include "inputsystem.h"

namespace lcgl
{
	class Window
	{
	public:
		class CantBeCopied: public LCGLException {}; 
			//I can't think of a reason to allow a Window to be copied
		class Uninitialized {}; //Exceptions
		class CantRegister {};
		class CantInitialize {};

		enum { DEFAULT_WIDTH = 800, DEFAULT_HEIGHT = 600 };

		Window(const char *szName = "")	{ hDC_ = NULL; name_ = szName;	}


		Window (const Window&) { throw CantBeCopied (); }
		virtual ~Window();

		void operator= (const Window&) { throw CantBeCopied (); }

		HWND initialize (HINSTANCE, 
			int width = DEFAULT_WIDTH, 
			int height = DEFAULT_HEIGHT, 
			int x=0, int y=0, //top left corner
			int bitsPerPixel = 32, 
			bool fullScreen = false,
			bool StencilBufferEnabled = false);
         		//See Chapter 12 for the need for StencilBufferEnabled

		// Windows message handling functions
		void createContexts (HWND, bool StencilBufferEnabled = false); //WM_CREATE
         		//See Chapter 12 for the need for StencilBufferEnabled
		void close ();
		void destruct();					// WM_DESTROY
		void makePaletteChanged(WPARAM wParam);	// WM_PALETTECHANGED.  What's it for?  Got me.  WSB
		BOOL queryNewPalette();				// WM_QUERYNEWPALETTE.  What's it for?  Got me.  WSB
		void paint();						// WM_PAINT
		void resize			(int, int);				// WM_SIZE
		void resizeOrtho	(int, int); //orthogonal projection, rarely used
		void resizeOrtho2D  (int, int); //also rarely used

		void beginFullScreen();
		void endFullScreen();

		void setName (const char* s) { name_ = s; }

		HWND hWND ()		 const { return hWND_; }
		static HDC	hDC		  ()	   { return hDC_;		}
		HINSTANCE	hInstance () const { return hInstance_; }
		unsigned int  width()		 const { return width_; }
		unsigned int  height()		 const { return height_; }
		bool isFullScreen () const { return isFullScreen_; }
		int  bits  ()		 const { return bits_; }
		int  aspect()		 const { return aspect_; }

		bool isActive () const { return isActive_; };
		bool& isActive()       { return isActive_; }; //we should be able to change this, too

		virtual bool OnCreate() { return true; } //Do I really need these?
		virtual bool OnClose() { return true; }
		virtual void OnSize() { }

	private:
		void setupPalette();		// setup the palette

		// this must be called before the class is used.  Called in initialize
		static void Register (HINSTANCE hInst);

		static HDC   hDC_;						// device context.  as I recall, it's static to force you to have only 1 window.
												//  There was some reason I did this.  If I ever need 2 windows, try making this nonstatic
		HINSTANCE	hInstance_;
		HPALETTE	hPalette_;	// palette
		HGLRC		hRC_;		// rendering context
		HWND		hWND_;		// window handle

		int bits_;
		int aspect_;
		unsigned int width_, height_;

		bool isFullScreen_;
		bool isActive_;

		std::string name_;
	};

	void InitializeWorld();			//This function is for OpenGL, not making a window
									//Some common initializations in using 3D

} //end namespace lcgl

#endif