#ifndef _SPHERE_H_
#define _SPHERE_H_


#include <iostream>
#include "ray.h"
using namespace std;



//auto converts to unit sphere
class sphere
	{
	private:
		vect center;
        float radius;
	
	public:	
		sphere() //default constructor
			{
			this -> center = vect(0,0,-5,1);
			this -> radius = 1;
			}
		sphere(vect c, float r) //constructor
			{
			this -> center = c;
			this -> radius = r;
			}
		//set / get methods
		vect getCenter() {return this->center;}
		float getRadius() {return this->radius;}
		vect getNormal(vect point) {return point - this -> center;}

        bool getIntersection(ray &r,float *thit, vect *intersect)
			{		
			//coefficients
			float a = 1;
			float b = 2 * (r.getDirection() ^ (r.getPoint() - this->getCenter()));
			float c = ((r.getPoint()-this->getCenter()) ^ (r.getPoint()-this->getCenter())) - (this->radius * this->radius);
			float discriminant = b * b - 4 * a * c;
    
			if (discriminant < 0) return false;
	
        
			float t1 = (-1*b + sqrt(discriminant)) / (2 * a);
			float t0 = (-1*b - sqrt(discriminant)) / (2 * a);

			if (t0 > 0) 
				{
				*thit = t0;
				*intersect = r.getPoint() + t0 * r.getDirection();
				return true;
				}
			else 
				{
				*thit = t1;
				*intersect = r.getPoint() + t1 * r.getDirection();
				return true;
				}
			}
	};

#endif