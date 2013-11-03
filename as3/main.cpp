// CS184 Assignment 3
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
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

#include <math.h>
#include "vect.h"
#include "bezier.h"

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265

using namespace std;

//****************************************************
// Some Classes
//****************************************************
class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport viewport;
int curvecount;
float epsilon = 0.001;
float step = 0.2;
vector<bezPatch> bezPatches;

//flags
bool wmode = 0;
bool shiftpressed = 0;
float rotatex = 0, rotatey = 0,translatex = 0,translatey = 0, scale = 1;

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) 
	{
	viewport.w = w;
	viewport.h = h;

	glViewport(0,0,viewport.w,viewport.h);// sets the rectangle that will be the window
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();                // loading the identity matrix for the screen

	//----------- setting the projection -------------------------
	// glOrtho sets left, right, bottom, top, zNear, zFar of the chord system


	// glOrtho(-1, 1 + (w-400)/200.0 , -1 -(h-400)/200.0, 1, 1, -1); // resize type = add
	// glOrtho(-w/400.0, w/400.0, -h/400.0, h/400.0, 1, -1); // resize type = center

	glOrtho(-5, 5, -5, 5, 5, -5);    // resize type = stretch

	//------------------------------------------------------------
	}


//****************************************************
// sets the window up
//****************************************************
void initScene()
	{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent
	myReshape(viewport.w,viewport.h);
	}

//****************************************************
// functions
//****************************************************


void drawPatch(bezPatch patch, float step)
	{
	int numdiv = (int)((1 + epsilon) / step);
	vect *pts = new vect[(numdiv+1) * (numdiv+1)];

    for (int iv = 0; iv <= numdiv; iv++)
		{
		float v = (float)iv * step; 
		for (int iu = 0; iu <= numdiv; iu++)
			{
			float u = (float)iu * step; 
			pts[iv*(numdiv+1) + iu] = patch.bezpatchinterp(patch,u,v).pt;
			}
		}

	for (int iv = 1; iv <= numdiv; iv++)
		{
		for (int iu = 1; iu <= numdiv; iu++)
			{
			glBegin(GL_QUADS); 
			
				vect tl = pts[(iv-1)*(numdiv+1) + (iu - 1)]; 
				vect tr = pts[(iv-1)*(numdiv+1) + (iu)]; 
				vect bl = pts[(iv)*(numdiv+1) + (iu - 1)]; 
				vect br = pts[(iv)*(numdiv+1)+ (iu)]; 

				glVertex3f(tl.getX(),tl.getY(),tl.getZ());  // Top Left
				glVertex3f(bl.getX(),bl.getY(),bl.getZ());  // Bottom Left
				glVertex3f(br.getX(),br.getY(),br.getZ());  // Bottom Right
				glVertex3f(tr.getX(),tr.getY(),tr.getZ());  // Top Right	
			glEnd();   
			}
		}
	free(pts);
	}



//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() 
	{

	glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)

	glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
	glLoadIdentity();                            // make sure transformation is "zero'd"
    glPushMatrix();
	glTranslatef(translatex,translatey,0);
	glRotatef(rotatex, 1, 0, 0);
	glRotatef(rotatey, 0, 0, 1);
	glScalef(scale, scale, scale);
	
	// gluLookAt(0,0,-10,0,0,1,1,0,0);

	//----------------------- code to draw objects --------------------------
	// Rectangle Code
	//glColor3f(red component, green component, blue component);
 
	for (int i = 0; i < curvecount; i++) 
		{
		drawPatch(bezPatches[i], step);
		}

	//-----------------------------------------------------------------------
	glPopMatrix();
	glFlush();
	glutSwapBuffers();                           // swap buffers (we earlier set double buffer)
	}


//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {
  //nothing here for now
#ifdef _WIN32
  Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
#endif
  glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}

void press(unsigned char k, int x, int y)
	{
	switch(k)
		{
		case 87: case 119:
			if(!wmode)
				{
				wmode = true;
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
			else
				{
				wmode = false;
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
		break;

		case 43: case 61:
			scale += 0.1;
		break;

		case 45: case 95:
			scale -= 0.1;
		break;

		case 32:
			exit(0);
		break;
		}
	} 

void SpecialInput(int key, int x, int y)
	{
	switch(key)
		{
		case GLUT_KEY_UP:
			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) translatey += 0.1;
			else rotatex-=5;
		break;	

		case GLUT_KEY_DOWN:
			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) translatey -= 0.1;
			else rotatex+=5;
		break;

		case GLUT_KEY_LEFT:
			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) translatex -= 0.1;
			else rotatey-=5;
		break;

		case GLUT_KEY_RIGHT:
			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) translatex += 0.1;
			else rotatey+=5;
		break;
		}
	}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) 
  {
  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 600;
  viewport.h = 600;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Assignment 3");

  initScene();                                 // quick function to set up scene

    string file = "teapot.bez";
    cout<<"Enter file name:"<<endl;
	cin>>file;
    
    std::ifstream inpfile(file.c_str());

	inpfile>>curvecount;

	for (int i = 0; i < curvecount; i++)
		{
		vect cp[4][4];
		//read in the 16 control points
		for (int j = 0; j < 4; j++)
			{
			for(int k = 0; k < 4; k++)
				{
				float x,y,z;
				inpfile>>x>>y>>z;
				cp[j][k] = vect(x,y,z,1);
				}
			}
		bezPatches.push_back(bezPatch(cp));
		}

  glutSpecialFunc(SpecialInput);
  glutKeyboardFunc(press); 
  glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
  glutReshapeFunc(myReshape);                  // function to run when the window gets resized
  glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
  glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

  return 0;
  }








