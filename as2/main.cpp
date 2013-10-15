#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>

#include "CImg.h"
#include "camera.h"
#include "vect.h"
#include "ray.h"
#include "sphere.h"
#include "triangle.h"
#include "film.h"
#include "matrix.h"
#include "object.h"
#include "allObjects.h"
#include "rayTracer.h"

using namespace cimg_library;
using namespace std;

#define PI 3.14159265  // Should be used from mathlib
inline float sqr(float x) { return x*x; }


//****************************************************
// Global Variables
//****************************************************
float imageMap[2000][2000][3] ={0};

int maxDepth, width, height;
string fname = "output.bmp";

long maxVertx;

//camera
vect lookat, lookfrom, up;
float fov;

//brdf
color ka,kd,ks,kr;
float shininess;

int main(int argc, char *argv[]) 
	{
	vector<object*> objs;
	vector<vect*> vertices;

	float x,y,z,r,g,b,v1,v2,v3;
	//store variables and set stuff at the end

	string file = "scene1.test";
	std::ifstream inpfile(file.c_str());
	  if(!inpfile.is_open()) 
			{
			std::cout << "Unable to open file" << std::endl;
		    } 
	  else {
			std::string line;
			//MatrixStack mst;

			while(inpfile.good()) {
			  std::vector<std::string> splitline;
			  std::string buf;

			  std::getline(inpfile,line);
			  std::stringstream ss(line);

			  while (ss >> buf) {
				splitline.push_back(buf);
			  }
			  //Ignore blank lines
			  if(splitline.size() == 0) {
				continue;
			  }

			  //Ignore comments
			  if(splitline[0][0] == '#') {
				continue;
			  }

			  //Valid commands:
			  //size width height
			  //  must be first command of file, controls image size
			  else if(!splitline[0].compare("size")) {
				width = atoi(splitline[1].c_str());
				height = atoi(splitline[2].c_str());
			  }
			  //maxdepth depth
			  //  max # of bounces for ray (default 5)
			  else if(!splitline[0].compare("maxdepth")) {
				maxDepth = atoi(splitline[1].c_str());
			  }
			  //output filename
			  //  output file to write image to 
			  else if(!splitline[0].compare("output")) {
				fname = splitline[1];
			  }

			  //camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov
			  //  speciﬁes the camera in the standard way, as in homework 2.
			else if(!splitline[0].compare("camera")) 
				{
				// lookfrom:
					x = (float)atof(splitline[1].c_str());
					y = (float)atof(splitline[2].c_str());
					z = (float)atof(splitline[3].c_str());
					lookfrom = vect(x,y,z,1);

				// lookat:
					x = (float)atof(splitline[4].c_str());
					y = (float)atof(splitline[5].c_str());
					z = (float)atof(splitline[6].c_str());
					lookat = vect(x,y,z,1);

				// up:
					x = (float)atof(splitline[7].c_str());
					y = (float)atof(splitline[8].c_str());
					z = (float)atof(splitline[9].c_str());
					up = vect(x,y,z,0);
				// fov: 
				float degree = (float)atof(splitline[10].c_str());
				fov = (float) (degree/180) * PI; 
				}

			  //sphere x y z radius
			  //  Deﬁnes a sphere with a given position and radius.
			  else if(!splitline[0].compare("sphere")) 
				{
				x = (float)atof(splitline[1].c_str());
				y = (float)atof(splitline[2].c_str());
				z = (float)atof(splitline[3].c_str());
				r = (float)atof(splitline[4].c_str());

				// Create new sphere:
				//   Store 4 numbers
				//   Store current property values
				//   Store current top of matrix stack

				objs.push_back(new object(sphere(vect(x,y,z,1), r), brdf(ka,kd,ks,kr)));
				}
			  //maxverts number
			  //  Deﬁnes a maximum number of vertices for later triangle speciﬁcations. 
			  //  It must be set before vertices are deﬁned.
			  else if(!splitline[0].compare("maxverts")) 
					{
					maxVertx = atol(splitline[1].c_str());
					// Care if you want
					// Here, either declare array size
					// Or you can just use a STL vector, in which case you can ignore this
					}
			  //maxvertnorms number
			  //  Deﬁnes a maximum number of vertices with normals for later speciﬁcations.
			  //  It must be set before vertices with normals are deﬁned.
			  else if(!splitline[0].compare("maxvertnorms")) 
				{
				// Care if you want
				}
			  //vertex x y z
			  //  Deﬁnes a vertex at the given location.
			  //  The vertex is put into a pile, starting to be numbered at 0.
			  else if(!splitline[0].compare("vertex")) 
				{
				x = (float)atof(splitline[1].c_str());
				y = (float)atof(splitline[2].c_str());
				z = (float)atof(splitline[3].c_str());	
				
				// Create a new vertex with these 3 values, store in some array
				vertices.push_back(new vect(x,y,z,1));
			    }
			  //vertexnormal x y z nx ny nz
			  //  Similar to the above, but deﬁne a surface normal with each vertex.
			  //  The vertex and vertexnormal set of vertices are completely independent
			  //  (as are maxverts and maxvertnorms).
			  else if(!splitline[0].compare("vertexnormal")) 
				{
				// x: atof(splitline[1].c_str()),
				// y: atof(splitline[2].c_str()),
				// z: atof(splitline[3].c_str()));
				// nx: atof(splitline[4].c_str()),
				// ny: atof(splitline[5].c_str()),
				// nz: atof(splitline[6].c_str()));
				// Create a new vertex+normal with these 6 values, store in some array
				}
			  //tri v1 v2 v3
			  //  Create a triangle out of the vertices involved (which have previously been speciﬁed with
			  //  the vertex command). The vertices are assumed to be speciﬁed in counter-clockwise order. Your code
			  //  should internally compute a face normal for this triangle.
			  else if(!splitline[0].compare("tri")) 
					{

					// Create new triangle:
					//   Store pointer to array of vertices
					//   Store 3 integers to index into array
					//   Store current property values
					//   Store current top of matrix stack

					v1 = (float)atof(splitline[1].c_str());
					v2 = (float)atof(splitline[2].c_str());
					v3 = (float)atof(splitline[3].c_str());

					// Create new triangle:
					objs.push_back(new object(triangle(&vertices, v1,v2,v3), brdf(ka,kd,ks,kr)));
					}
			  //trinormal v1 v2 v3
			  //  Same as above but for vertices speciﬁed with normals.
			  //  In this case, each vertex has an associated normal, 
			  //  and when doing shading, you should interpolate the normals 
			  //  for intermediate points on the triangle.
			  else if(!splitline[0].compare("trinormal")) {
				// v1: atof(splitline[1].c_str())
				// v2: atof(splitline[2].c_str())
				// v3: atof(splitline[3].c_str())
				// Create new triangle:
				//   Store pointer to array of vertices (Different array than above)
				//   Store 3 integers to index into array
				//   Store current property values
				//   Store current top of matrix stack
			  }

			  //translate x y z
			  //  A translation 3-vector
			  else if(!splitline[0].compare("translate")) {
				// x: atof(splitline[1].c_str())
				// y: atof(splitline[2].c_str())
				// z: atof(splitline[3].c_str())
				// Update top of matrix stack
			  }
			  //rotate x y z angle
			  //  Rotate by angle (in degrees) about the given axis as in OpenGL.
			  else if(!splitline[0].compare("rotate")) {
				// x: atof(splitline[1].c_str())
				// y: atof(splitline[2].c_str())
				// z: atof(splitline[3].c_str())
				// angle: atof(splitline[4].c_str())
				// Update top of matrix stack
			  }
			  //scale x y z
			  //  Scale by the corresponding amount in each axis (a non-uniform scaling).
			  else if(!splitline[0].compare("scale")) {
				// x: atof(splitline[1].c_str())
				// y: atof(splitline[2].c_str())
				// z: atof(splitline[3].c_str())
				// Update top of matrix stack
			  }
			  //pushTransform
			  //  Push the current modeling transform on the stack as in OpenGL. 
			  //  You might want to do pushTransform immediately after setting 
			  //   the camera to preserve the “identity” transformation.
			  else if(!splitline[0].compare("pushTransform")) {
				//mst.push();
			  }
			  //popTransform
			  //  Pop the current transform from the stack as in OpenGL. 
			  //  The sequence of popTransform and pushTransform can be used if 
			  //  desired before every primitive to reset the transformation 
			  //  (assuming the initial camera transformation is on the stack as 
			  //  discussed above).
			  else if(!splitline[0].compare("popTransform")) {
				//mst.pop();
			  }

			  //directional x y z r g b
			  //  The direction to the light source, and the color, as in OpenGL.
			  else if(!splitline[0].compare("directional")) {
				// x: atof(splitline[1].c_str()),
				// y: atof(splitline[2].c_str()),
				// z: atof(splitline[3].c_str()));
				// r: atof(splitline[4].c_str()),
				// g: atof(splitline[5].c_str()),
				// b: atof(splitline[6].c_str()));
				// add light to scene...
			  }
			  //point x y z r g b
			  //  The location of a point source and the color, as in OpenGL.
			  else if(!splitline[0].compare("point")) {
				// x: atof(splitline[1].c_str()),
				// y: atof(splitline[2].c_str()),
				// z: atof(splitline[3].c_str()));
				// r: atof(splitline[4].c_str()),
				// g: atof(splitline[5].c_str()),
				// b: atof(splitline[6].c_str()));
				// add light to scene...
			  }
			  //attenuation const linear quadratic
			  //  Sets the constant, linear and quadratic attenuations 
			  //  (default 1,0,0) as in OpenGL.
			  else if(!splitline[0].compare("attenuation")) {
				// const: atof(splitline[1].c_str())
				// linear: atof(splitline[2].c_str())
				// quadratic: atof(splitline[3].c_str())
			  }
			  //ambient r g b
			  //  The global ambient color to be added for each object 
			  //  (default is .2,.2,.2)
			  else if(!splitline[0].compare("ambient")) {
				 r = (float)atof(splitline[1].c_str());
				 g = (float)atof(splitline[2].c_str());
				 b = (float)atof(splitline[3].c_str());
				 ka = color(r,g,b);
			  }

			  //diﬀuse r g b
			  //  speciﬁes the diﬀuse color of the surface.
			  else if(!splitline[0].compare("diffuse")) {
				r = (float)atof(splitline[1].c_str());
				g = (float)atof(splitline[2].c_str());
				b = (float)atof(splitline[3].c_str());
				kd = color(r,g,b);
				// Update current properties
			  }
			  //specular r g b 
			  //  speciﬁes the specular color of the surface.
			  else if(!splitline[0].compare("specular")) {
				 r = (float)atof(splitline[1].c_str());
				 g = (float)atof(splitline[2].c_str());
				 b = (float)atof(splitline[3].c_str());
				 ks = color(r,g,b);
				// Update current properties
			  }
			  //shininess s
			  //  speciﬁes the shininess of the surface.
			  else if(!splitline[0].compare("shininess")) {
				shininess = atof(splitline[1].c_str());
				// Update current properties
			  }
			  //emission r g b
			  //  gives the emissive color of the surface.
			  else if(!splitline[0].compare("emission")) {
				// r: atof(splitline[1].c_str())
				// g: atof(splitline[2].c_str())
				// b: atof(splitline[3].c_str())
				// Update current properties
			  } else {
				std::cerr << "Unknown command: " << splitline[0] << std::endl;
			  }
			}
		   inpfile.close();
		  }

	//set camera
	camera eye = camera(lookat.getUnitVector(), (lookfrom-lookat).getUnitVector(), up);

	//set film
	film screen = film(width, height, abs(lookfrom.getZ() - lookat.getZ()), fov, lookat);

	//create all objects
	allObjects objects = allObjects(objs);


	//create ray tracer
	rayTracer tracer = rayTracer(objects);

	//this is a cheap hack to get the rotate matrix
	/*
	matrix m = matrix();
	matrix cw = m.rotateZ(up.getAngle(y)), ccw = m.rotateZ(-up.getAngle(y));
	vect v1 = cw*up,v2 = ccw*up;
	if(v1.getAngle(y) < v2.getAngle(y)) m = cw;
	else m = ccw;
	*/

	int i, j;
	for (i = 0; i < width; i++)
		{
		for ( j = 0; j < height; j++)
			{
			vect pt = screen.getPoint((float)i,(float)j);
			ray generatedRay = ray(pt, pt-lookfrom);

			color c= tracer.trace(generatedRay, 5);
			
			imageMap[i][j][0] = c.getR();
			imageMap[i][j][1] = c.getG();
			imageMap[i][j][2] = c.getB();
			}
		}

	//draw out
	CImg<float> image(width,height,1,3,0);

    //
    for (int x = 0; x < width;x++)
		{
		for (int y = 0; y < height;y++)
			{
			// find front object, map color of that pixel to imageMap and draw
			float color[3] = {imageMap[x][y][0],imageMap[x][y][1],imageMap[x][y][2]};
			image.draw_point(x,y,color);
			}
		}
    image.save("output.bmp");

    //image.display("Assignment 2");           // Display the image in a display window.
    return 0;
	}

