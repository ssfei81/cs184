#ifndef _BBOX_H_
#define _BBOX_H_

#include "ray.h"
#include <vector>
using namespace std;

class bbox
	{
	private:
		vect min;
		vect max;


	public:
		bbox()
			{
			this->min = vect(0,0,0,0);
			this->max = vect(0,0,0,0);
			}
		bbox(vect min, vect max)
			{
			this -> min = min;
			this -> max = max;
			}	
		vect getMin() {return this->min;}
		vect getMax() {return this->max;}

		bool intersect(ray &r)
			{
			/*
			intersection algorithm
			thanks to
			http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-box-intersection/
			*/
			float tmin = (this->getMin().getX() - r.getPoint().getX()) / r.getDirection().getX();
			float tmax = (this->getMax().getX() - r.getPoint().getX()) / r.getDirection().getX();
			if (tmin > tmax) swap(tmin, tmax);
			float tymin = (this->getMin().getY() - r.getPoint().getY()) / r.getDirection().getY();
			float tymax = (this->getMax().getY() - r.getPoint().getY()) / r.getDirection().getY();
			if (tymin > tymax) swap(tymin, tymax);
			if ((tmin > tymax) || (tymin > tmax))
				return false;
			if (tymin > tmin)
				tmin = tymin;
			if (tymax < tmax)
				tmax = tymax;
			float tzmin = (this->getMin().getZ() - r.getPoint().getZ()) / r.getDirection().getZ();
			float tzmax = (this->getMax().getZ() - r.getPoint().getZ()) / r.getDirection().getZ();
			if (tzmin > tzmax) swap(tzmin, tzmax);
			if ((tmin > tzmax) || (tzmin > tmax))
				return false;
			if (tzmin > tmin)
				tmin = tzmin;
			if (tzmax < tmax)
				tmax = tzmax;
			if ((tmin > r.getTmax()) || (tmax < r.getTmin())) return false;
			return true;
			}
	};

#endif