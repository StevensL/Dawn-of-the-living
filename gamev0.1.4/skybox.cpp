/*********************************************************************
	skybox.h

	The Skybox class.  Use it to make a sky box that seems to be at 
	 infinite distance, because it moves with you
	 adapted by Michael Carey
**********************************************************************/

#include <windows.h>     
#include <gl\gl.h>       
#include <gl\glu.h>  
#include <sstream>
#include "skybox.h"

using namespace lcgl;

Skybox::Skybox (const char* directory)
{	
	static const char* faceNames[FACES] 
		= { "Front.bmp", "Left.bmp", "Back.bmp", "Right.bmp", "Top.bmp"};

	for (int i = 0; i < FACES; ++i)
	{
		std::stringstream filename;
		filename << directory << "/" << faceNames[i];
		_faces[i].load (filename.str().c_str(), GL_LINEAR);
	}
}
	
void Skybox::render(const worldCoord& pov) const
{	
	// Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_BORDER); 

    // Just in case we set all vertices to white.
    glColor4fv(WHITE);
	
	glPushMatrix();
		glTranslatefv(pov);
		
		_faces[4].activate ();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBegin(GL_QUADS);	// top face
			glTexCoord2fv(LOWER_LEFT); glVertex3f(-0.5f, 0.5f, 0.5f);	
			glTexCoord2fv(LOWER_RIGHT); glVertex3f(0.5f, 0.5f, 0.5f);
			glTexCoord2fv(UPPER_RIGHT); glVertex3f(0.5f, 0.5f, -0.5f);
			glTexCoord2fv(UPPER_LEFT ); glVertex3f(-0.5f, 0.5f, -0.5f);
		glEnd();
	
		_faces[0].activate ();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBegin(GL_QUADS);	// front face
			glTexCoord2fv(LOWER_LEFT ); glVertex3f(0.5f, -0.5f, 0.5f);
			glTexCoord2fv(LOWER_RIGHT); glVertex3f(0.5f, 0.5f, 0.5f);	
			glTexCoord2fv(UPPER_RIGHT); glVertex3f(-0.5f, 0.5f, 0.5f);
			glTexCoord2fv(UPPER_LEFT ); glVertex3f(-0.5f, -0.5f, 0.5f);
		glEnd();

		_faces[3].activate ();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBegin(GL_QUADS);	// right face
			glTexCoord2fv(LOWER_LEFT ); glVertex3f(0.5f, 0.5f, -0.5f);
			glTexCoord2fv(LOWER_RIGHT); glVertex3f(0.5f, 0.5f, 0.5f);	
			glTexCoord2fv(UPPER_RIGHT); glVertex3f(0.5f, -0.5f, 0.5f);
			glTexCoord2fv(UPPER_LEFT ); glVertex3f(0.5f, -0.5f, -0.5f);
		glEnd();

		_faces[1].activate ();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBegin(GL_QUADS);	// left face
			glTexCoord2fv(LOWER_LEFT ); glVertex3f(-0.5f, -0.5f, 0.5f);
			glTexCoord2fv(LOWER_RIGHT); glVertex3f(-0.5f, 0.5f, 0.5f);	
			glTexCoord2fv(UPPER_RIGHT); glVertex3f(-0.5f, 0.5f, -0.5f); 
			glTexCoord2fv(UPPER_LEFT ); glVertex3f(-0.5f, -0.5f, -0.5f);
		glEnd();
		
		_faces[2].activate ();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBegin(GL_QUADS);	// back face
			glTexCoord2fv(LOWER_LEFT ); glVertex3f(0.5f, 0.5f, -0.5f);
			glTexCoord2fv(LOWER_RIGHT); glVertex3f(0.5f, -0.5f, -0.5f);
			glTexCoord2fv(UPPER_RIGHT); glVertex3f(-0.5f, -0.5f, -0.5f);
			glTexCoord2fv(UPPER_LEFT ); glVertex3f(-0.5f, 0.5f, -0.5f);	
		glEnd();
		
    glPopMatrix();
	glPopAttrib();

	
};

