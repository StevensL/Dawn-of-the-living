/****************************************************************************

 
 Author   :   Dave Astle
 Date     :   3/17/2001

 Written for OpenGL Game Programming
*****************************************************************************/
#ifndef __EXPLOSION_H_INCLUDED__
#define __EXPLOSION_H_INCLUDED__

#include "Particles.h"
#include "FRAND.h"


using namespace lcgl;

const worldCoord PARTICLE_VELOCITY  (0.0f, 2.0f, 0.0f);
const worldCoord VELOCITY_VARIATION  (4.0f, 4.0f, 4.0f);
const worldCoord PARTICLE_ACCELERATION  (0.0f, -5.0f, 0.0f);
const float     PARTICLE_SIZE      = 5.0f;
const float     SIZE_VARIATION     = 2.0f;

class Explosion : public ParticleSystem
{
public:
  Explosion(int maxParticles, const worldCoord& origin, float spread, 
	  Texture2D& texture);
  
  Explosion (const Explosion&);
  void operator= (const Explosion&);

  void  Update(float elapsedTime);
  void  Render();

  bool  IsDead() { return numParticles == 0; }

protected:
  void    InitializeParticle(int index);
  float   m_spread;

  Texture2D*  texture;     // explosion texture
};



#endif // __EXPLOSION_H_INCLUDED__