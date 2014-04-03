/****************************************************************************
 Particles.h  Particle and particle system base classes.
 Sandstorm class to produce a sand blowing effect

 Written by:
 Michael Carey
 
*****************************************************************************/

#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "coord.h"
#include "texture.h"
#include "SandStorm.h"
#include <ctime>


const worldCoord SAND_P_VELOCITY  (-5.0f, -6.0f, 1.0f);
const float SAND_GRAIN_SIZE = 0.025f;

using namespace lcgl;

Sandstorm::Sandstorm(int numParticles, const worldCoord& origin, float height_, float width_, float depth_)
  : height(height_), width(width_), depth(depth_), ParticleSystem(numParticles, origin)
{
} 
Sandstorm::~Sandstorm()
{
	KillSystem();
}
void Sandstorm::InitializeParticle(int index)
{
	  // start the particle in emission zone
	  particleList[index].pos.x() = width + 0.05f +Frand(2.0f)* width;

	  particleList[index].pos.y() = height + 20 + Frand();

	  particleList[index].pos.z() = origin.z() +Frand();

	  particleList[index].size = SAND_GRAIN_SIZE; // set the size of the particle

	  // give the particle velocity
	  particleList[index].velocity.x() = SAND_P_VELOCITY.x() * Frand() * VELOCITY_VARIATION.x();

	  particleList[index].velocity.y() = SAND_P_VELOCITY.y() * VELOCITY_VARIATION.y(); 
		
	  particleList[index].velocity.z() = SAND_P_VELOCITY.z() * Frand(5);
 
} 

 //Update the existing particles, killing them and creating new ones as needed
void Sandstorm::Update(float elapsedTime)
{
     
  for (int i = 0; i < numParticles; )
  {
	  // update the particle's position based on the elapsed time and velocity
 	  //particleList[i].pos += particleList[i].velocity * elapsedTime;
	 
	   particleList[i].pos.x() 
			= particleList[i].pos.x() + particleList[i].velocity.x() * elapsedTime;
	   particleList[i].pos.y() 
			= particleList[i].pos.y() + particleList[i].velocity.y() * elapsedTime;
	   particleList[i].pos.z()
			= particleList[i].pos.z() + particleList[i].velocity.z() * elapsedTime;

		// if the particle has hit the ground plane, kill it
		if (particleList[i].pos.y() <= origin.y())
		  // move the last particle to the current position, and decrease the count
		  particleList[i] = particleList[--numParticles];
		else ++i;
  }

  accumulatedTime += elapsedTime;

 float newParticlesNeeded = GRAINS_PER_SEC * accumulatedTime;

  // save the remaining time for after the new particles are released.
  accumulatedTime -= 1.0f/(float)GRAINS_PER_SEC * newParticlesNeeded;

  Emit(newParticlesNeeded);
} 

void Sandstorm::Render()
{
  // enable alpha blending and texturing
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);

  // set the blend mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);


  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // to avoid constant dereferencing...
  worldCoord partPos;
  float size;
  sandTex.activate();
  // draw the quads
  glColor4fv(WHITE);
  glBegin(GL_QUADS);

	  for (int i = 0; i < numParticles; ++i)
	  {
			partPos = particleList[i].pos;
			size = particleList[i].size;
			glTexCoord2fv(UPPER_LEFT );  glVertex3fv(partPos);
			glTexCoord2fv(UPPER_RIGHT);
			glVertex3f(partPos.x() + size,partPos.y(),partPos.z());
			glTexCoord2fv(LOWER_RIGHT);
			glVertex3f(partPos.x() + size, partPos.y() - size, partPos.z());
			glTexCoord2fv(LOWER_LEFT);
			glVertex3f(partPos.x(),	partPos.y() - size,	partPos.z());
	  }

  glEnd();

  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
} 

void Sandstorm::InitializeSystem()
{
	sandTex.load ("bill/sandparticle.bmp", GL_NEAREST, SOIL_LOAD_RGBA, true, BLACK);
	
	// parent does the remaining initialization
	ParticleSystem::InitializeSystem();
} 
float Sandstorm::Frand(int X)
{ 
	float floatRandom = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/X));
	return floatRandom; 

} 
