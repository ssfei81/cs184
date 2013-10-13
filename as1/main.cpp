
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>
//#include "vect.h"
#include "effect.h"
#include <algorithm>
#include "CImg.h"
using namespace cimg_library;

#define PI 3.14159265  // Should be used from mathlib
inline float sqr(float x) { return x*x; }

using namespace std;

//****************************************************
// Some Classes
//****************************************************

class Viewport;

class Viewport 
{
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport viewport;
GLfloat ka[5][3]={0},kd[5][3]={0},ks[5][3]={0}, sp[3]={0};
int PLCount = 0; int DLCount = 0;

GLfloat DLpos[5][3]={0}, PLpos[5][3]={0},PLrgb[5][3]={0}, DLrgb[5][3]={0};

int KACount = 0, KDCount = 0, KSCount = 0, SPCount = 0;

float cX[3]={0}, cY[3]={0};

//flags
int draw = 0, shadow = 0, toon = 0, aniso = 0, torus = 0;
float anU, anV;


//image
float imageMap[1000][1000][3] ={0};

//****************************************************
// Simple init function
//****************************************************
void initScene(int argc, char *argv[])
	{	
	int i = 1;
	while (i < argc)
		{
		if (!strcmp(argv[i],"-ka")) {ka[KACount][0] = atof(argv[i+1]); ka[KACount][1] =atof(argv[i+2]); ka[KACount][2] = atof(argv[i+3]); i+=4; KACount++;}
		else if (!strcmp(argv[i],"-kd")) {kd[KDCount][0] = atof(argv[i+1]); kd[KDCount][1] =atof(argv[i+2]); kd[KDCount][2] = atof(argv[i+3]); i+=4; KDCount++;}
		else if (!strcmp(argv[i],"-ks")) {ks[KSCount][0] = atof(argv[i+1]); ks[KSCount][1] =atof(argv[i+2]); ks[KSCount][2] = atof(argv[i+3]); i+=4; KSCount++;}
		else if (!strcmp(argv[i],"-sp")) {sp[SPCount] = atof(argv[i+1]); i+=2; SPCount++;}
		else if (!strcmp(argv[i],"-pl")) 
			{
			PLpos[PLCount][0] = atof(argv[i+1]); PLpos[PLCount][1] = atof(argv[i+2]); PLpos[PLCount][2] = atof(argv[i+3]);
			PLrgb[PLCount][0] = atof(argv[i+4]); PLrgb[PLCount][1] = atof(argv[i+5]); PLrgb[PLCount][2] = atof(argv[i+6]);
			PLCount++;
			i+=7;
			}
		else if (!strcmp(argv[i],"-dl")) 
			{
			DLpos[DLCount][0] = atof(argv[i+1]); DLpos[DLCount][1] = atof(argv[i+2]); DLpos[DLCount][2] = atof(argv[i+3]);
			DLrgb[DLCount][0] = atof(argv[i+4]); DLrgb[DLCount][1] = atof(argv[i+5]); DLrgb[DLCount][2] = atof(argv[i+6]);
			DLCount++;
			i+=7;
			}
		//draw to bmp
		else if (!strcmp(argv[i],"-image")) 
			{
			draw = 1; i++;
			}

		else if(!strcmp(argv[i], "-shadow"))
			{
			shadow = 1; i++;
			}

		else if(!strcmp(argv[i], "-toon"))
			{ 
			toon = 1; i++;
			}
        else if(!strcmp(argv[i], "-an"))
			{
			anU = atof(argv[i+1]); anV = atof(argv[i+2]);
			aniso = 1;
			i+=3;
			}
		else if(!strcmp(argv[i], "-torus"))
			{
			torus = 1;
			i++;
			}
		}	
	}

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) 
{
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, viewport.w, 0, viewport.h);

}


//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) 
{
  glColor3f(r, g, b);
  glVertex2f(x + 0.5, y + 0.5);   // The 0.5 is to target pixel
  // centers 
  // Note: Need to check for gap
  // bug on inst machines.
}

//****************************************************
// Draw a filled circle.  
//****************************************************

void circle(float centerX, float centerY, float radius, float originX, float originY, int sphereID, float radius2) 
	{

  int i,j;  // Pixel indices

  int minI = max(0,(int)floor(centerX-radius));
  int maxI = min(viewport.w-1,(int)ceil(centerX+radius));

  int minJ = max(0,(int)floor(centerY-radius));
  int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));

  for (i=0;i<viewport.w;i++) 
	{
    for (j=0;j<viewport.h;j++) 
		{
		// Location of the center of pixel relative to center of sphere
		float x = (i+0.5-centerX);
		float y = (j+0.5-centerY);

		//determine shift of origin
		float shiftX = centerX - originX;
		float shiftY = centerY - originY;

		float dist = sqrt(sqr(x) + sqr(y));


		float start, end;
		if(torus) {start = radius; end = radius2;}
		else {start = 0; end = radius;}

		if (dist<=end && dist>=start) 
		{
		GLfloat red = 0 , green = 0, blue = 0;
		GLfloat redAniso = 0, greenAniso = 0, blueAniso = 0;
		GLfloat redToon = 0, greenToon = 0, blueToon = 0;

		// This is the front-facing Z coordinate
		
		float z;
		float torusRadius;
		float t;
		if(torus) 
			{
			float mid = 0.5 * (radius + radius2);
			torusRadius = 0.5*(radius2-radius);
			z = torusRadius - sqrt(pow((mid-dist),2));
			t = mid/dist;
			}
		else z= sqrt(radius*radius-dist*dist);

		//ambient term

		red += ka[sphereID][0]*255;
		green += ka[sphereID][1]*255;
		blue += ka[sphereID][2]*255;

		float radiusOffset;
		if(torus) radiusOffset = radius2;
		else radiusOffset = radius;

		//directional light
		for (int u = 0; u < DLCount; u++)
			{

			//point relative to the center of the screen
			vect pixelPt(x + shiftX,y+shiftY,z), lightPt(DLpos[u][0]*radiusOffset, DLpos[u][1]*radiusOffset, DLpos[u][2]*radiusOffset), viewPt(0,0,1);
			lightPt = -1*lightPt;
			vect SphereOrigin(shiftX, shiftY, 0);

			vect normalVector = (pixelPt - SphereOrigin).getUnitVector(), lightVector = (lightPt).getUnitVector();
			//this is just l.n
			GLfloat angle = lightVector^normalVector;
			vect reflectedVector = (-1 * lightVector + (2 * angle * normalVector)).getUnitVector();

			int hit = 0;
			for (int t = 0; t<KACount;t++)
				{
			    if(sphereID != t)
					{
					vect d = (lightPt*radius-pixelPt);
					float a = d^d;
					vect eminusc = (pixelPt - vect(cX[t], cY[t], 0));
					float b = 2*d^(eminusc);
					float c = (eminusc^eminusc) - (radius * radius);

					float discriminant = b*b - (4*a*c);

					if(discriminant >= 0) 
						{
					    float t1 = (-(d^eminusc) + sqrt(discriminant))/a;
						float t2 = (-(d^eminusc) - sqrt(discriminant))/a;
						if(t1>0 || t2>0) hit = 1;
						}
					}
				}

			if(!hit || !shadow)
				{
				//diffuse term(s)
				red += kd[sphereID][0] * DLrgb[u][0] * 255 * max(0.0f, angle);
				green += kd[sphereID][1] * DLrgb[u][1] * 255 * max(0.0f, angle);
				blue += kd[sphereID][2] * DLrgb[u][2] * 255 * max(0.0f, angle);

				//specular term(s)
				red += ks[sphereID][0] * DLrgb[u][0] * 255 * pow(max(0.0f, reflectedVector^viewPt), sp[sphereID]);
				green += ks[sphereID][1] * DLrgb[u][1] * 255 * pow(max(0.0f, reflectedVector^viewPt), sp[sphereID]);
				blue += ks[sphereID][2] * DLrgb[u][2] * 255 * pow(max(0.0f, reflectedVector^viewPt), sp[sphereID]);
				}
			
				//apply toon effect
				if(toon)
				{
				vect color = tooning(red, green, blue, angle);
				redToon += color.getX();
				greenToon += color.getY();
				blueToon += color.getZ();
				}
                
                //apply anisotropic diffusion
                if(aniso) 
                {
                    redAniso += anisotropic(lightPt, pixelPt, viewPt, normalVector, anU, anV, ks[sphereID][0], kd[sphereID][0],0);
                    greenAniso += anisotropic(lightPt, pixelPt, viewPt, normalVector, anU, anV, ks[sphereID][1], kd[sphereID][1],0);
                    blueAniso += anisotropic(lightPt, pixelPt, viewPt, normalVector, anU, anV, ks[sphereID][2], kd[sphereID][2],0);
                }
			
		}
		//point light
		for (int u = 0; u < PLCount; u++)
			{
			vect pixelPt(x+shiftX,y+shiftY,z), lightPt(PLpos[u][0]*radiusOffset, PLpos[u][1]*radiusOffset, PLpos[u][2]*radiusOffset), viewPt(0,0,1);
			

			if(torus)
				{
				shiftX = x*t;
				shiftY = y*t;
				}

			vect SphereOrigin(shiftX, shiftY, 0);
			vect normalVector = (pixelPt-SphereOrigin).getUnitVector(), lightVector = (lightPt-pixelPt).getUnitVector();

			

			//this is just l.n
			GLfloat angle = lightVector^normalVector;
			vect reflectedVector = (-1 * lightVector + (2 * angle * normalVector)).getUnitVector();


			//check intersection

			int hit = 0;
			for (int t = 0; t<KACount;t++)
				{
			    if(sphereID != t)
					{
					vect d = (lightPt-pixelPt);
					float a = d^d;
					vect eminusc = (pixelPt - vect(cX[t], cY[t], 0));
					float b = 2*d^(eminusc);
					float c = (eminusc^eminusc) - (radius * radius);

					float discriminant = b*b - (4*a*c);


					if(discriminant >= 0) 
						{
					    float t1 = (-(d^eminusc) + sqrt(discriminant))/a;
						float t2 = (-(d^eminusc) - sqrt(discriminant))/a;
						if(t1>0 || t2>0) hit = 1;
						}
					}
				}

			if(!hit || !shadow)
				{
				//diffuse term(s)
				red += kd[sphereID][0] * PLrgb[u][0] * 255 * max(0.0f, angle);
				green += kd[sphereID][1] * PLrgb[u][1] * 255 * max(0.0f, angle);
				blue += kd[sphereID][2] * PLrgb[u][2] * 255 * max(0.0f, angle);

				//specular term(s)
				red += ks[sphereID][0] * PLrgb[u][0] * 255 * pow(max(0.0f, reflectedVector^viewPt), sp[sphereID]);
				green += ks[sphereID][1] * PLrgb[u][1] * 255  * pow(max(0.0f, reflectedVector^viewPt), sp[sphereID]);
				blue += ks[sphereID][2] * PLrgb[u][2] * 255 * pow(max(0.0f, reflectedVector^viewPt), sp[sphereID]);
				}
                
                if(aniso)
                {
                    redAniso += anisotropic(lightPt, pixelPt, viewPt, normalVector, anU, anV, ks[sphereID][0], kd[sphereID][0],1);
                    greenAniso += anisotropic(lightPt, pixelPt, viewPt, normalVector, anU, anV, ks[sphereID][1], kd[sphereID][1],1);
                    blueAniso += anisotropic(lightPt, pixelPt, viewPt, normalVector, anU, anV, ks[sphereID][2], kd[sphereID][2],1);
                }
				//apply toon effect
				if(toon)
				{
				vect color = tooning(red, green, blue, angle);
				redToon += color.getX();
				greenToon += color.getY();
				blueToon += color.getZ();
				}

			}		

		if(aniso)
		{
			red = redAniso;
			green = greenAniso;
			blue = blueAniso;
		} else if (toon)
		{
			red = redToon;
			green = greenToon;
			blue = blueToon;
		}

		//output to screen or image file
		if(!draw) setPixel(i,j, red/255, green/255, blue/255);
		else 
			{
			float color[]={max(0,min(255,red)), max(0,min(255,green)), max(0,min(255,blue))};
			imageMap[i][viewport.h-j][0] = color[0];
			imageMap[i][viewport.h-j][1] = color[1];
			imageMap[i][viewport.h-j][2] = color[2];

			}
		}
     }
  }
  //output image
}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() 
 {
  glClear(GL_COLOR_BUFFER_BIT);				// clear the color buffer

  glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations
  glLoadIdentity();				        // make sure transformation is "zero'd"
  glBegin(GL_POINTS);
  // Start drawing

  //set origin to be center of screen
  float originX = viewport.w/2.0, originY = viewport.h/2.0;

	if (torus)
		{
		cX[0] = 0; cY[0] = 0;
		circle(originX, originY , min(viewport.w, viewport.h)/4.0, originX, originY, 0, min(viewport.w, viewport.h)/2.5);
		}
	else if (KACount == 1) 
		{
		cX[0] = 0; cY[0] = 0;
		circle(originX, originY , min(viewport.w, viewport.h)/3.0, originX, originY, 0, 0);
		}
	else if (KACount == 2) 
		{
		cX[0] = originX/2; cY[0] = 0;
		cX[1] = -originX/2; cY[1] = 0;
		circle(originX+originX/2, originY , min(viewport.w, viewport.h)/4.0, originX, originY, 0, 0);
		circle(originX-originX/2, originY, min(viewport.w, viewport.h)/4.0, originX, originY, 1, 0);
		}
	else if (KACount == 3) 
		{
		cX[0] = 0; cY[0] = 0;
		cX[1] = -originX/1.5; cY[1] = 0;
		cX[2] = originX/1.5; cY[2] = 0;

		circle(originX, originY , min(viewport.w, viewport.h)/5.0, originX, originY, 0,0);
		circle(originX-originX/1.5, originY, min(viewport.w, viewport.h)/5.0, originX, originY, 1, 0);
		circle(originX+originX/1.5, originY, min(viewport.w, viewport.h)/5.0, originX, originY, 2, 0);
		}

  glEnd();
  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
 }

void press(unsigned char k, int x, int y)
	{
	switch(k)
		{
		case 32:
		exit(0);
		break;
		}
	} 
//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 800;
  viewport.h = 600;
  initScene(argc, argv);							// quick function to set up scene

  if(!draw)
	  {
	  //The size and position of the window
	  glutInitWindowSize(viewport.w, viewport.h);
	  glutInitWindowPosition(0,0);
	  glutCreateWindow(argv[0]);

	  glutKeyboardFunc (press); //keyboard listener
	  glutDisplayFunc(myDisplay);				// function to run when its time to draw something
	  glutReshapeFunc(myReshape);				// function to run when the window gets resized

	  glutMainLoop();							// infinite loop that will keep drawing and resizing
	  // and whatever else
	  }
  else
	  {
	  CImg<float> image(viewport.w,viewport.h,1,3,0);
	  //set origin to be center of screen
	  float originX = viewport.w/2.0, originY = viewport.h/2.0;
	
	if (torus)
		{
		cX[0] = 0; cY[0] = 0;
		circle(originX, originY , min(viewport.w, viewport.h)/4.0, originX, originY, 0, min(viewport.w, viewport.h)/2.5);
		}
	else if (KACount == 1) 
		{
		cX[0] = 0; cY[0] = 0;
		circle(originX, originY , min(viewport.w, viewport.h)/3.0, originX, originY, 0, 0);
		}
	else if (KACount == 2) 
		{
		cX[0] = originX/2; cY[0] = 0;
		cX[1] = -originX/2; cY[1] = 0;
		circle(originX+originX/2, originY , min(viewport.w, viewport.h)/4.0, originX, originY, 0, 0);
		circle(originX-originX/2, originY, min(viewport.w, viewport.h)/4.0, originX, originY, 1, 0);
		}
	else if (KACount == 3) 
		{
		cX[0] = 0; cY[0] = 0;
		cX[1] = -originX/1.5; cY[1] = 0;
		cX[2] = originX/1.5; cY[2] = 0;

		circle(originX, originY , min(viewport.w, viewport.h)/5.0, originX, originY, 0, 0);
		circle(originX-originX/1.5, originY, min(viewport.w, viewport.h)/5.0, originX, originY, 1, 0);
		circle(originX+originX/1.5, originY, min(viewport.w, viewport.h)/5.0, originX, originY, 2, 0);
		}

	  //some hack to solve the image saving problem
	  for (int x = 0; x<viewport.w;x++)
		{
		for (int y = 0; y<viewport.h;y++)
			{
			float color[3] = {imageMap[x][y][0],imageMap[x][y][1],imageMap[x][y][2]};
			image.draw_point(x,y,color);
			}
		}
	  image.save("output.bmp");
	  }
  return 0;
}








