// CS184 Final Project
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
#include <GL/glext.h>
#endif

#include <math.h>
#include <time.h>
#include "object.h"
#include "FreeImage.h"
#include "cloth.h"
#include "gameInterface.h"
#include "collision.h"
#include "video.h"
#include "replay.h"
#include "Menu.h"

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265

bool isMenu = true;
Menu menu;

using namespace std;

//****************************************************
// Some Classes
//****************************************************
class Viewport
{
public:
	int w, h; // width and height
};

//****************************************************
// Global Variables
//****************************************************

//viewport
Viewport viewport;

//3d objects and video
object *stadium, *football, *hulk, *pikachu, *goku, *naruto, *joker, *spongebob, *mario, *obeseman, *post;
videoDecode *video;

//gameflow control
bool replayMode = false;
bool startRecording = false;
replay *rep=NULL;

//frame rate control
struct timeval tStart, tEnd;

//misc variables
float rotatex = 0, rotatey = -40, rotatez = 0, translatex = 0, translatey = 0, translatez = 0;
float rotatexdir = 0, rotateydir = -40;
int mousex = 0; //1 = left region, 2 = right region
int mousey = 0; //1 = top region, 2 = bottom region
float keeperpos = 7.35, ballpos = 7.35;
float originalStrengthStep = strengthStep;
float ballSpeed = 25;
float yPos = 1;
int shotNumber = 0, score = 0;
const int timer = 25;
int goalTime = 0, shotTime = 0;

//character variables
struct characters
	{
	float movespeed;
    float height;
    float jumpHeight;
    float left;
    float right;
	};
characters hulkstats, pikachustats, gokustats, narutostats, jokerstats, spongebobstats, mariostats, obesemanstats, currentstats;

gameInterface *currentDisplay;

//collision surfaces
collision c;
surface* keeper = new surface(0); //Wall
surface* keeperHands = new surface(0); //Hands
surface* keeperNaruto1 = new surface(0); //Wall
surface* keeperNaruto2 = new surface(0); //Wall
surface* leftPost = new surface(0); //Left Post
surface* rightPost = new surface(0); //Right Post
surface* topPost = new surface(0); //Top Post
surface* backWall = new surface(0); //Behind post


//program flags
bool videoMode = true;
int currentCharacter = 1; //1 = hulk, 2 = pikachu
bool activatedSpecial = false, specialDisabled = false;
bool drawarrow = true;
bool jump = false, spazz = false;
bool start = true, gameOver = false, keyDisabled = false;
bool shotClock = false, scored = false, printGoal = false;
//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h)
	{
	viewport.w = w;
	viewport.h = h;
    
	glViewport(0,0,viewport.w,viewport.h);// sets the rectangle that will be the window
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();                // loading the identity matrix for the screen
	//gluPerspective(85, viewport.w/viewport.h, 1, 100);
	}

//****************************************************
// sets the window up
//****************************************************
void initScene()
	{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent
	myReshape(viewport.w,viewport.h);
        
    //character stats
	hulkstats.movespeed = 0.1;
    hulkstats.height = 3;
    hulkstats.jumpHeight = 2;
        
	pikachustats.movespeed = 0.25;
    pikachustats.height = 2.45;
    pikachustats.jumpHeight = 2.55;
	
    gokustats.movespeed = 0.2;
    gokustats.height = 2.45;
    gokustats.jumpHeight = 2.55;
	
    narutostats.movespeed = 0.2;
    narutostats.height = 2.7;
    narutostats.jumpHeight = 2.3;
	
    jokerstats.movespeed = 0.15;
    jokerstats.height = 2.8;
    jokerstats.jumpHeight = 2.2;

    spongebobstats.movespeed = 0.15;
    spongebobstats.height = 2.6;
    spongebobstats.jumpHeight = 2.4;
        
    mariostats.movespeed = 0.20;
    mariostats.height = 2.6;
    mariostats.jumpHeight = 2.4;
	
    obesemanstats.movespeed = 0.025;
    obesemanstats.height = 1.6;
    obesemanstats.jumpHeight = 2;
	
    currentstats = hulkstats; //default

    //default surfaces
    leftPost->setSurface(3.6, 11, 10.9, Vector(10.5, 1, 2.5));
    rightPost->setSurface(3.6, 3.65, 3.55, Vector(4.1, 1, 2.5));
    topPost->setSurface(3.65, 11, 3.55, Vector(5, 1, 2.5), 3.55);
    backWall->setSurface(1.5, 20, -10, Vector(5, 1, 7.5));
        
    menu = Menu();
	}

//****************************************************
// functions
//****************************************************

void resetStats(int character) {
    if(character == 1) //Hulk
        {
        currentstats.left = keeperpos + 0.5;
        currentstats.right = keeperpos - 0.5;
        currentstats.height = hulkstats.height;
        }
    else
        {
        keeperHands = new surface(0);
        }
	if(character == 2) //Pikachu
        {
        currentstats.movespeed = pikachustats.movespeed;
        }
	if(character == 3) //Goku
        {
        ballSpeed = 20.0f;
        }
	if(character == 4) //Naruto
        {
        keeperNaruto1 = new surface(0);
        keeperNaruto2 = new surface(0);
        }
	if(character == 5) //Joker
        {
        strengthStep = originalStrengthStep;
        }
	if(character == 6) //Spongebob
        {
        spazz = false;
        }
    if(character == 7) //Mario
        {
        currentstats.left = keeperpos + 0.5;
        currentstats.right = keeperpos - 0.5;
        currentstats.height = mariostats.height;
        }
	if(character == 8) //Obeseman
        {
        currentstats.height = obesemanstats.height;
        }
}

void resetBall() {
    //stop the recording
    ball.reset(ballCenter, Vector(0, 0, 0));
    ball.update();
    drawarrow = true;
    shotNumber++;
    if (scored) {
        score++;
        scored = false;
        printGoal = true;
    }
    if (shotNumber > 5) {
        gameOver = true;
    }
    keyDisabled = false;
    activatedSpecial = false;
}

void drawText(float x, float y, char *string)
{
    int len, i;
    glRasterPos2f(x, y);
    len = (int) strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}

//***************************************************
// function that does the actual drawing
//***************************************************
void Playing()
{
    glLineWidth(1);
    
    gettimeofday(&tStart, NULL);
    
    if (start) {
        resetBall();
        start = false;
    }
    if (shotClock) { //start timer for ball shot
        shotTime += timer;
    }
    if (!shotClock && !replayMode) { //fix camera when shooting or replaying
        if(mousex == 1){rotatey -= 5; rotatey = max(-170.0f, rotatey);}
        else if(mousex == 2){rotatey += 5; rotatey = min(170.0f, rotatey);}
        
        if(mousey == 1){rotatex -= 5; rotatex = max(-80.0f, rotatex);}
        else if(mousey == 2){rotatex += 5; rotatex = min(80.0f, rotatex);}
    }
    if (shotClock || replayMode) { //fix camera when shooting or replaying
        rotatey = -40;
        rotatex = 0;
    }
    
	//glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color buffer (sets everything to black)

    // Add Lights
    GLfloat light_ambient[]  = {  0.2,  0.2,  0.2,  1.0 };
    GLfloat light_diffuse[]  = {  1.0,  1.0,  1.0,  1.0 };
    GLfloat light_specular[] = {  0.5,  0.5,  0.5,  1.0 };
    GLfloat light_position0[] = { 0.0,  10.0,  10.0, 0.0};
    GLfloat light_position1[] = {  0.0, 10.0, -10.0, 0.0};
    GLfloat light_position2[] = {  10.0, 10.0,  10.0, 0.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
    
    glEnable(GL_LIGHTING); // enable lighting
    glEnable(GL_LIGHT0); // enable light 0
    glEnable(GL_LIGHT1);
    //glEnable(GL_LIGHT2);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LEQUAL);    
	glEnable(GL_DEPTH_TEST);	

    glMatrixMode(GL_PROJECTION);	
	
    //load video
    if(!videoMode)
    {
        video->cleanup();
        free(video);
        if(currentCharacter == 1) video = new videoDecode("videos/hulk.mp4");
        else if(currentCharacter == 2) video = new videoDecode("videos/pikachu.mp4");
        else if(currentCharacter == 3) video = new videoDecode("videos/dbz.mp4");
        else if(currentCharacter == 4) video = new videoDecode("videos/naruto.mp4");
        else if(currentCharacter == 5) video = new videoDecode("videos/joker.mp4");
        else if(currentCharacter == 6) video = new videoDecode("videos/spongebob.mp4");
        else if(currentCharacter == 7) video = new videoDecode("videos/mario.mp4");
        else if(currentCharacter == 8) video = new videoDecode("videos/calbear.mp4");
        videoMode = true;
	}
    
    if(printGoal) {
        video->cleanup();
        free(video);
        video = new videoDecode("videos/goal.mp4");
    }
    
    if (!replayMode) {
        printGoal = false;
    }

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluLookAt(0, 0, -1, 0, 0, 0, 0, 1, 0);

	currentDisplay->draw();

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, viewport.w/viewport.h, 1, 100);
	glRotatef(rotatey, 0, 1, 0);
    
	if(!replayMode) gluLookAt(ball.pos.getX(), 3, -5 + ball.pos.getZ(), 2.5, 3, ball.pos.getZ(), 0, 1, 0);
    else gluLookAt(-11, 3, -5, 0, 3, 0, 0, 1, 0);
    
	glPopMatrix();
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(videoMode)
        {
		if(video->finished()) videoMode = false;
		glPushMatrix();
		float scale = 2/(float)video->getWidth();
        
		float shifty = (scale * (float)(video->getHeight()))/2;
		
        glTranslatef(14,18,18);
        glRotatef(180, 0, 1, 0);
        glScalef(0.02,0.025,1);
		
        video->getNextFrame();
		glPopMatrix();
        }

	{if(keeperpos > 11) keeperpos = 11;}
	{if(keeperpos < 4) keeperpos = 4;}
    
    //set default values
    currentstats.left = keeperpos + 0.5;
    currentstats.right = keeperpos - 0.5;
    
   
    //draw stadium
	glPushMatrix();
	glRotatef(10,0,1,0);
	glScalef(0.5,0.5,0.5);
	stadium->draw();
	glPopMatrix();
  
    
    //draw goal post
	glPushMatrix();
    glTranslatef(7.25,0,2.6);
	glScalef(1,1.5,1);
    post->draw();
    glTranslatef(0,0,2.1);
    post->draw();
    glPopMatrix();
    
    ////////////////////////
    ///replay mode !!!!///
    /////////////////////
    float repkeeperpos, repballrot;
    Vector repballpos;
    int status = 0;
    
    if(replayMode)
    {
        status = rep->playNextFrame(repkeeperpos,repballpos,repballrot);
        if(status) keeperpos = repkeeperpos;
        if(!status) replayMode = false;
    }
    
    
	//draw goalkeeper
	glPushMatrix();
	if(currentCharacter == 1) //Hulk
		{
		glTranslatef(keeperpos,yPos,1);
		glRotatef(180, 0, 1, 0);
            
        if(activatedSpecial) //Hulk grows in size
            {
            glScalef(0.8,0.8,0.8);
            currentstats.height = 4.3;
            currentstats.left = keeperpos + 1;
            currentstats.right = keeperpos - 1;
            }
        else
            {
            glScalef(0.45,0.45,0.45);
            resetStats(currentCharacter);
            }
            
        hulk->draw();
		}
	if(currentCharacter == 2) //Pikachu
		{
		glTranslatef(keeperpos,yPos,1);
		glRotatef(180, 0, 1, 0);
		glScalef(0.2,0.3,0.3);
            
        if(activatedSpecial) //Pikachu gains speed
            {
            currentstats.movespeed = 0.9;
            }
        else
            {
            resetStats(currentCharacter);
            }
            
        pikachu->draw();
		}
	if(currentCharacter == 3) //Goku
		{
		glTranslatef(keeperpos,1.2 + yPos,1);
		glRotatef(180, 0, 1, 0);
		glScalef(0.005,0.005,0.005);
            
        if (activatedSpecial) //ball slows down
            {
            ballSpeed = 13.0f;
            }
        else
            {
            resetStats(currentCharacter);
            }
            
        goku->draw();
		}
	if(currentCharacter == 4) //Naruto
		{
		glTranslatef(keeperpos,yPos,1);
		glRotatef(180, 0, 1, 0);
		glRotatef(-90, 1, 0, 0);
		glScalef(0.01,0.01,0.01);

		naruto->draw();

		if(activatedSpecial) //multiple goalkeepers
			{
			glPopMatrix();

			glPushMatrix();
			glTranslatef(keeperpos-2,yPos,1);
			glRotatef(180, 0, 1, 0);
			glRotatef(-90, 1, 0, 0);
			glScalef(0.01,0.01,0.01);
			naruto->draw();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(keeperpos+2,yPos,1);
			glRotatef(180, 0, 1, 0);
			glRotatef(-90, 1, 0, 0);
			glScalef(0.01,0.01,0.01);
			naruto->draw();
                
            keeperNaruto1->setSurface(narutostats.height, keeperpos-2 + 0.5, keeperpos-2 - 0.5, Vector(keeperpos-2, yPos, 2));
            keeperNaruto2->setSurface(narutostats.height, keeperpos+2 + 0.5, keeperpos+2 - 0.5, Vector(keeperpos+2, yPos, 2));
                
			}
        else
            {
            resetStats(currentCharacter);
            }
		}

	if(currentCharacter == 5) //Joker
		{
		glTranslatef(keeperpos,yPos,1);
		glRotatef(180, 0, 1, 0);
		glScalef(0.5,0.5,0.5);
            
        if (activatedSpecial) //strength bar speeds up
            {
            strengthStep = 25;
            }
        else
            {
            resetStats(currentCharacter);
            }
            
        joker->draw();
		}

	if(currentCharacter == 6) //Spongebob
		{
		glTranslatef(keeperpos + 0.12,yPos,1);
		glRotatef(180, 0, 1, 0);
		glScalef(1.5,1.5,1.5);
            
        if (activatedSpecial) //spazz
            {
            spazz = true;
            }
        else
            {
            resetStats(currentCharacter);
            }
            
        spongebob->draw();
		}
    if(currentCharacter == 7)
        {
        glTranslatef(keeperpos-0.9,yPos,1.35);
        glRotatef(180, 0, 1, 0);
        if(activatedSpecial) //Mario grows in size
        {
            glTranslatef(0.9,0,0);
            glScalef(0.025,0.025,0.025);
            currentstats.height = 4;
            currentstats.left = keeperpos + 1;
            currentstats.right = keeperpos - 1;
        }
        else
        {
            glScalef(0.013,0.013,0.013);
            resetStats(currentCharacter);
        }            
        mario->draw();
        }
	if(currentCharacter == 8) //Obeseman
		{
		glTranslatef(keeperpos,yPos,1);
		glRotatef(180, 0, 1, 0);
            
        if(activatedSpecial) //become smaller
            {
            glScalef(0.05,0.03,0.03);
            currentstats.height *= 0.3;
            currentstats.left = keeperpos + 0.03;
            currentstats.right = keeperpos - 0.03;
            }
        else
            {
            glScalef(0.15,0.1,0.1);
            resetStats(currentCharacter);
            }
            
        obeseman->draw();
		}

	glPopMatrix();
    
    //jumping behavior
    if (spazz) {
        if (yPos < currentstats.jumpHeight) {yPos += 0.1f;}
        else {yPos = 1.0f;}
    } else if (!jump){
        yPos = 1.0f;
    }
    
    if (jump) {
        if (yPos < currentstats.jumpHeight) {yPos += 0.1f;}
        else { yPos = 1.0f; jump = false;}
    }
    
    //update keeper surface
    keeper->setSurface(currentstats.height + yPos - 1, currentstats.left, currentstats.right, Vector(keeperpos, yPos, 1));
    
    //If Hulk, draw hands
    if (currentCharacter == 1)
    {
        keeperHands->setSurface(keeper->height - 0.3, keeper->left + (keeper->height-2.1) / 1.3, keeper->right - (keeper->height-2.1) / 1.3, Vector(keeperpos, yPos, 1), keeper->height - 0.8);
    }
    
    //draw wall
//    glColor3f(1, 1, 1);
//    glBegin(GL_POLYGON);
//    glVertex3f(keeper->left, 0, 1);
//    glVertex3f(keeper->left, keeper->height, 1);
//    glVertex3f(keeper->right, keeper->height, 1);
//    glVertex3f(keeper->right, 0, 1);
//    glEnd();
//    
//    glColor3f(1, 1, 1);
//    glBegin(GL_POLYGON);
//    glVertex3f(keeperHands->left, keeperHands->bottom, 1);
//    glVertex3f(keeperHands->left, keeperHands->height, 1);
//    glVertex3f(keeperHands->right, keeperHands->height, 1);
//    glVertex3f(keeperHands->right, keeperHands->bottom, 1);
//    glEnd();
    
    //draw ball
	glPushMatrix();
    glTranslatef(0, 0.18, 0);
    float theta = -10.0f/180.0f*3.141592;
    Vector centerY = Vector((ball.center.x - 40.7986) * 0.2, (ball.center.y - 3) * 0.2, (ball.center.z + 134.194) * 0.2);
    Vector rotY = Vector(centerY.getX() * cos(theta) + centerY.getZ() * -sin(theta), centerY.getY(), centerY.getX() * sin(theta) + centerY.getZ() * cos(theta));
    ball.pos = Vector(7.35 + rotY.getX(), 0.75 + rotY.getY(), -5.5 + rotY.getZ());
    if(replayMode && status)
        {
        glTranslatef(repballpos.getX(), repballpos.getY(), repballpos.getZ());
        glRotatef(repballrot, 1, 0, 0);
        }
    else
        {
        glTranslatef(ball.pos.getX(), ball.pos.getY(), ball.pos.getZ());
        glRotatef(ball.rotation, 1, 0, 0);
        }
    glTranslatef(0, -0.18, 0);
    glScalef(0.005,0.005,0.005);
	football->draw();
	glPopMatrix();
    
    //update cloth and ball
	cloth.update();
    c.afterCollision(&ball, *keeper);
    c.afterCollision(&ball, *keeperHands);
    c.afterCollision(&ball, *keeperNaruto1);
    c.afterCollision(&ball, *keeperNaruto2);
    c.afterCollision(&ball, *topPost);
    c.afterCollision(&ball, *leftPost);
    c.afterCollision(&ball, *rightPost);
    c.afterCollision(&ball, *backWall);

    c.rotationUpdate(&ball);
    ball.update();
    
    //handle stationery ball or out of bounds
    if(shotTime > 5000 || (ball.v.getZ() > 0.05 && ball.v.getZ() < 0.10) || ball.pos.getZ() > 12) {
        if(rep!=NULL)
        {
            if(startRecording==true)
            {
                startRecording = false;
                replayMode = true;
            }
        }
        shotTime = 0;
        shotClock = false;
        resetBall();
    }
    
    //handle score
    if(ball.pos.getZ() < 4.2 && ball.pos.getZ() > 3.5 //crosses the line
       && ball.pos.getY() < 2.8 //below bar
       && ball.pos.getX() > 4 && ball.pos.getX() < 11) { //between bars
        scored = true;
    }

    //draw arrow
    if (drawarrow) {
        glPushMatrix();
        glTranslatef(ballpos, 0.75, -4);
        glRotatef(-rotateydir-30, 0, 1, 0);
        glTranslatef(0, 0, 2);
        
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.02, 0, -2);
        glVertex3f(-0.02, 0, 0);
        glVertex3f(0.02, 0, 0);
        glVertex3f(0.02, 0, -2);
        glEnd();
        
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-0.1, 0, 0);
        glVertex3f(0.1, 0, 0);
        glVertex3f(0, 0, 2);
        glEnd();
        
        glPopMatrix();
    }

	glPushMatrix();
	glTranslatef(4,0.5,22);
	glRotatef(10,0,1,0);
	glScaled(0.2, 0.2, 0.2);
    draw();
    drawBar(strength);
	glPopMatrix();

	strength += strengthStep;
	if (strength > strengthMax) strength = strengthMin;
		
	glPopMatrix();
    
    // shots left
    glPushMatrix();
    int i = 3 - shotNumber;
    if (shotNumber < 5) {
        glTranslatef(6.6, 0.75, -7.5);
        glScalef(0.005,0.005,0.005);
        football->draw();
    }
    while (i >= 0) {
        glScalef(200, 200, 200);
        glTranslatef(0.5, 0, 0);
        glScalef(0.005,0.005,0.005);
        football->draw();
        i -= 1;
    }
    glPopMatrix();
    
    //Orthogonal Projection
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.0, 10.0);
    glDisable(GL_LIGHTING); //for drawing text
    
    //cheap hack
    Vector ballpos = ball.pos;
    float ballrot = ball.rotation;
    
    //print text
    stringstream ss;
    ss << shotNumber;
    string s = ss.str();
    s = "Shots Left: ";
    //s = s + " out of 5";
    char *a = new char[s.size()+1];
    a[s.size()] = 0;
    memcpy(a,s.c_str(),s.size());
    
    stringstream ss2;
    ss2 << score;
    s = ss2.str();
    s = "Score: " + s;;
    char *b = new char[s.size()+1];
    b[s.size()] = 0;
    memcpy(b,s.c_str(),s.size());
    
    s = "Final " + s;
    char *c = new char[s.size()+1];
    c[s.size()] = 0;
    memcpy(c,s.c_str(),s.size());
    
    glColor3f(1.0,1.0,1.0);
    //drawText(3.4, 0.6, a); //shots left
    drawText(26.0, 0.6, b); //score
    if (gameOver) {
        drawarrow = false;
        glColor3f(0.0,1.0,0.0);
        drawText(12.5, 2.0, "Game Over! Press r to reset!");
        drawText(14, 3.0, c);

    }
    if (specialDisabled) {
        glColor3f(1.0,1.0,1.0);
        drawText(3, 0.6, "You have used your");
        drawText(3.7, 1, "special power");

    }
    
    
	glFlush();
	glutSwapBuffers();
    
    //record replay
    if(startRecording)
    {
        rep->addFrame(keeperpos, ballpos, ballrot);
    }
    
    
    //some frame control thingy
    gettimeofday(&tEnd, NULL);
    double diff = tEnd.tv_usec - tStart.tv_usec;
    //if(diff > 0 && diff < 30000) usleep(30000 - diff);
}

void myDisplay() {
    if (isMenu) {
        menu.draw();
    }
    else {
        Playing();
    }
}

//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() 
	{
		//nothing here for now
	#ifdef _WIN32
		Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
	#endif
		glutPostRedisplay(); // forces glut to call the display function (myDisplay())
	}

void mousemove(int x, int y) {
	if (isMenu) {
        double w = glutGet(GLUT_WINDOW_WIDTH);
        double h = glutGet(GLUT_WINDOW_HEIGHT);
        double x1 = 2 * (x / w) - 1;
        double y1 = -2 * (y / h) + 1;
        for (int i = 0; i < 4; i++) {
            if (x1 >= -0.2 && x1 <= 0.2 && y1 >= -0.3 - i * 0.15 && y1 <= -0.2 - i * 0.15) {
                menu.setMover(i);
            } else {
                menu.setMout(i);
            }
        }       
    } else {
    	if(x < 150) {mousex = 1;}
    	else if(x > viewport.w - 150) {mousex = 2;}
	    else {mousex = 0;}
	    if(y < 150) {mousey = 1;}
    	else if(y > viewport.h - 150) {mousey = 2;}
    	else {mousey = 0;}
    }
}

void press(unsigned char k, int x, int y)
	{
	switch(k)
		{
		case 'q':
			video->cleanup();
			delete currentDisplay;
			//free(video);
			exit(0);
            break;
        case ' ': //shoot
            if (replayMode) {
                replayMode = false;
            } else {
                if (!keyDisabled) {
                    if (!gameOver) {
                        ball.v = c.rotateBall(Vector(0, 25 * strength/strengthMax, ballSpeed * strength/strengthMax), -rotateydir - 40); //Velocity
                        shotClock = true;
                        shotTime = 0;
                        //starting recording
                        printGoal = false;
                         delete rep;
                        rep = new replay();
                        startRecording = true;
                    }
                }
                keyDisabled = true;
            }
            break;
		case 'a': //move keeper left
			keeperpos += currentstats.movespeed;
			break;

		case 'd': //move keeper right
			keeperpos -= currentstats.movespeed;
			break;

        case 'r':
            resetBall();
            specialDisabled = false;
            gameOver = false;
            printGoal = false;
            shotNumber = 1;
            score = 0;
			break;

        case 's': //activate special
            if (activatedSpecial) {
                activatedSpecial = false;
            } else {
                if (!specialDisabled) {
                    activatedSpecial = true;
                }
                specialDisabled = true;
            }
			break;
        case 'w': //jump
            jump = true;
            break;

		case '1':
            resetStats(currentCharacter);
			activatedSpecial = false;
			videoMode = false;
			currentCharacter = 1;
			currentstats = hulkstats;
			delete currentDisplay;
			currentDisplay = new gameInterface("gui/images/hulk.jpg");
			break;

		case '2':
            resetStats(currentCharacter);
			activatedSpecial = false;
			videoMode = false;
			currentCharacter = 2;
			currentstats = pikachustats;
			delete currentDisplay;
			currentDisplay = new gameInterface("gui/images/pikachu.jpg");
			break;

		case '3':
            resetStats(currentCharacter);
			activatedSpecial = false;
			videoMode = false;
			currentCharacter = 3;
			currentstats = gokustats;
			delete currentDisplay;
			currentDisplay = new gameInterface("gui/images/goku.jpg");
			break;

		case '4':
            resetStats(currentCharacter);
			activatedSpecial = false;
			videoMode = false;
			currentCharacter = 4;
			currentstats = narutostats;
			delete currentDisplay;
			currentDisplay = new gameInterface("gui/images/naruto.jpg");
			break;

		case '5':
            resetStats(currentCharacter);
			activatedSpecial = false;
			videoMode = false;
			currentCharacter = 5;
			currentstats = jokerstats;
			delete currentDisplay;
			currentDisplay = new gameInterface("gui/images/joker.jpg");
			break;

		case '6':
            resetStats(currentCharacter);
			activatedSpecial = false;
			videoMode = false;
			currentCharacter = 6;
			currentstats = spongebobstats;
			delete currentDisplay;
			currentDisplay = new gameInterface("gui/images/spongebob.jpg");
			break;

		case '7':
            resetStats(currentCharacter);
			activatedSpecial = false;
			videoMode = false;
			currentCharacter = 7;
			currentstats = mariostats;
			delete currentDisplay;
			currentDisplay = new gameInterface("gui/images/mario.jpg");
			break;
        case '8':
            resetStats(currentCharacter);
            activatedSpecial = false;
            videoMode = false;
            currentCharacter = 8;
            currentstats = obesemanstats;
            delete currentDisplay;
            currentDisplay = new gameInterface("gui/images/obeseman.jpg");
            break;
		}
	}

void SpecialInput(int key, int x, int y)
	{
	switch(key)
		{
		case GLUT_KEY_UP:
			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) translatey += 1;
			else rotatex-=10;
            break;
            
		case GLUT_KEY_DOWN:
			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) translatey -= 1;
			else rotatex+=10;
            break;
            
		case GLUT_KEY_LEFT:
			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) translatex -= 1;
            else rotateydir-=1;
            break;
            
		case GLUT_KEY_RIGHT:
			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) translatex += 1;
            else rotateydir+=1;
            break;

		case GLUT_KEY_PAGE_UP:
			rotatez += 10;
			break;

		case GLUT_KEY_PAGE_DOWN:
			rotatez -= 10;
			break;
		}
	}

int initX;
int curX;
void myMouse(int button, int state, int x, int y) 
{
    if (isMenu) {
        double w = glutGet(GLUT_WINDOW_WIDTH);
        double h = glutGet(GLUT_WINDOW_HEIGHT);
        double x1 = 2 * (x / w) - 1;
        double y1 = -2 * (y / h) + 1;
        if (button == GLUT_LEFT_BUTTON) {
            if (x1 >= -0.2 && x1 <= 0.2 && y1 >= -0.3 && y1 <= -0.2 && menu.getWindow() != 3) {
                switch (state) {
                case GLUT_DOWN:
                    menu.setButton(0);
                    break;
                case GLUT_UP:
                    menu.releaseButton(0);
                    if (menu.getWindow() == 1) { isMenu = false;}
                    if (menu.getWindow() == 2) { isMenu = false;}
                    break;

                }
            }

            if (x1 >= -0.2 && x1 <= 0.2 && y1 >= -0.45 && y1 <= -0.35 && menu.getWindow() != 3) {
                switch (state) {
                case GLUT_DOWN:
                    menu.setButton(1);
                    break;
                case GLUT_UP:
                    menu.releaseButton(1);
                    if (menu.getWindow() == 1) { isMenu = false; }
                    if (menu.getWindow() == 2) { menu.setWindow(1);}
                    break;

                }
            }

            if (x1 >= -0.2 && x1 <= 0.2 && y1 >= -0.6 && y1 <= -0.5 && menu.getWindow() != 3) {
                switch (state) {
                case GLUT_DOWN:
                    menu.setButton(2);
                    break;
                case GLUT_UP:
                    menu.releaseButton(2);
                    exit(0);
                    break;

                }
            }

            if (x1 >= -0.2 && x1 <= 0.2 && y1 >= -0.75 && y1 <= -0.65) {
                switch (state) {
                case GLUT_DOWN:
                    menu.setButton(3);
                    break;
                case GLUT_UP:
                    menu.releaseButton(3);
                    if (menu.getWindow() == 3) { menu.setToPreviousWindow(); }
                    else { menu.setWindow(3);}
                    break;

                }
            }
        }
    }
}


void myMotionFunc(int x, int y)
	{
    ball.center += orientX * (x - curX) / 10;
    curX = x;;
	}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[])
{
	//This initializes glut
	glutInit(&argc, argv);
    
	//This tells glut to use a double-buffered window with red, green, and blue channels
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Initalize theviewport size
	viewport.w = 1280;
	viewport.h = 800;
    
	//The size and position of the window
	glutInitWindowSize(viewport.w, viewport.h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Soccer Game");
	glutFullScreen();
    
	initScene();                                 // quick function to set up scene
    
	cout<<"Loading intro video..."<<endl;
	video = new videoDecode("videos/hulk.mp4");
	currentDisplay = new gameInterface("gui/images/hulk.jpg");
    topBar = new gameInterface("gui/images/topbar.jpg");
    
	cout<<"Loading scene object files..."<<endl;
	//load i/Users/ssfei81/Desktop/gab cs184/main.cppn obj files
	object sta("stadium.obj","objects/stadium/");
	object ball("Football.obj", "objects/ball/");
	object hu("Hulk.obj", "objects/hulk/"); 
	object pika("pikachu.obj", "objects/pikachu/");
	object go("Songoku.obj", "objects/goku/");
	object naru("naruto.obj", "objects/naruto/");
	object jo("Joker.obj", "objects/joker/");
	object bob("spongebob.obj", "objects/spongebob/");
    object mar("mario_obj.obj", "objects/mario/");
	object obs("obese_male.obj", "objects/obeseman/");
    object po("post.obj", "objects/post/");

	stadium = &sta;
	football = &ball;
	hulk = &hu;
	pikachu = &pika;
	goku = &go;
	naruto = &naru;
	joker = &jo;
	spongebob = &bob;
    mario = &mar;
	obeseman = &obs;
    post = &po;

	//keyboard and mouse
	glutSpecialFunc(SpecialInput);
	glutKeyboardFunc(press); 
	glutPassiveMotionFunc(mousemove);
	glutMouseFunc(myMouse);
    
	glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
	glutReshapeFunc(myReshape);                  // function to run when the window gets resized
	glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
	glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

	return 0;
}








