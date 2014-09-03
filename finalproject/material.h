#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif

#include "vect.h"
#include "FreeImage.h"
#include <iostream>
using namespace std;

class material
	{
	private:
	float ns, ni, d;
	int illum;
	Vector ka, kd, ks;
	GLuint texture;

	public:
	material()
		{
		ns = 0; ni = 0; d = 0; illum = 0; 
		ka = Vector(0,0,0); kd = Vector(0,0,0); ks = Vector(0,0,0);
		texture = 0;
		}
	material(float ns, float ni, float d, int illum, Vector ka, Vector kd, Vector ks)
		{
		this -> ns = ns; this -> ni = ni; this -> d = d;
		this -> illum = illum;
		this -> ka = ka; this -> kd = kd; this -> ks = ks;
		texture = 0;
		}

	//get variables
	float getNs() {return this -> ns;}
	float getNi() {return this -> ni;}
	float getD() {return this -> d;}
	int getIllum() {return this -> illum;}
	Vector getKa() {return this -> ka;}
	Vector getKd() {return this -> kd;}
	Vector getKs() {return this -> ks;}
	
	//set variables
	void setNs(float ns) {this -> ns = ns;}
	void setNi(float ni) {this -> ni = ni;}
	void setD(float d) {this -> d = d;}
	void setIllum(int illum) {this -> illum = illum;}
	void setKa(Vector ka) {this -> ka = ka;}
	void setKd(Vector kd) {this -> kd = kd;}
	void setKs(Vector ks) {this -> ks = ks;}

	void setKdMap(string imgUrl)
		{
		if(!this->texture)
			{
			FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(imgUrl.c_str(), 0),imgUrl.c_str());
			FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);
			int nWidth = FreeImage_GetWidth(pImage);
			int nHeight = FreeImage_GetHeight(pImage);

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
			FreeImage_Unload(bitmap);
			FreeImage_Unload(pImage);
			}
		}
	GLuint getKdMap() {return this->texture;}
	};
#endif