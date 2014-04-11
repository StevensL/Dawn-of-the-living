/*******************************
Gui Class 
adapted by: Michael && l_wood


********************************/

#include <math.h>
#include "gui.h"
#include <timer.h>//for use of fps counter mac


using namespace lcgl;

GUI::GUI() :	font		("Arial",	25), 
				endText		("Arial",	40),
				fps         ("Arial",	25),
				healthIcon  ("Arial",	80),
				health      ("Arial",   40)
			

{
	timer_.Init();//initialize timer (debug for FPS display) mac 3/15/14 

	minutesLeft = secondsLeft = enemiesLeft = 0;
	crosshair_.load ("bill/greenbiocross.bmp", GL_NEAREST, SOIL_LOAD_RGBA, true, BLACK);  //make Black transparent
	_icon.load ("bill/acidicon.bmp", GL_NEAREST, SOIL_LOAD_RGBA, true, BLACK);

}

void GUI::SetCurrentTime(float timeLeft)	// timeLeft is in seconds
{
	
	minutesLeft = (int)(timeLeft / 60.0);	// 60 seconds in 1 minute
	secondsLeft = (int)floor(timeLeft) % 60;
	millisecondsLeft = (int)(timeLeft - (float)floor(timeLeft)) * 60;
}

void GUI::DrawWinner()
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4fv(Color(0.3f, 1.0f, 0.3f, 0.8f));
	print(endText, twoDCoord<float> (-0.25f, 0.15f), "YOU WIN!");

	glColor4fv(Color(1.0f, 1.0f, 1.0f, 0.8f));
	print(endText, twoDCoord<float> (-0.5f, 0.2f), "Press the ESC key to exit");
	
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}

void GUI::DrawLoser()
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4fv(Color(1.0f, 0.3f, 0.3f, 0.8f));
	print(endText, twoDCoord<float> (-0.15f, 0.15f), "YOU LOSE!");

	glColor4fv(Color(1.0f, 1.0f, 1.0f, 0.8f));
	print(endText, twoDCoord<float> (-0.4f, -0.2f), "Press the ESC key to exit");
	
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}

void GUI::Draw()
{
	
	glDisable(GL_TEXTURE_2D);

	twoDCoord<float> TimePos (0.4f, 0.48f);

	
	glColor4fv(GREEN);
	if (secondsLeft < 10)
	{
		if (millisecondsLeft < 10)
			print(font, TimePos, "Time: %d:0%d.0%d", minutesLeft, secondsLeft, millisecondsLeft);
		else
			print(font, TimePos, "Time: %d:0%d.%d", minutesLeft, secondsLeft, millisecondsLeft);
	}
	else
	{
		if (millisecondsLeft < 10)
			print(font, TimePos, "Time: %d:%d.0%d", minutesLeft, secondsLeft, millisecondsLeft);
		else
			print(font, TimePos, "Time: %d:%d.%d", minutesLeft, secondsLeft, millisecondsLeft);
	}

	print(font, twoDCoord<float> (0.4f, 0.44f), "Enemies: %d", enemiesLeft);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawCrosshair();//draw the cross hair here mac 3/26/14
	
	drawAcidIcon();

	//debug fps (comment out if you do not want this displayed) mac 3/15/14
	//******************************DEBUG***************************************
	glColor4fv(GREEN);
	print(fps, twoDCoord<float>(-0.65f,0.44f) ,"Fps: %4.0f", timer_.GetFPS(1) );
	//********************************FPS***************************************


	print(healthIcon,twoDCoord<float>(-0.7f,-0.5f),"+");//draw a plus symbol for the health bar
	print(health, twoDCoord<float> (-0.6f,-.48f),"health: %d"  ,player_.getHealth()); //print out the player's health 

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}

void GUI::drawCrosshair()const //used to draw the cross hair mac 4/5/14
{

	const int WINDOW_HI = 800, WINDOW_WID = 600;//windows params

	glEnable(GL_TEXTURE_2D);  //make sure we can render the texture
	crosshair_.activate();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0.0, WINDOW_HI, WINDOW_WID, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);

	glColor3fv(WHITE);
	glBegin(GL_QUADS);

		glTexCoord2f(0.0,0.0); glVertex2f(425.0, 320.0);
		glTexCoord2f(1.0,0.0); glVertex2f(390.0, 320.0);
		glTexCoord2f(1.0,1.0); glVertex2f(390.0, 280.0);
		glTexCoord2f(0.0,1.0); glVertex2f(425.0, 280.0);

	glEnd();

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D); //disable texture so we do not mess up our scene


}
void GUI::drawAcidIcon()const //draws the icon for spitting acid mac 4/5/14
{
	
	const int WINDOW_HI = 800, WINDOW_WID = 600;//window parameters

	glEnable(GL_TEXTURE_2D);  //make sure we can render the texture
	_icon.activate();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0.0, WINDOW_HI, WINDOW_WID, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);

	glColor3fv(WHITE);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0,0.0); glVertex2f(725.0, 585.0);
	glTexCoord2f(1.0,0.0); glVertex2f(690.0, 585.0);
	glTexCoord2f(1.0,1.0); glVertex2f(690.0, 545.0);
	glTexCoord2f(0.0,1.0); glVertex2f(725.0, 545.0);

	glEnd();

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D); //disable texture so we do not mess up our scene

	

}