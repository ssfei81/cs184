#ifndef _FILM_H_
#define _FILM_H_

#include "vect.h"
#include <cmath>

//auto converts to unit ray
class film
	{
	private:
		float width, height,fov, fovh, ppux, ppuy; //pixel per unit
		vect origin;
	public:	
		film(); //default constructor
		film(float, float,float,vect); //constructor
		
		//set / get methods
		float getWidth();
		float getHeight();
		float getFov();
		vect getPoint(float x, float y);
	};

#endif