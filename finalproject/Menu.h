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

#include <string>
#include <cstdlib>
#include "freeImage.h"
#include <iostream>

using namespace std;

class Menu {
public:
	void draw() {

		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 500.0);
		gluLookAt(0, 0, 1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		switch(window) {
			case 1:
				mainMenu();
				break;
			case 2:
				pauseMenu();
				break;
			case 3:
				info();
				break;
			case 4:
				selectCharacter();
				break;
            case 5:
                playerMenu();
			default:
				break;
		}
        glPopMatrix();
		glFlush();
		glutSwapBuffers();
	}

	void setMover(int button) {
		color[button][0] = 0;
		color[button][1] = 0.8;
		color[button][2] = 0;
	}
	void setMout(int button) {
		color[button][0] = 1;
		color[button][1] = 1;
		color[button][2] = 1;
	}
	void setButton(int button) {
		color[button][0] = 0;
		color[button][1] = 0.5;
		color[button][2] = 0;
	}

	void releaseButton(int button) {
		color[button][0] = 1;
		color[button][1] = 1;
		color[button][2] = 1;
	}
	void setWindow(int w) {
		previousW = window;
		window = w;
	}
	int getWindow() {
		return window;
	}
	int setToPreviousWindow() {
		window = previousW;
	}

private:
	void loadImage(string imgUrl, GLuint& texture) {

		int nWidth, nHeight;

		FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(imgUrl.c_str(), 0),imgUrl.c_str());
		FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);
		nWidth = FreeImage_GetWidth(pImage);
		nHeight = FreeImage_GetHeight(pImage);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
		FreeImage_Unload(bitmap);
		FreeImage_Unload(pImage);
	}
    //void selectCharacter() {
	//void pauseMenu() {
	void mainMenu() {
		GLuint button;
		GLuint background;
		loadImage("button.jpg", button);
		loadImage("background0.jpg", background);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);

/****************** draw background *****************/
		glColor3f(1.0, 1.0, 1.0);

		glBindTexture(GL_TEXTURE_2D, background);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex2f(-1, -1);

		glTexCoord2f(0, 1);
		glVertex2f(-1, 1);

		glTexCoord2f(1, 1);
		glVertex2f(1, 1);

		glTexCoord2f(1, 0);
		glVertex2f(1, -1);
		glEnd();

/********** draw four retangulars **************/
		glBindTexture(GL_TEXTURE_2D, button);
		for (int i = 0; i < 4; i++) {
			glColor3f(color[i][0], color[i][1], color[i][2]);
			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-0.2, -0.2 - i*0.15, 0.1);

			glTexCoord2f(0, 1);
			glVertex3f(-0.2, -0.3 - i*0.15, 0.1);

			glTexCoord2f(1, 1);
			glVertex3f(0.2, -0.3 - i*0.15, 0.1);

			glTexCoord2f(1, 0);
			glVertex3f(0.2, -0.2 - i*0.15, 0.1);
			glEnd();
		}


		glDisable(GL_TEXTURE_2D);
		glDisable(GL_COLOR_MATERIAL);

/*************** write text *******************/
	    glColor3f(0.0f, 0.0f, 0.0f);
	    glLineWidth(4);
	    glPushMatrix();
	    glTranslatef(-0.18, -0.28, 1);
		glScalef(0.0005,0.0005,0.0005);
		strcpy(line, "Start Game");
	    for (int i = 0; i < strlen(line); i++) {
	    	glutStrokeCharacter(GLUT_STROKE_ROMAN, line[i]);
	    }
	    glPopMatrix();

	    glPushMatrix();
	    glTranslatef(-0.17, -0.43, 1);
		glScalef(0.0005,0.0005,0.0005);
		strcpy(line, "Instructions");
	    for (int i = 0; i < strlen(line); i++) {
	    	glutStrokeCharacter(GLUT_STROKE_ROMAN, line[i]);
	    }
	    glPopMatrix();

	    glPushMatrix();
	    glTranslatef(-0.18, -0.58, 1);
		glScalef(0.0005,0.0005,0.0005);
		strcpy(line, "  Credit");
	    for (int i = 0; i < strlen(line); i++) {
	    	glutStrokeCharacter(GLUT_STROKE_ROMAN, line[i]);
	    }
	    glPopMatrix();

	    glPushMatrix();
	    glTranslatef(-0.17, -0.73, 1);
		glScalef(0.0005,0.0005,0.0005);
		strcpy(line, "  Quit");
	    for (int i = 0; i < strlen(line); i++) {
	    	glutStrokeCharacter(GLUT_STROKE_ROMAN, line[i]);
	    }
	    glPopMatrix();
	}

    void playerMenu() {
        GLuint button;
		GLuint background;
		loadImage("button.jpg", button);
		loadImage("background0.jpg", background);
        
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);
        
        /****************** draw background *****************/
		glColor3f(1.0, 1.0, 1.0);
        
		glBindTexture(GL_TEXTURE_2D, background);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex2f(-1, -1);
        
		glTexCoord2f(0, 1);
		glVertex2f(-1, 1);
        
		glTexCoord2f(1, 1);
		glVertex2f(1, 1);
        
		glTexCoord2f(1, 0);
		glVertex2f(1, -1);
		glEnd();
        
        /********** draw two retangulars **************/
		glBindTexture(GL_TEXTURE_2D, button);
		for (int i = 0; i < 2; i++) {
			glColor3f(color[i][0], color[i][1], color[i][2]);
			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-0.2, -0.2 - i*0.15, 0.1);
            
			glTexCoord2f(0, 1);
			glVertex3f(-0.2, -0.3 - i*0.15, 0.1);
            
			glTexCoord2f(1, 1);
			glVertex3f(0.2, -0.3 - i*0.15, 0.1);
            
			glTexCoord2f(1, 0);
			glVertex3f(0.2, -0.2 - i*0.15, 0.1);
			glEnd();
		}
        
        
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_COLOR_MATERIAL);
        
        /*************** write text *******************/
	    glColor3f(0.0f, 0.0f, 0.0f);
	    glLineWidth(4);
	    glPushMatrix();
	    glTranslatef(-0.18, -0.28, 1);
		glScalef(0.0004,0.0004,0.0004);
		strcpy(line, "Single Player");
	    for (int i = 0; i < strlen(line); i++) {
	    	glutStrokeCharacter(GLUT_STROKE_ROMAN, line[i]);
	    }
	    glPopMatrix();
        
	    glPushMatrix();
	    glTranslatef(-0.17, -0.43, 1);
		glScalef(0.0004,0.0004,0.0004);
		strcpy(line, "Double Player");
	    for (int i = 0; i < strlen(line); i++) {
	    	glutStrokeCharacter(GLUT_STROKE_ROMAN, line[i]);
	    }
	    glPopMatrix();
    }
    
	void pauseMenu() {
		GLuint button;
		GLuint background;
		loadImage("button.jpg", button);
		loadImage("pause.jpg", background);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);

/****************** draw background *****************/
		glColor3f(1.0, 1.0, 1.0);

		glBindTexture(GL_TEXTURE_2D, background);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex2f(-1, -1);

		glTexCoord2f(0, 1);
		glVertex2f(-1, 1);

		glTexCoord2f(1, 1);
		glVertex2f(1, 1);

		glTexCoord2f(1, 0);
		glVertex2f(1, -1);
		glEnd();

/********** draw four retangulars **************/
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, button);
		for (int i = 0; i < 4; i++) {
            glColor3f(color[i][0], color[i][1], color[i][2]);
			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-0.2, -0.2 - i*0.15, 0.1);

			glTexCoord2f(0, 1);
			glVertex3f(-0.2, -0.3 - i*0.15, 0.1);

			glTexCoord2f(1, 1);
			glVertex3f(0.2, -0.3 - i*0.15, 0.1);

			glTexCoord2f(1, 0);
			glVertex3f(0.2, -0.2 - i*0.15, 0.1);
			glEnd();
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_COLOR_MATERIAL);

	/*************** write text *******************/
	    glColor3f(0.0f, 0.0f, 0.0f);
	    glLineWidth(4);
	    glPushMatrix();
	    glTranslatef(-0.13, -0.28, 1);
		glScalef(0.0005,0.0005,0.0005);
		strcpy(line, "Continue");
	    for (int i = 0; i < strlen(line); i++) {
	    	glutStrokeCharacter(GLUT_STROKE_ROMAN, line[i]);
	    }
	    glPopMatrix();

	    glPushMatrix();
	    glTranslatef(-0.17, -0.43, 1);
		glScalef(0.0005,0.0005,0.0005);
		strcpy(line, "Main Menu");
	    for (int i = 0; i < strlen(line); i++) {
	    	glutStrokeCharacter(GLUT_STROKE_ROMAN, line[i]);
	    }
	    glPopMatrix();
	 
	    glPushMatrix();
	    glTranslatef(-0.16, -0.58, 1);
		glScalef(0.0005,0.0005,0.0005);
		strcpy(line, "Instructions");
	    for (int i = 0; i < strlen(line); i++) {
	    	glutStrokeCharacter(GLUT_STROKE_ROMAN, line[i]);
	    }
	    glPopMatrix();

	    glPushMatrix();
	    glTranslatef(-0.19, -0.73, 1);
		glScalef(0.0006,0.0006,0.0006);
		strcpy(line, "  Quit");
	    for (int i = 0; i < strlen(line); i++) {
	    	glutStrokeCharacter(GLUT_STROKE_ROMAN, line[i]);
	    }
	    glPopMatrix();
	}


	void info() {

		GLuint button;
		GLuint background;
		loadImage("button.jpg", button);
		loadImage("tutorial.jpg", background);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);



/********** draw one return button **************/
		glBindTexture(GL_TEXTURE_2D, button);
		glColor3f(color[3][0], color[3][1], color[3][2]);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(-0.2, -0.65, 0.1);

		glTexCoord2f(0, 1);
		glVertex3f(-0.2, -0.75, 0.1);

		glTexCoord2f(1, 1);
		glVertex3f(0.2, -0.75, 0.1);

		glTexCoord2f(1, 0);
		glVertex3f(0.2, -0.65, 0.1);
		glEnd();

/****************** draw background *****************/
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, background);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, 0);

		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 0);

		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 0);

		glTexCoord2f(1, 0);
		glVertex3f(1, -1, 0);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_COLOR_MATERIAL);

/***************** write text *******************/

	    glColor3f(0.0f, 0.0f, 0.0f);
	    glLineWidth(3);
	    glPushMatrix();
	    glTranslatef(-0.18, -0.73, 1);
		glScalef(0.0006,0.0006,0.0006);
		strcpy(line, " Return");
	    for (int i = 0; i < strlen(line); i++) {
	    	glutStrokeCharacter(GLUT_STROKE_ROMAN, line[i]);
	    }
	    glPopMatrix();
	}
	//void mainMenu() {
	void selectCharacter() {
        
		GLuint background;
		loadImage("selectCharacter.jpg", background);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);
        
    	GLuint num[8] = {0};
    	char character[30] = "gui/images/character0.jpg";
    	for (int i = 0; i < 8; i++) {
    		character[20] = i + '0';
    		loadImage(character, num[i]);
    	}

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);
    	glColor3f(1.0, 1.0, 1.0);

    	for (int i = 0; i < 8; i++) {
    		float x = -0.9 + 0.5 * (i % 4);
    		float y = 0.5 - 0.7 * (i / 4);
    		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, num[i]);
	        glBegin(GL_POLYGON);
			glTexCoord2f(0, 1);
			glVertex3f(x, y, 0.1);

			glTexCoord2f(0, 0);
			glVertex3f(x, y - 0.6, 0.1);

			glTexCoord2f(1, 0);
			glVertex3f(x + 0.3, y - 0.6, 0.1);

			glTexCoord2f(1, 1);
			glVertex3f(x + 0.3, y, 0.1);
			glEnd();
			glPopMatrix();
    	}
        
        
        /****************** draw background *****************/
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, background);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, 0);
        
		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 0);
        
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 0);
        
		glTexCoord2f(1, 0);
		glVertex3f(1, -1, 0);
		glEnd();
        
    	glDisable(GL_TEXTURE_2D);
		glDisable(GL_COLOR_MATERIAL);
    }


	int window = 1;
	int previousW = 1;
	char line[100];
	float color[4][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
};

