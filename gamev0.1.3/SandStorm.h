/****************************************************************************
// represents a sand storm

//created by Michael Carey
*****************************************************************************/
#include "Particles.h"
#include "texture.h"
#include"FRAND.h"


extern const lcgl::worldCoord SAND_P_VELOCITY;
extern const lcgl::worldCoord VELOCITY_VARIATION;
extern const float SAND_GRAIN_SIZE;


using namespace lcgl;

class Sandstorm : public ParticleSystem
{
	enum{GRAINS_PER_SEC = 800};
	
public:
	Sandstorm(int maxParticles, const worldCoord& origin, 
			float height, float width, float depth);
	~Sandstorm();

	void  Update(float elapsedTime);
	void  Render();

	static float Frand(int x);
	inline static float Frand(){return FRAND;};

	void  InitializeSystem();

protected:
	void    InitializeParticle(int index);

private:

	float   height;
	float   width;
	float   depth;

	lcgl::Texture2D sandTex;//actual sand texture

};


