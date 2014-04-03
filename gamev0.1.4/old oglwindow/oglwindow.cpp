/************************************************

  window class:  take care of getting your windows code started.
  See class examples for how it's used.

  WSB 1-06

  Major updates 1-13, heavily informed by Astle's OGLWindow class
*************************************************/

#define WIN32_MEAN_AND_LEAN
#define WIN32_EXTRA_LEAN

#include "oglwindow.h"
#include "color.h"

//This is used here in window.cpp, but it'll be defined by the programmer that uses
// this library.  So it's not in the lcgl namespace.
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

namespace lcgl
{
	HDC Window::hDC_ = NULL;

	//Maybe this next one should be local to window.cpp?
	void setScreenMode (DWORD& dwExStyle, DWORD& dwStyle, bool fullScreen, int width, int height, int bits);

	void Window::Register (HINSTANCE hInst)
	{
		WNDCLASSEX windowClass;		// window class

		// fill out the window class structure
		windowClass.cbSize			= sizeof(WNDCLASSEX);
		windowClass.style			= CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc		= WndProc;
		windowClass.cbClsExtra		= 0;
		windowClass.cbWndExtra		= 0;
		windowClass.hInstance		= hInst;
		windowClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);	// default icon
		windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);		// default arrow
		windowClass.hbrBackground	= NULL;								// don't need background
		windowClass.lpszMenuName	= NULL;								// no menu
		windowClass.lpszClassName	= "Engine"; //"MyClass";
		windowClass.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);		// windows logo small icon

		// register the windows class
		if (!RegisterClassEx(&windowClass))
			throw CantRegister ();
	}

	// function to set the pixel format for the device context
	static void SetupPixelFormat(HDC hDC, bool StencilBufferEnabled = false)
	{
		int nPixelFormat;					// our pixel format index

		static PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),	// size of structure
			1,								// default version
			PFD_DRAW_TO_WINDOW |			// window drawing support
			PFD_SUPPORT_OPENGL |			// OpenGL support
			PFD_DOUBLEBUFFER,				// double buffering support
			PFD_TYPE_RGBA,					// RGBA color mode
			32,								// 32 bit color mode
			0, 0, 0, 0, 0, 0,				// ignore color bits, non-palettized mode
			0,								// no alpha buffer
			0,								// ignore shift bit
			0,								// no accumulation buffer
			0, 0, 0, 0,						// ignore accumulation bits
			16,								// 16 bit z-buffer size
			(StencilBufferEnabled?8:0),		// 8 allows it, 0 prevents it.  See Ch 12
			0,								// no auxiliary buffer
			PFD_MAIN_PLANE,					// main drawing plane
			0,								// reserved
			0, 0, 0 };						// layer masks ignored

		nPixelFormat = ChoosePixelFormat(hDC, &pfd);	// choose best matching pixel format
		if (! nPixelFormat) throw Window::CantInitialize ();

		if (! SetPixelFormat(hDC, nPixelFormat, &pfd))// set pixel format to device context
			throw Window::CantInitialize ();
	}

	void Window::setupPalette()
	{
		int pixelFormat = GetPixelFormat(hDC ());
		PIXELFORMATDESCRIPTOR pfd;
		LOGPALETTE* pPal;
		int paletteSize;

		int redMask, greenMask, blueMask;
		int i;

		DescribePixelFormat(hDC (), pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

		if (pfd.dwFlags & PFD_NEED_PALETTE)
			paletteSize = 1 << pfd.cColorBits;
		else
			return;

		pPal = (LOGPALETTE*)LocalAlloc(LPTR, sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
		pPal->palVersion = 0x300;
		pPal->palNumEntries = (short)paletteSize;

		//	build a simple RGB color palette
		redMask   = (1 << pfd.cRedBits)   - 1;
		greenMask = (1 << pfd.cGreenBits) - 1;
		blueMask  = (1 << pfd.cBlueBits)  - 1;

		for (i=0; i<paletteSize; ++i)
		{	pPal->palPalEntry[i].peRed = (BYTE)(
				(((i >> pfd.cRedShift) & redMask) * 255) / redMask);
			pPal->palPalEntry[i].peGreen = (BYTE)(
				(((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask);
			pPal->palPalEntry[i].peBlue = (BYTE)(
				(((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask);
			pPal->palPalEntry[i].peFlags = 0;
		}

		hPalette_ = CreatePalette(pPal);
		LocalFree(pPal);

		if (hPalette_)
		{
			DeleteObject(SelectPalette(hDC (), hPalette_, FALSE));
			RealizePalette(hDC ());
		}
	}

	void Window::makePaletteChanged(WPARAM wParam)
	{
		if (hRC_ && hPalette_ && (HWND)wParam != hWND_)
		{	
			wglMakeCurrent(hDC (), hRC_);
			UnrealizeObject(hPalette_);
			SelectPalette(hDC (), hPalette_, FALSE);
			RealizePalette(hDC ());
		}
	}

	BOOL Window::queryNewPalette()
	{
		if (hRC_ && hPalette_)
		{	
			wglMakeCurrent(hDC (), hRC_);
			UnrealizeObject(hPalette_);
			SelectPalette(hDC (), hPalette_, FALSE);
			RealizePalette(hDC ());

			return TRUE;
		}
		return FALSE;
	}

	void Window::paint() { PAINTSTRUCT ps; BeginPaint(hWND_, &ps); EndPaint(hWND_, &ps); }

	void Window::createContexts (HWND hwnd, bool StencilBufferEnabled)
	{
		hWND_ = hwnd;

		hDC_ = GetDC(hwnd);			// get current window's device context

		SetupPixelFormat(hDC_, StencilBufferEnabled);
		setupPalette();

		// create rendering context and make it current
		if(! (hRC_ = wglCreateContext(hDC_)))
			throw CantInitialize ();

		if(! wglMakeCurrent(hDC_, hRC_))
			throw CantInitialize ();

		OnCreate();
	}

	void Window::resize (int w, int h)
	{
		if (h==0) h = 1;				// don't want a divide by zero					

		width_ = w; height_ = h;

		if (width () > height ()) aspect_ = width  ();
		else					  aspect_ = height ();

		if (hRC_) //but what if it's not?  Is that possible?  WSB
		{
			glViewport(0, 0, width(), height());		// reset the viewport to new dimensions
			glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
			glLoadIdentity();						// reset projection matrix

			gluPerspective(54.0f,(GLfloat)width()/(GLfloat)height(),0.1f,1000.0f); //Why these numbers?  WSB

			OnSize();

			glMatrixMode(GL_MODELVIEW);				// set modelview matrix
			glLoadIdentity();						// reset modelview matrix
		}
	}

	void Window::resizeOrtho (int width, int height) //Just like resize, except
												//it does orthographic projection
	{
		if (height==0)					// don't want a divide by zero
			height=1;					

		glViewport(0, 0, width, height);	// reset the viewport to new dimensions
		glMatrixMode(GL_PROJECTION);		// set projection matrix current matrix
		glLoadIdentity();					// reset projection matrix

		glOrtho(0.0f, width - 1.0, 0.0, height - 1.0, -1.0, 1.0); 

		glMatrixMode(GL_MODELVIEW);			// set modelview matrix
		glLoadIdentity();					// reset modelview matrix
		width_ = width; height_ = height;
	}

	void Window::resizeOrtho2D(int width, int height)
	{
		if (height==0)	height=1;			// don't want a divide by zero

		glViewport(0, 0, width, height);	// reset the viewport to new dimensions
		glMatrixMode(GL_PROJECTION);		// set projection matrix current matrix
		glLoadIdentity();					// reset projection matrix

		gluOrtho2D(0, width, 0, height);

		glMatrixMode(GL_MODELVIEW);			// set modelview matrix
		glLoadIdentity();					// reset modelview matrix
		width_ = width; height_ = height;
	} 

	// go to full screen widthXheight resolution
	//	    and save original state
	void Window::beginFullScreen() 
	{
		// Save current display state
		//iPrevWidth = GetSystemMetrics(SM_CXSCREEN);
		//iPrevHeight = GetSystemMetrics(SM_CYSCREEN);
	
		// Set new display state
		DEVMODE dm;
		memset(&dm, 0, sizeof(dm));
		dm.dmSize	= sizeof(DEVMODE);
		dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		dm.dmPelsWidth	= width ();
		dm.dmPelsHeight = height();
		dm.dmBitsPerPel = bits  ();
		ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
	}

	// return to desktop at original resolution
	void Window::endFullScreen() { ChangeDisplaySettings(NULL, 0); }

	void Window::close ()
	{
		if (hDC() == NULL) throw Uninitialized ();

		// deselect rendering context and delete it
		//wglMakeCurrent(hDC_, NULL);  //This causes a crash!
		//wglDeleteContext(hRC_);		//So does this!

		// send WM_QUIT to message queue
		PostQuitMessage(0);
	}

	/* Chapters 4, 5 (projections) and 15 have this in a KillWindow
	   function that is called at the end of WinMain.  Good idea?
	   Could also be called in Window::close

	   if (hRC_)
	  {
		// release the RC
		if (!wglMakeCurrent(NULL,NULL))
		{
		  MessageBox(NULL, "Unable to release rendering context", "Error", MB_OK | MB_ICONINFORMATION);
		}

		// delete the RC
		if (!wglDeleteContext(hRC_))
		{
		  MessageBox(NULL, "Unable to delete rendering context", "Error", MB_OK | MB_ICONINFORMATION);
		}

		hRC_ = NULL;
	  }

	  // release the DC if we have one
	  if (hDC_ && !ReleaseDC(g_hwnd, hDC_))
	  {
		MessageBox(NULL, "Unable to release device context", "Error", MB_OK | MB_ICONINFORMATION);
		hDC_ = NULL;
	  }

	  // destroy the window if we have a valid handle
	  if (g_hwnd && !DestroyWindow(g_hwnd))
	  {
		MessageBox(NULL, "Unable to destroy window", "Error", MB_OK | MB_ICONINFORMATION);
		g_hwnd = NULL;
	  }

	  // unregister our class so we can create a new one if we need to
	  if (!UnregisterClass(WND_CLASS_NAME, g_hInstance))
	  {
		MessageBox(NULL, "Unable to unregister window class", "Error", MB_OK | MB_ICONINFORMATION);
		g_hInstance = NULL;
	  }

	  */
	HWND Window::initialize (HINSTANCE hInstance, int w, int h, 
							 int x, int y,
							 int bitsPerPixel,
							 bool fullScreen,
							 bool StencilBufferEnabled)
	{
		RECT	   windowRect;
		DWORD	   dwExStyle;						// Window Extended Style
		DWORD	   dwStyle;						// Window Style

		isFullScreen_	= fullScreen;
		isActive_	= true;
		width_		  = w;
		height_		  = h; //WSB: shouldn't I change this to 1 if it's a 0?
		bits_		  = bitsPerPixel;
		hInstance_	= hInstance;

		Register (hInstance); 

		windowRect.left=  (long)x;						// Set Left Value To 0
		windowRect.right= (long)(width()+x);						// Set Right Value To Requested Width
		windowRect.top=   (long)y;							// Set Top Value To 0
		windowRect.bottom=(long)(height()+y);						// Set Bottom Value To Requested Height

		if (isFullScreen_)
		{
			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;
		}
		else
		{
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPEDWINDOW;
		}

		AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);
	
		// create the window
		if (isFullScreen_)
		{
			beginFullScreen(); 
			hWND_ = CreateWindowEx(NULL, "Engine", name_.c_str(), dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
							  0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
							  NULL, NULL, (HINSTANCE)GetModuleHandle(NULL), (void*)this);
		}
		else
		{

			hWND_ = CreateWindowEx(NULL, "Engine", name_.c_str(), dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
							  0, 0, width (), height (), NULL, NULL, (HINSTANCE)GetModuleHandle(NULL),(void*)this);
		}

		// check if window creation failed (hwnd would equal NULL)
		if (!hWND_)
			throw CantInitialize ();

		createContexts (hWND_, StencilBufferEnabled);

		ShowWindow(hWND_, SW_SHOW);			// display the window
		UpdateWindow(hWND_);					// update the window

		SetForegroundWindow(hWND_);
		SetCapture(hWND_);
		SetFocus(hWND_);

		ShowCursor(FALSE);

		return hWND_;
	}

	void Window::destruct()
	{
		if (hRC_)
		{	
			wglMakeCurrent(hDC (), NULL);
			wglDeleteContext(hRC_);
			hRC_ = NULL; 
		}
		if (hPalette_)
		{	
			DeleteObject(hPalette_);
		}

		// Should I add this? 
		// release the DC if we have one
		// if (hDC_ && !ReleaseDC(g_hwnd, hDC_))
		// {
		//MessageBox(NULL, "Unable to release device context", "Error", MB_OK | MB_ICONINFORMATION);
		//hDC_ = NULL;
		// }
	  
		SetWindowLong(hWND_, GWL_USERDATA, (LONG)NULL);

		if (hWND_) DestroyWindow(hWND_); 

		//Should I add this? Consider later
		// // unregister our class so we can create a new one if we need to
		// if (!UnregisterClass(WND_CLASS_NAME, g_hInstance))
		// {
		//MessageBox(NULL, "Unable to unregister window class", "Error", MB_OK | MB_ICONINFORMATION);
		//g_hInstance = NULL;
		// }

		// if we're in fullscreen, get out of it
		endFullScreen();
	}

	Window::~Window()
	{
		if (hRC_)				destruct();
		if (isFullScreen ())	endFullScreen();
		ShowCursor(TRUE);
	}


	void InitializeWorld()			//This function is for OpenGL, not making a window
									//There's nothing special; it's just some stuff that
									//commonly needs doing
	{
		glClearColorv(BLACK);		// clear to black

		glShadeModel(GL_SMOOTH);					// use smooth shading
		glEnable(GL_DEPTH_TEST);					// hidden surface removal
		glEnable(GL_CULL_FACE);						// do not calculate inside of poly's
		glFrontFace(GL_CCW);						// counter clock-wise polygons are out
	}
} //end namespace lcgl