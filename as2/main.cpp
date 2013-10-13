#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <math.h>

#include <stdlib.h>
#include <stdio.h>
#include "CImg.h"
#include "camera.h"
#include "vect.h"
#include "ray.h"
#include "sphere.h"
#include "film.h"
#include "matrix.h"

using namespace cimg_library;
using namespace std;

#define PI 3.14159265  // Should be used from mathlib
inline float sqr(float x) { return x*x; }


//****************************************************
// Global Variables
//****************************************************
int w = 640, h = 480;
float imageMap[2000][2000][3] ={0};

int main(int argc, char *argv[]) 
	{
	
	//set camera
	vect lookat = vect(0,0,-1,1),lookfrom = vect(0,0,0,0), up = vect(0,1,0,0),y=vect(0,1,0,0);
	camera eye = camera(lookat.getUnitVector(), (lookfrom-lookat).getUnitVector(), up);

	//set film
	float width = 640, height = 480, fov = (float)3.1415926/4;
	film screen = film(width, height, abs(lookfrom.getZ() - lookat.getZ()), fov, lookat);

	//set sphere
	vect sphereCenter(5,5,-25,1);
	float sphereRadius = 3;
	sphere sphere1 = sphere(sphereCenter, sphereRadius);


	//this is a cheap hack to get the rotate matrix
	matrix m = matrix();
	matrix cw = m.rotateZ(up.getAngle(y)), ccw = m.rotateZ(-up.getAngle(y));
	vect v1 = cw*up,v2 = ccw*up;
	if(v1.getAngle(y) < v2.getAngle(y)) m = cw;
	else m = ccw;

	
	int i, j;
	for (i = 0; i < width; i++)
		{
		for ( j = 0; j < height; j++)
			{
			vect pt = screen.getPoint((float)i,(float)j);
			ray generatedRay = ray(pt, pt-lookfrom);

			if(sphere1.getIntersection(generatedRay)!=-1)
				{
				imageMap[i][j][0] = 200;
				imageMap[i][j][1] = 44;
				imageMap[i][j][2] = 555;
				}
			}
		}


	CImg<float> image(w,h,1,3,0);

    //image.save("output.bmp");
    for (int x = 0; x < w;x++)
		{
		for (int y = 0; y < h;y++)
			{
			// find front object, map color of that pixel to imageMap and draw
			float color[3] = {imageMap[x][y][0],imageMap[x][y][1],imageMap[x][y][2]};
			image.draw_point(x,y,color);
			}
		}
    
    image.display("Assignment 2");           // Display the image in a display window.
    return 0;
	}

