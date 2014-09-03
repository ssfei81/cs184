#ifndef _RAY_H_
#define _RAY_H_

#include "vect.h"

//auto converts to unit ray
class ray
	{
	private:
		vect point, direction;
		float tmin, tmax;
	
	public:	
		ray() //default constructor
			{
			this -> point = vect(0,0,0,1);
			this -> direction = vect(1,0,0,0);
			}
		ray(vect p, vect d, float tmin, float tmax) //constructor
			{
			this -> point = p;
			this -> direction = d.getUnitVector();
			this -> tmin = tmin;
			this -> tmax = tmax;
			}
		//set / get methods
		vect getPoint() {return this-> point;}
		vect getDirection() {return this-> direction;}
		float getTmin() {return this->tmin;}
		float getTmax() {return this->tmax;}
		void setTmax(float tmax ) {this->tmax = tmax;}
	};

#endif