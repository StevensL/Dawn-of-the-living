#ifndef __GUI_H
#define __GUI_H

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

/*
	
	GUI.H
	The GUI class interface
	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: 4/2/2001
	Description:

*/

#include <windows.h>
#include "billboardfont.h"
#include "player.h"
#include <timer.h>


using namespace lcgl;

class GUI
{
private:
	int minutesLeft, secondsLeft, millisecondsLeft;
	int enemiesLeft;
	float x, y, z;

	BillboardFont font, endText, fps,healthIcon;
	Player player_;
	Timer timer_;

	Texture2D crosshair_, _icon;

public:
	GUI();
	GUI (const GUI&);
	void operator= (const GUI&);

	void SetCurrentTime	(float timeLeft);
	void SetEnemiesLeft	(int eLeft)			{	enemiesLeft = eLeft;	}
	void Draw			();
	void Animate		(float deltaTime);

	void DrawWinner		();
	void DrawLoser		();
	void drawCrosshair  ()const;
	void drawAcidIcon   ()const;
	
};


#endif