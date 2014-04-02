#ifndef __TERRAIN_H
#define __TERRAIN_H

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN


/*
	TERRAIN.H

	
				The Terrain class is derived from Object, and represents
			   the main world. Randomly generated terrain and frustum clipping
			   are provided. 

			   The random terrain is created with the midpoint
			   displacement algorithm. 

			   edited by: Michael Carey
*/

#include "color.h"
#include "texture.h"
#include "fog.h"
#include "object.h"
#include "EnviroObj.h"
#include<vector>


#define MAP_X       10  // size of map along x-axis
#define MAP_Z       10      // size of map along z-axis



using namespace lcgl;

class Player;

class Terrain : public Object
{
private:
	int width;							// terrain is of size width X width
										// preferably with 2^n = width
	float terrainMul;
	float heightMul;
	float scanDepth;
	float textureMul;
	float _edgeWidth;

	lcgl::Texture2D cactus_;			//"EnviroObj" variables
	lcgl::Texture2D tree_;
	lcgl::Texture2D building_;
	lcgl::Texture2D car_;
	lcgl::Texture2D pCactus_;
	lcgl::Texture2D rock_;
	lcgl::Texture2D yea;
	
	EnviroObj envCacti_;
	EnviroObj envGrass_;
	EnviroObj envHouse_;
	EnviroObj envCar_;
	EnviroObj envPcactus_;
	EnviroObj envRock_;
	
	std::vector<worldCoord> locations_;//possible positions stored here 

	float RangedRandom(float v1,float v2);
	void NormalizeTerrain(float field[],int size);
	void FilterHeightBand(float *band,int stride,int count,float filter);
	void FilterHeightField(float field[],int size,float filter);
	void MakeTerrainPlasma(float field[],int size,float rough);
	void BuildTerrain(int width, float rFactor);

protected:

	

	float*		heightMap;		// dynamic heightmap
	Texture2D	terrainTex[5];	// for multiple textures on the terrain
								//but ti seems we only use the one
public:
	Color		fogColor;		// color of the fog/sky

protected:
	Fog			myFog;			//ok, this is strange:  I must declare
								//fog color before fog, even though I
								//initialize them in the right order.
public:
	
	Terrain (int width, float rFactor);
	Terrain (const Terrain& other) : Object (other) {} //throws exception
	void operator= (const Terrain& other) { Object::operator= (other); } //throws exception
	~Terrain() { delete [] heightMap; }

	void Load	() {}
	void Unload	() {}

	// terrain doesn't move, so no physics animations
	void OnAnimate(float deltaTime) {}
	
	 
	void Populate(int heightOffGround,     std::vector<worldCoord>& input);//array of possible locations
	
	void OnDraw(const Player& player);
	void OnCollision(Object *collisionObject);

	float GetHeight		(double x, double z);
	 float GetHeightH (float x, float z) const;


	float GetWidth		() { return float(width);	}
	float GetMul		() { return terrainMul;		}
	float GetScanDepth	() { return scanDepth;		}
	float edgeWidth     () const { return _edgeWidth; }
	

	inline worldCoord& getLocations(int index){ return locations_[index]; }					//for getting a location out of the locations vector
	inline const worldCoord& getLocations(int index) const { return locations_[index]; }	//can be called on a const obj
	inline void clearLocations(){  locations_.clear();}										//adds fps by releasing vector memory


	GLuint  g_indexArray[MAP_X * MAP_Z * 2];    // vertex array

};

#endif