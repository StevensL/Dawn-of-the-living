/******************************************************
create the terrain that the player explores 
also push billboarded objects to add realism to the map
mac 3/16/14

*******************************************************/

#include <stdlib.h>
#include <math.h>
#include "fog.h"
#include "terrain.h"
#include "player.h"
#include "lcglmath.h"
#include "skybox.h"
#include "EnviroObj.h"
#include<algorithm>
#include<ctime>
#include "light.h"


using namespace lcgl;

#define DEFAULT_TERRAIN_WIDTH  500
#define DEFAULT_R_FACTOR       0.04f
#define TERRAIN_MUL			   50.0f
#define TEXTURE_MUL			   0.25f
#define HEIGHT_MUL			   175.0f
#define SCAN_DEPTH			   70.0f
#define FOG_MIN_DEPTH          40.0f
#define FOG_MAX_DEPTH		   550.0f
#define EDGE_WIDTH             50.0f



Terrain::Terrain (int w=DEFAULT_TERRAIN_WIDTH, float rFactor=DEFAULT_R_FACTOR):

	Object (NULL, float(sqrt(2*square (width*TERRAIN_MUL)))), 
	fogColor (0.35,0.14,0.01), 
	myFog (0.02f,fogColor, GL_LINEAR), 
	envCacti_(cactus_,"bill/cacti.bmp"),
	envGrass_(tree_,"bill/deadgrass.bmp"),
	envHouse_(building_,"bill/building.bmp"),
	envCar_(car_,"bill/car.bmp")	,
	envPcactus_(pCactus_,"bill/prickle.bmp"),
	envRock_(rock_,"bill/smoothrock.bmp")//initialize billboard items in the constructor


{
	//Color(0.35,0.14,0.01); // fog color value its brown
	width = w;
	scanDepth = SCAN_DEPTH;   //how far in the distance you can see
	terrainMul = TERRAIN_MUL;
	textureMul = TEXTURE_MUL;
	heightMul = HEIGHT_MUL;
	_edgeWidth = EDGE_WIDTH;   //how close to edge of map you can get
	heightMap = NULL;
	BuildTerrain(width, rFactor);

}


	
void Terrain::BuildTerrain(int w, float rFactor)
{
	width = w;
	heightMap = new float[width*width];

	MakeTerrainPlasma(heightMap, width, rFactor);

	terrainTex[0].load("bill/cracked.bmp"); terrainTex[0].activate();	// load texture

	terrainTex[0].repeat();
	
	terrainTex[0].buildMipmaps ();
}

void Terrain::OnCollision(Object *collisionObject){}


void Terrain::Populate(int heightOffGround, vector<worldCoord>& input)//populates an array of world coords  for a particular object
{
	int x,z;
	

	for (z = (int)(position().z() / terrainMul - scanDepth), z = z< 50; 
		(z < position().z() / terrainMul + scanDepth) && z < width-1; 
		z++)
	{
		
		for (x = (int)(position().x() / terrainMul - scanDepth), x=x<50;
			(x < position().x() / terrainMul + scanDepth) && x < width-1; 
			x++)
		{
			
			input.push_back(worldCoord( (float)(x+1)*terrainMul+5,
				(float)heightMap[x+1 + z*width]*heightMul+heightOffGround,
				(float)z*terrainMul+5) );
		}
	
	}
}

void Terrain::OnDraw(const Player& p)
{
	
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.0);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);
	
	int z, x;

	// fog parameters here in order for them to work 
	myFog.enable();
	myFog.setDensity(0.0010);
	myFog.setMode(GL_LINEAR);
	myFog.setRange (FOG_MIN_DEPTH, FOG_MAX_DEPTH);		//setup fog min and max range
	myFog.setHint (GL_NICEST);

	
	/*******************************************************
	Add enviroObjs here 
	before the terrain is created because enviroObjs 
	utilize the existing coordinates  created during this process 
	only supports 3 objects at the moment
	for some reason rand() functions mess up the particle system
	mac 3/15/14
	*******************************************************/


	//draw patches of dried grass
	Populate(4, locations_ );//creates a list of legitimate locations for objects
	envGrass_.setSize(10);

	for ( int i=0;i < (signed)locations_.size ();i++)
	{
		envGrass_.setLocations(locations_[i]);//set the locations vector in envGrass_
	}


	for (int i=0;i < envGrass_.getVectorSize() ;i++)
	{
		envGrass_.render(envGrass_.getLocations(i) );// render all the grass procedurally  
	}
	
	envGrass_.clearLocations();		//clear out the locations vector increases fps
	clearLocations();				//clear all the vectors to free up unused locations increases fps

	//Draw Billboarded objects***********************************************************************
	//***********************************************************************************************
	//draw the building
	envHouse_.setSize(125);
	envHouse_.render(worldCoord(1414, 200, 1499));

	//render the cacti 
	//had to render this way because using rand() functions messes with 
	//the particle system mac 3/15/14

	envCacti_.setSize(20);
	envCacti_.render(worldCoord(1303, 146, 930) );
	envCacti_.render(worldCoord(1055, 106, 1224) );
	envCacti_.render(worldCoord(792, 35, 1103) );
	envCacti_.render(worldCoord(728, 32, 718) );
	envCacti_.render(worldCoord(223, 158, 340) );
	envCacti_.render(worldCoord(271, 155, 168) );
	envCacti_.render(worldCoord(938, 92, 413) );
	envCacti_.render(worldCoord(296, 96, 1139) );
	envCacti_.render(worldCoord(429, 158, 490) );
	envCacti_.render(worldCoord(570, 35, 1011) );
	envCacti_.render(worldCoord(1373, 139, 471) );


	//draw old cars

	envCar_.setSize(20);
	envCar_.render(worldCoord(1322, 127, 471) ) ;
	envCar_.render(worldCoord(689, 22, 913) ) ;

	//draw prickle cactus

	envPcactus_.setSize(8);
	envPcactus_.render(worldCoord(245, 158, 553) );
	envPcactus_.render(worldCoord(973, 55, 753) );
	envPcactus_.render(worldCoord(1020, 122, 1389) );
	envPcactus_.render(worldCoord(545, 62, 1215) );
	envPcactus_.render(worldCoord(780, 65, 382) );
	envPcactus_.render(worldCoord(1397, 91, 1184));

	//draw rocks

	envRock_.setSize(10);
	envRock_.render(worldCoord(311, 148, 276) );
	envRock_.render(worldCoord(804, 50, 531) );
	envRock_.render(worldCoord(920, 58, 989) );
	envRock_.render(worldCoord(172, 75, 1001) );
	envRock_.render(worldCoord(827, 68, 314) );
	envRock_.render(worldCoord(1099, 95, 1274) );
	

//*******************************************************************************************************
//*******************************************************************************************************


	terrainTex[0].activate();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glColor3fv(WHITE);
	for (z = (int)(p.position().z() / terrainMul - scanDepth), z=z<0?0:z; 
			(z < p.position().z() / terrainMul + scanDepth) && z < width-1; 
			z++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (x = (int)(p.position().x() / terrainMul - scanDepth), x=x<0?0:x;
				(x < p.position().x() / terrainMul + scanDepth) && x < width-1; 
				x++)
		{
			

		//	glColor3f(heightMap[x+z*width], heightMap[x+z*width], heightMap[x+z*width]);
			glTexCoord2f(textureMul * x, textureMul * z);
			glVertex3f((float)x*terrainMul, (float)heightMap[x + z*width]*heightMul, (float)z*terrainMul);

		//	glColor3f(heightMap[x+1+z*width], heightMap[x+1+z*width], heightMap[x+1+z*width]);
			glTexCoord2f(textureMul * (x+1), textureMul * z);
			glVertex3f((float)(x+1)*terrainMul, (float)heightMap[x+1 + z*width]*heightMul, (float)z*terrainMul);

		//	glColor3f(heightMap[x+(z+1)*width], heightMap[x+(z+1)*width], heightMap[x+(z+1)*width]);
			glTexCoord2f(textureMul * x, textureMul * (z+1));
			glVertex3f((float)x*terrainMul, (float)heightMap[x + (z+1)*width]*heightMul, (float)(z+1)*terrainMul);

		//	glColor3f(heightMap[x+1+(z+1)*width], heightMap[x+1+(z+1)*width], heightMap[x+1+(z+1)*width]);
			glTexCoord2f(textureMul * (x+1), textureMul * (z+1));
			glVertex3f((float)(x+1)*terrainMul, (float)heightMap[x+1 + (z+1)*width]*heightMul, (float)(z+1)*terrainMul);

			
		}
			
		glEnd();		
	}
}

// RangedRandom()
// Returns a random number between v1 and v2
float Terrain::RangedRandom(float v1,float v2)
{
	return v1 + (v2-v1)*((float)rand())/((float) RAND_MAX);
}

// NormalizeTerrain()
// Given a height field, normalize it so that the minimum altitude
// is 0.0 and the maximum altitude is 1.0
void Terrain::NormalizeTerrain(float field[],int size)
{
	float maxVal,minVal,dh;
	int i;

	/*
	Find the maximum and minimum values in the height field
	*/ 
	maxVal = field[0];
	minVal = field[0];

	for (i=1;i<size*size;i++)
	{
		if (field[i] > maxVal) 
		{
			maxVal = field[i];
		}
		else if (field[i] < minVal) 
		{
			minVal = field[i];
		}
	}

	/*
	Find the altitude range (dh)
	*/
	if (maxVal <= minVal) return;
	dh = maxVal-minVal;

	/*
	Scale all the values so they are in the range 0-1
	*/
	for (i=0;i<size*size;i++)
	{
		field[i] = (field[i]-minVal)/dh;
	}

}

// FilterHeightBand()
// Erosion filter -
// FilterHeightBand applies a FIR filter across a row or column of the height field
void Terrain::FilterHeightBand(float *band,int stride,int count,float filter)
{
	int i,j=stride;
	float v = band[0];
	for (i=0;i<count-1;i++)
	{
		band[j] = filter*v + (1-filter)*band[j];
		v = band[j];
		j+=stride;
	}
}


// FilterHeightField()
// Erosion filter -
// Erodes a terrain in all 4 directions
void Terrain::FilterHeightField(float field[],int size,float filter)
{
	int i;

	// Erode rows left to right
	for (i=0;i<size;i++)
	{
		FilterHeightBand(&field[size*i],1,size,filter);
	}
	
	// Erode rows right to left
	for (i=0;i<size;i++)
	{
		FilterHeightBand(&field[size*i+size-1],-1,size,filter);
	}

	// Erode columns top to bottom
	for (i=0;i<size;i++)
	{
		FilterHeightBand(&field[i],size,size,filter);
	}

	// Erode columns bottom to top
	for (i=0;i<size;i++)
	{
		FilterHeightBand(&field[size*(size-1)+i],-size,size,filter);
	}


}


//Generate terrain using diamond-square (plasma) algorithm(float field[],int size,float rough)
void Terrain::MakeTerrainPlasma(float field[],int size,float rough)
{
	

	

	int i,j,ni,nj,mi,mj,pmi,pmj,rectSize = size;
	float dh = (float)rectSize/2,r = (float)pow(2,-1*rough);

//	Since the terrain wraps, all 4 "corners" are represented by the value at 0,0,
	//seed srand(200) in game.cpp gives a "standard map 
	//instead of random each time the program starts mac 3/16/14


	

	field[0] = 0;


	while(rectSize > 0)
	{
		
		/*
		Diamond step -

		Find the values at the center of the retangles by averaging the values at 
		the corners and adding a random offset:


		a.....b
		.     .  
		.  e  .
		.     .
		c.....d   

		e  = (a+b+c+d)/4 + random

		In the code below:
		a = (i,j)
		b = (ni,j)
		c = (i,nj)
		d = (ni,nj)
		e = (mi,mj)

		*/
		       
		for (i=0;i<size;i+=rectSize)
		for (j=0;j<size;j+=rectSize)
		{
			
			ni = (i+rectSize)%size;
			nj = (j+rectSize)%size;

			mi = (i+rectSize/2);
			mj = (j+rectSize/2);

			field[mi+mj*size] = (field[i+j*size] + field[ni+j*size] + field[i+nj*size] + field[ni+nj*size])/4 + RangedRandom(-dh/2,dh/2);
		}

		/*
		Square step -

		Find the values on the left and top sides of each rectangle
		The right and bottom sides are the left and top sides of the neighboring rectangles,
		  so we don't need to calculate them

		The height field wraps, so we're never left hanging.  The right side of the last
			rectangle in a row is the left side of the first rectangle in the row.  The bottom
			side of the last rectangle in a column is the top side of the first rectangle in
			the column

              .......
		      .     .
		      .     .
		      .  d  .
		      .     .
		      .     .
		......a..g..b
		.     .     .
		.     .     .
		.  e  h  f  .
		.     .     .
		.     .     .
		......c......

		g = (d+f+a+b)/4 + random
		h = (a+c+e+f)/4 + random
		
		In the code below:
			a = (i,j) 
			b = (ni,j) 
			c = (i,nj) 
			d = (mi,pmj) 
			e = (pmi,mj) 
			f = (mi,mj) 
			g = (mi,j)
			h = (i,mj)

		*/
		for (i=0;i<size;i+=rectSize)
		for (j=0;j<size;j+=rectSize)
		{

			ni = (i+rectSize)%size;
			nj = (j+rectSize)%size;

			mi = (i+rectSize/2);
			mj = (j+rectSize/2);

			pmi = (i-rectSize/2+size)%size;
			pmj = (j-rectSize/2+size)%size;

			// Calculate the square value for the top side of the rectangle
			field[mi+j*size] = (field[i+j*size] + field[ni+j*size] + field[mi+pmj*size] + field[mi+mj*size])/4 + RangedRandom(-dh/2,dh/2);

			// Calculate the square value for the left side of the rectangle
			field[i+mj*size] = (field[i+j*size] + field[i+nj*size] + field[pmi+mj*size] + field[mi+mj*size])/4 + RangedRandom(-dh/2,dh/2);

		}

		// Setup values for next iteration
		// At this point, the height field has valid values at each of the coordinates that fall on a rectSize/2 boundary

		rectSize /= 2;
		dh *= r;
	}
	
	// Normalize terrain so minimum value is 0 and maximum value is 1
	NormalizeTerrain(field,size);

}

float Terrain::GetHeight(double x, double z)
{	

	

	float projCameraX, projCameraZ;

	// divide by the grid-spacing if it is not 1
	projCameraX = float(x / terrainMul);
	projCameraZ = float(z / terrainMul);

	// compute the height field coordinates (Col0, Row0)
	// and (Col1, Row1) that identify the height field cell 
	// directly below the camera.
	int col0 = int(projCameraX);
	int row0 = int(projCameraZ);
	int col1 = col0 + 1;
	int row1 = row0 + 1;
	
	// make sure that the cell coordinates don't fall
	// outside the height field.
	if (col1 > width)
		col1 = 0;
	if (row1 > width)
		row1 = 0;

	// get the four corner heights of the cell from the height field
	float h00 = heightMul * (float)heightMap[col0 + row0*width];
	float h01 = heightMul * (float)heightMap[col1 + row0*width];
	float h11 = heightMul * (float)heightMap[col1 + row1*width];
	float h10 = heightMul * (float)heightMap[col0 + row1*width];

	// calculate the position of the camera relative to the cell.
	// note, that 0 <= tx, ty <= 1.
	float tx = projCameraX - float(col0);
	float ty = projCameraZ - float(row0);

	// the next step is to perform a bilinear interpolation
	// to compute the height of the terrain directly below
	// the object.
	float txty = tx * ty;

	float final_height = h00 * (1.0f - ty - tx + txty)
					+ h01 * (tx - txty)
					+ h11 * txty
					+ h10 * (ty - txty);

	return final_height;
}