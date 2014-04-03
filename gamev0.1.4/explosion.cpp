/****************************************************************************
 Particles.h  Particle and particle system base classes.
 
 Author   :   Dave Astle
 Date     :   2/1/2001

 Written for OpenGL Game Programming
*****************************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "explosion.h"

using namespace lcgl;

Explosion::Explosion(int numParticles, const worldCoord& origin, float spread, 
					   Texture2D& newTexture)
  : texture(&newTexture), m_spread(spread), ParticleSystem(numParticles, origin)
{
  ParticleSystem::InitializeSystem();
  Emit(numParticles);
} // end Explosion::Constructor


void Explosion::InitializeParticle(int index)
{
  // start the particle at the sky at a random location in the emission zone
  particleList[index].pos.x() = origin.x() + FRAND * m_spread;
  particleList[index].pos.y() = origin.y() + FRAND * m_spread;
  particleList[index].pos.z() = origin.z() + FRAND * m_spread;

  // set the size of the particle
  particleList[index].size = PARTICLE_SIZE + FRAND * SIZE_VARIATION;

  // give the particle a random velocity
  particleList[index].velocity.x() = PARTICLE_VELOCITY.x() + FRAND * VELOCITY_VARIATION.x();
  particleList[index].velocity.y() = PARTICLE_VELOCITY.y() + FRAND * VELOCITY_VARIATION.y();
  particleList[index].velocity.z() = PARTICLE_VELOCITY.z() + FRAND * VELOCITY_VARIATION.z();

  particleList[index].acceleration = PARTICLE_ACCELERATION;

  particleList[index].color[0] = 1.0;
  particleList[index].color[1] = 0.5f + FRAND * 0.5f;
  particleList[index].color[2] = 0.01f;
  particleList[index].color[3] = 1.0;

  particleList[index].energy = 1.5f + FRAND/2.0f;

  particleList[index].colorDelta[0] = 0.0;
  particleList[index].colorDelta[1] = -(particleList[index].color[1]/2.0f)/particleList[index].energy;
  particleList[index].colorDelta[2] = 0.0;
  particleList[index].colorDelta[3] = -1.0f/particleList[index].energy;

  particleList[index].sizeDelta = -particleList[index].size/particleList[index].energy;

} 

void Explosion::Update(float elapsedTime)
{
  for (int i = 0; i < numParticles; )
  {
    // update the particle's position based on the elapsed time and velocity
    particleList[i].pos 
			= particleList[i].pos + particleList[i].velocity * elapsedTime;
    particleList[i].velocity 
			= particleList[i].velocity + particleList[i].acceleration * elapsedTime;

    particleList[i].energy -= elapsedTime;
    particleList[i].size += particleList[i].sizeDelta * elapsedTime;

    particleList[i].color[3] += particleList[i].colorDelta[3] * elapsedTime;
    particleList[i].color[1] += particleList[i].colorDelta[1] * elapsedTime;

    // if the particle has hit the ground plane, kill it
    if (particleList[i].energy <= 0.0)
      // move the last particle to the current positon, and decrease the count
      particleList[i] = particleList[--numParticles];
    else ++i;
  }
} 

void Explosion::Render()
{
  float viewMatrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);

  worldCoord right(viewMatrix[0], viewMatrix[4], viewMatrix[8]);
  worldCoord up   (viewMatrix[1], viewMatrix[5], viewMatrix[9]);

  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

  glEnable(GL_TEXTURE_2D);
  texture->activate();
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  GLfloat size;
  worldCoord pos;
  glBegin(GL_QUADS);
  for (int i = 0; i < numParticles; ++i)
  {
    size = particleList[i].size/2;
    pos = particleList[i].pos;
    //glColor4fv(particleList[i].color);
	glColor4fv(GREEN);
    glTexCoord2fv(LOWER_LEFT ); glVertex3fv((pos + (right + up) * -size));
    glTexCoord2fv(LOWER_RIGHT); glVertex3fv((pos + (right - up) * size));
    glTexCoord2fv(UPPER_RIGHT); glVertex3fv((pos + (right + up) * size));
    glTexCoord2fv(UPPER_LEFT ); glVertex3fv((pos + (up - right) * size));
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
}
