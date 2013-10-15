#ifndef _RAY_H_
#define _RAY_H_

#include "vect.h"

//auto converts to unit ray
class ray
	{
	private:
		vect point, direction;
		float t_min, t_max;
	
	public:	
		ray() //default constructor
			{
			//initialize ray to (0,0,0)
			this -> point = vect(0,0,0,1);
			this -> direction = vect(1,0,0,0);
			}
		ray(vect p, vect d) //constructor
			{
			this -> point = p;
			this -> direction = d.getUnitVector();
			}
		//set / get methods
		vect getPoint() {return this-> point;}
		vect getDirection() {return this-> direction;}
	};

#endif