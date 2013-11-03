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
#include "light.h"
#include "bvh.h"


using namespace cimg_library;
using namespace std;

#define PI 3.14159265  // Should be used from mathlib
inline float sqr(float x) { return x*x; }

void traverse(bvh *tree);

//****************************************************
// Global Variables
//****************************************************
float imageMap[2000][2000][3] ={0};

int maxDepth=-1, width, height;
string fname = "output.bmp";

long maxVertx;

bool antiAliasing = false;

//camera
vect lookat, lookfrom, up;
float fov;

//brdf
color ka,kd,ks,kr =color(0.0, 0.0, 0.0), em;
float sp, rindex = 0;
//rindex = refractive index

//****************************************************
// Rendering
//****************************************************

int main(int argc, char *argv[]) 
	{
	vector<object*> objs;
    vector<vect*> vertices;
    vector<light*> lights;

	float x,y,z,r,g,b,v1,v2,v3;
	//store variables and set stuff at the end

	string file = "scene1.test";
	cout<<"Enter file name:"<<endl;
	cin>>file;
	
	std::ifstream inpfile(file.c_str());
	  if(!inpfile.is_open()) 
			{
			std::cout << "Unable to open file" << std::endl;
		    } 
	  else {
			std::string line;
            vector<matrix> mst;
            matrix m;
            mst.push_back(m);
            vect attenuationCoefficients;

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
				fov = (float) (degree/(float)180) * PI; 
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
				objs.push_back(new object(sphere(vect(x,y,z,1), r), brdf(ka,kd,ks,kr,em,sp,rindex), mst.back()));
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
			  else if(!splitline[0].compare("maxvertnorms")) {
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
			  else if(!splitline[0].compare("vertexnormal")) {
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
                  objs.push_back(new object(triangle(&vertices, v1,v2,v3), brdf(ka,kd,ks,kr,em,sp,rindex), mst.back()));
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
                  matrix translate = m.translate(atof(splitline[1].c_str()), atof(splitline[2].c_str()), atof(splitline[3].c_str()));

                  // Update top of matrix stack
                  mst.back() = mst.back() * translate;
                  
              }
                //rotate x y z angle
                //  Rotate by angle (in degrees) about the given axis as in OpenGL.
              else if(!splitline[0].compare("rotate")) {
                  matrix rotate;
                  float angle = atof(splitline[4].c_str()) / 180 * PI;
                  if (atof(splitline[1].c_str()) == 1) {
                      rotate = m.rotateX(angle);
                  } else if (atof(splitline[2].c_str()) == 1) {
                      rotate = m.rotateY(angle);
                  } else {
                      rotate = m.rotateZ(angle);
                  }
                  
                  // Update top of matrix stack
                  mst.back() = mst.back() * rotate;
                  
              }
                //scale x y z
                //  Scale by the corresponding amount in each axis (a non-uniform scaling).
              else if(!splitline[0].compare("scale")) {
                  matrix scale = m.scale(atof(splitline[1].c_str()), atof(splitline[2].c_str()), atof(splitline[3].c_str()));
                  
                  // Update top of matrix stack
                  mst.back() = mst.back() * scale;

              }
                //pushTransform
                //  Push the current modeling transform on the stack as in OpenGL.
                //  You might want to do pushTransform immediately after setting
                //   the camera to preserve the “identity” transformation.
              else if(!splitline[0].compare("pushTransform")) {
                  mst.push_back(mst.back());
              }
                //popTransform
                //  Pop the current transform from the stack as in OpenGL.
                //  The sequence of popTransform and pushTransform can be used if
                //  desired before every primitive to reset the transformation
                //  (assuming the initial camera transformation is on the stack as
                //  discussed above).
              else if(!splitline[0].compare("popTransform")) {
                  mst.pop_back();
              }
			  //directional x y z r g b
			  //  The direction to the light source, and the color, as in OpenGL.
			  else if(!splitline[0].compare("directional")) {
                  x = (float)atof(splitline[1].c_str());
                  y = (float)atof(splitline[2].c_str());
                  z = (float)atof(splitline[3].c_str());
                  r = (float)atof(splitline[4].c_str());
                  g = (float)atof(splitline[5].c_str());
                  b = (float)atof(splitline[6].c_str());
                  
                  // add light to scene...
                  lights.push_back(new light(x, y, z, r, g, b, attenuationCoefficients, 0));
              }
			  //point x y z r g b
			  //  The location of a point source and the color, as in OpenGL.
			  else if(!splitline[0].compare("point")) {
                  x = (float)atof(splitline[1].c_str());
                  y = (float)atof(splitline[2].c_str());
                  z = (float)atof(splitline[3].c_str());
                  r = (float)atof(splitline[4].c_str());
                  g = (float)atof(splitline[5].c_str());
                  b = (float)atof(splitline[6].c_str());
                  
                  // add light to scene...
                  lights.push_back(new light(x, y, z, r, g, b, attenuationCoefficients, 1));

			  }
			  //attenuation const linear quadratic
			  //  Sets the constant, linear and quadratic attenuations 
			  //  (default 1,0,0) as in OpenGL.
			  else if(!splitline[0].compare("attenuation")) {
				    attenuationCoefficients = vect(atof(splitline[1].c_str()), atof(splitline[2].c_str()), atof(splitline[3].c_str()), 0);
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
				sp = (float) atof(splitline[1].c_str());
				// Update current properties
			  }
			  //emission r g b
			  //  gives the emissive color of the surface.
			  else if(!splitline[0].compare("emission")) {
				r = (float) atof(splitline[1].c_str());
				g = (float) atof(splitline[2].c_str());
				b = (float) atof(splitline[3].c_str());
				em = color(r,g,b);
				// Update current properties
			  }
			  else if(!splitline[0].compare("reflection")){
				r = (float) atof(splitline[1].c_str());
				g = (float) atof(splitline[2].c_str());
				b = (float) atof(splitline[3].c_str());
				kr = color(r,g,b);
			  }
              //activates anti aliasing
              else if(!splitline[0].compare("antiAliasing")){
                antiAliasing = true;
			  }
              //activates anti aliasing
              else if(!splitline[0].compare("rindex")){
                rindex = (float) atof(splitline[1].c_str());
			  }			  
			  else {
				std::cerr << "Unknown command: " << splitline[0] << std::endl;
			  }
			}
		   inpfile.close();
		  }
	
	if(maxDepth == -1) maxDepth = 5;

	//set camera
    camera eye = camera(lookfrom, (lookat - lookfrom), up);


	//set film
	film screen = film((float)width, (float)height, (float)fov, lookat);

	//create all objects
	allObjects objects = allObjects(objs);



	bvh *tree = new bvh();
	cout<<"Building BVH tree ... ";
	buildBVH(objs, tree, 0, objs.size(), 0);
	cout<<" done"<<endl;
	

	//create ray tracer
	rayTracer tracer = rayTracer(objects, lights, tree);

	cout<<"Tracing rays ... ";
	int i, j;
	for (i = 0; i < width; i++)
		{
		cout<<".";
		for ( j = 0; j < height; j++)
			{
			vect pt = screen.getPoint((float)i,(float)j);
            vect direction = eye.getU() * pt.getX() + eye.getV() * pt.getY() + eye.getW();
            ray generatedRay = ray(lookfrom, direction, (float)0.001, FLT_MAX);
            color c= tracer.trace(generatedRay, maxDepth, -1);
			
			imageMap[i][j][0] = min((float)255,c.getR()*255);
			imageMap[i][j][1] = min((float)255,c.getG()*255);
			imageMap[i][j][2] = min((float)255,c.getB()*255);
			}
		}
	cout<<"done"<<endl;

	//draw out
	CImg<float> image(width,height,1,3,0);

    //image.save("output.bmp");
    for (int x = 0; x < width;x++)
		{
		
		for (int y = 0; y < height;y++)
			{
            //Simple Anti-Aliasing, averaging out pixel values
                float color[3];
                float r=0.0f, g=0.0f, b=0.0f;
                for (int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        if (i == 0 || j == 0) {
                            r += (imageMap[x+i][y+j][0])/5;
                            g += (imageMap[x+i][y+j][1])/5;
                            b += (imageMap[x+i][y+j][2])/5;
                        }
                    }
                }
			// find front object, map color of that pixel to imageMap and draw
                if (antiAliasing) {color[0] = r; color[1] = g; color[2] = b;}
                else {color[0] = imageMap[x][y][0]; color[1] = imageMap[x][y][1]; color[2] = imageMap[x][y][2];}
			image.draw_point(x,y,color);
			}
		}
	image.save(fname.c_str());

    
    image.display("Assignment 2");           // Display the image in a display window.
    return 0;
	}

	void traverse(bvh *tree)
		{
		//cout<<"bounding box:"<<endl;
		//cout<<"min: "<<tree->getmin()<<endl;
		//cout<<"max: "<<tree->getmax()<<endl;

		//bottom
		if(tree->getLeft()==NULL && tree->getRight()==NULL) 
		{
		//cout<<"object type: "<<tree->getObj()->getType()<<endl;
		return;
		}
		if(tree->getLeft()!=NULL) traverse(tree->getLeft());
		if(tree->getRight()!=NULL) traverse(tree->getRight());
		}