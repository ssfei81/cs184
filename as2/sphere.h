#include <iostream>
#include "ray.h"

using namespace std;

//auto converts to unit sphereor
class sphere
	{
	private:
		vect center;
        float radius;
	
	public:	
		sphere(); //default constructor
		sphere(vect, float); //constructor
		
		//set / get methods
		vect getCenter();
		float getRadius();
		vect getNormal(vect);
        float getIntersection(ray);
	};