#include "ray.h"

ray::ray()
	{
	//initialize ray to (0,0,0)
	this -> point = vect(0,0,0,1);
	this -> direction = vect(1,0,0,0);
	}

ray::ray(vect p, vect d)
	{
	this -> point = p;
	this -> direction = d.getUnitVector();
	}

vect ray::getPoint()
	{
	return this->point;
	}
vect ray::getDirection()
	{
	return this->direction;
	}

