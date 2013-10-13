#ifndef _RAY_H_
#define _RAY_H_

#include "vect.h"

//auto converts to unit ray
class ray
	{
	private:
		vect point, direction;
	
	public:	
		ray(); //default constructor
		ray(vect, vect); //constructor
		
		//set / get methods
		vect getPoint();
		vect getDirection();
	};

#endif