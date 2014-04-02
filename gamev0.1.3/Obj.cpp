/*********************************************
EnviroObj.cpp

used to add billboarded items into the world
can add as many as the particular world can hold
it is recommended to use print the world coords for debugging  

created by Michael Carey

********************************************/

#include "terrain.h"
#include<string>
#include "object.h"
#include <vector>
#include "worldCoord.h"
#include"color.h"
#include "texture.h"
#include "EnviroObj.h"
#include <vector>


using namespace lcgl;

EnviroObj::EnviroObj()
{
}
EnviroObj::EnviroObj( Texture2D& enviroObj, const char* string):enviroObj_(enviroObj),
size_(DEFAULT_SIZE)
{
	
	fileLocation_ = new char[MAX_CHARS];//dynamically allocate the file's location

	for (int i=0;i <strlen(string);i++)
	{
		fileLocation_[i] = string[i];
	}
	fileLocation_[strlen(string)] = '\0';//add null terminator to file location

	
	enviroObj_.load (fileLocation_, GL_NEAREST, SOIL_LOAD_RGBA, true, BLACK);  //make Black transparent
	enviroObj_.repeat(); 

}

EnviroObj::~EnviroObj(void)
{
	delete [] fileLocation_;	//deallocate the file location
}

void EnviroObj::render(const worldCoord& pos)//render a billboarded object in a specified location
{
	//for billboards that are not square
	static const float ASPECT_RATIO = float(enviroObj_.height())/enviroObj_.width ();

	glEnable(GL_TEXTURE_2D);//enable the 2d texture
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	
	// make sure the transparent part of the texture isn't drawn
	
	float viewMatrix[16];

	
	glGetFloatv(GL_MODELVIEW_MATRIX , viewMatrix);// save the current model view matrix

	// get the right and up vectors
	worldCoord right(viewMatrix[0], viewMatrix[4], viewMatrix[8]); right /= ASPECT_RATIO;
	worldCoord  up(viewMatrix[1], viewMatrix[5], viewMatrix[9]);

	
	
	enviroObj_.activate (); // select the texture

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


	glPushMatrix();
		glBegin(GL_QUADS);//draw and texture map a square

		glTexCoord2f(0.0,0.0); glVertex3fv(pos + (right  + up) * -size_);
		glTexCoord2f(1.0,0.0); glVertex3fv(pos + (right / 2  - up) * size_);
		glTexCoord2f(1.0,1.0); glVertex3fv(pos + (right / 2  + up) * size_);
		glTexCoord2f(0.0,1.0); glVertex3fv(pos + (up - right) * size_);
				
		glEnd();
	glPopMatrix();


	glDisable(GL_ALPHA);  glDisable(GL_BLEND);

}

std::vector<worldCoord>& EnviroObj::getVector()
{ 
	return locations_;  
}