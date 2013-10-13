#include "sphere.h"
#include "math.h"

sphere::sphere()
	{
	this -> center = vect(0,0,0,1);
	this -> radius = 1;
	}

sphere::sphere(vect c, float r)
	{
	this -> center = c;
	this -> radius = r;
	}

vect sphere::getCenter()
	{
    return this -> center;
	}

float sphere::getRadius()
	{
    return this -> radius;
	}

vect sphere::getNormal(vect point)
    {
    return point - this -> center;
    }

float sphere::getIntersection(ray r)
    {
    //coefficients
    float a = 1;
    float b = 2 * (r.getDirection() ^ (r.getPoint() - this->getCenter()));
    float c = ((r.getPoint()-this->getCenter()) ^ (r.getPoint()-this->getCenter())) - (this->radius * this->radius);
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) return -1;
	
        
    float t1 = (-1*b + sqrt(discriminant)) / (2 * a);
    float t0 = (-1*b - sqrt(discriminant)) / (2 * a);

    if (t0 > 0) return t0;
    else return t1;
    }

