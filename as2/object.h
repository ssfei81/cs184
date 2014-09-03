#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "sphere.h"
#include "triangle.h"
#include "brdf.h"
#include "matrix.h"
#include "bbox.h"
#include "ray.h"
#include <cfloat>

class object
	{
	private:
		//0 = sphere, 1 = triangle
		int type;
		sphere s;
        triangle t;
		brdf b;
		matrix objToWorld, worldToObj;
		bbox box;
		
	public:	
		object() //default constructor
			{
			//do nothing, ignore
			}
		object(sphere s, brdf b, matrix o2w)
			{
			this -> s = s;
			this -> b = b;
			this -> type = 0;

			//object to origin
			vect center = s.getCenter();
			matrix o2o = matrix().translate(-1*center.getX(), -1*center.getY(), -1*center.getZ());

			this -> objToWorld = o2o.invert()*o2w*o2o;
            this -> worldToObj = this->objToWorld.invert();
			this -> setBbox();
			}
        object(triangle t, brdf b, matrix o2w)
			{
			this -> t = t;
			this -> b = b;
			this -> type = 1;
            this -> objToWorld = o2w;
            this -> worldToObj = this->objToWorld.invert();
			this -> setBbox();
			}

		brdf getBRDF() 
			{
			return this->b;
			}
		
		sphere getSphere() {return this->s;}
        triangle getTriangle() {return this->t;}

		bool getIntersection(ray &r,float *thit, vect *normal, vect *intersect)
			{
            ray oray = this->worldToObj * r;
			//cheap hack for directional light
			if(r.getTmax() == FLT_MAX) oray.setTmax(FLT_MAX);

            float othit;
            vect onormal;
            if(this->type == 0) 
				{
                if (!(s.getIntersection(oray, &othit, &onormal, intersect))) 
					{
                    return false;
					}
				else 
					{
                    *thit = othit;
		
					*normal = (this->worldToObj).transpose() * onormal.getUnitVector();

                    (*intersect).setH(1);

                    *intersect = this->objToWorld * (*intersect);
					*thit = (*intersect-r.getPoint()).getX()/r.getDirection().getX();

					(*intersect).setH(1);
					}
                } 
			else 
				{
                if (!(t.getIntersection(oray, &othit, intersect))) 
					{
                    return false;
					}
				else 
					{
                    *thit = othit;
                    (*intersect).setH(1);
					//normal
					vect v1 = this-> objToWorld * t.getV1();
					vect v2 = this-> objToWorld * t.getV2();
					vect v3 = this-> objToWorld * t.getV3();
					*normal = (v2 - v1) * (v3 - v2);


                    *intersect = this->objToWorld * (*intersect);
					*thit = (*intersect-r.getPoint()).getX()/r.getDirection().getX();
					(*intersect).setH(1);
					}
				}
                return true;
			}
        
        bool intersectP(ray &r)
			{
            ray oray = this->worldToObj * r;
			//cheap hack for directional light
			if(r.getTmax() == FLT_MAX) oray.setTmax(FLT_MAX);

            if(this->type == 0) 
				{
                return s.intersectP(oray);
				}
            else 
				{
				if (this->getBRDF().getrindex()>0) return false;
                return t.intersectP(oray);
				}
			}
        int getType() {return this->type;}
		
		//set / get methods
//        void seto2w(matrix a) {
//            this->objToWorld = a;
//        }
//        void setw2o(matrix a) {
//            this->worldToObj = a;
//        }
        matrix geto2w() {
            return this->objToWorld;
        }
        matrix getw2o() {
            return this->worldToObj;
        }

		void setBbox()
			{
			float minx=FLT_MAX, miny=FLT_MAX, minz=FLT_MAX, maxx = FLT_MIN, maxy = FLT_MIN, maxz = FLT_MIN; 
			if(this->type==0)
				{
				//get eight corners of sphere
				float radius = this->getSphere().getRadius();
				vect center = this->getSphere().getCenter();
				matrix o2w = this->geto2w();

				vect c1 = (center + vect(-radius, -radius, -radius, 1));
				vect c2 = (center + vect(-radius, +radius, -radius, 1));
				vect c3 = (center + vect(+radius, -radius, -radius, 1));
				vect c4 = (center + vect(+radius, +radius, -radius, 1));
				vect c5 = (center + vect(-radius, -radius, +radius, 1));
				vect c6 = (center + vect(-radius, +radius, +radius, 1));
				vect c7 = (center + vect(+radius, -radius, +radius, 1));
				vect c8 = (center + vect(+radius, +radius, +radius, 1));
				c1.setH(1); c2.setH(1); c3.setH(1); c4.setH(1); c5.setH(1); c6.setH(1); c7.setH(1); c8.setH(1);
				c1 = o2w * c1;
				c2 = o2w * c2;
				c3 = o2w * c3;
				c4 = o2w * c4;
				c5 = o2w * c5;
				c6 = o2w * c6;
				c7 = o2w * c7;
				c8 = o2w * c8;


				if(c1.getX() < minx) {minx = c1.getX();} if(c1.getY() < miny) {miny = c1.getY();} if(c1.getZ() < minz) {minz = c1.getZ();}
				if(c2.getX() < minx) {minx = c2.getX();} if(c2.getY() < miny) {miny = c2.getY();} if(c2.getZ() < minz) {minz = c2.getZ();}
				if(c3.getX() < minx) {minx = c3.getX();} if(c3.getY() < miny) {miny = c3.getY();} if(c3.getZ() < minz) {minz = c3.getZ();}
				if(c4.getX() < minx) {minx = c4.getX();} if(c4.getY() < miny) {miny = c4.getY();} if(c4.getZ() < minz) {minz = c4.getZ();}
				if(c5.getX() < minx) {minx = c5.getX();} if(c5.getY() < miny) {miny = c5.getY();} if(c5.getZ() < minz) {minz = c5.getZ();}
				if(c6.getX() < minx) {minx = c6.getX();} if(c6.getY() < miny) {miny = c6.getY();} if(c6.getZ() < minz) {minz = c6.getZ();}
				if(c7.getX() < minx) {minx = c7.getX();} if(c7.getY() < miny) {miny = c7.getY();} if(c7.getZ() < minz) {minz = c7.getZ();}
				if(c8.getX() < minx) {minx = c8.getX();} if(c8.getY() < miny) {miny = c8.getY();} if(c8.getZ() < minz) {minz = c8.getZ();}

				if(c1.getX() > maxx) {maxx = c1.getX();} if(c1.getY() > maxy) {maxy = c1.getY();} if(c1.getZ() > maxz) {maxz = c1.getZ();}
				if(c2.getX() > maxx) {maxx = c2.getX();} if(c2.getY() > maxy) {maxy = c2.getY();} if(c2.getZ() > maxz) {maxz = c2.getZ();}
				if(c3.getX() > maxx) {maxx = c3.getX();} if(c3.getY() > maxy) {maxy = c3.getY();} if(c3.getZ() > maxz) {maxz = c3.getZ();}
				if(c4.getX() > maxx) {maxx = c4.getX();} if(c4.getY() > maxy) {maxy = c4.getY();} if(c4.getZ() > maxz) {maxz = c4.getZ();}
				if(c5.getX() > maxx) {maxx = c5.getX();} if(c5.getY() > maxy) {maxy = c5.getY();} if(c5.getZ() > maxz) {maxz = c5.getZ();}
				if(c6.getX() > maxx) {maxx = c6.getX();} if(c6.getY() > maxy) {maxy = c6.getY();} if(c6.getZ() > maxz) {maxz = c6.getZ();}
				if(c7.getX() > maxx) {maxx = c7.getX();} if(c7.getY() > maxy) {maxy = c7.getY();} if(c7.getZ() > maxz) {maxz = c7.getZ();}
				if(c8.getX() > maxx) {maxx = c8.getX();} if(c8.getY() > maxy) {maxy = c8.getY();} if(c8.getZ() > maxz) {maxz = c8.getZ();}
				}
			else
				{
				matrix o2w = this->geto2w();
				vect c1 = o2w * this->getTriangle().getV1();
				vect c2 = o2w * this->getTriangle().getV2();
				vect c3 = o2w * this->getTriangle().getV3();

				if(c1.getX() < minx) {minx = c1.getX();} if(c1.getY() < miny) {miny = c1.getY();} if(c1.getZ() < minz) {minz = c1.getZ();}
				if(c2.getX() < minx) {minx = c2.getX();} if(c2.getY() < miny) {miny = c2.getY();} if(c2.getZ() < minz) {minz = c2.getZ();}
				if(c3.getX() < minx) {minx = c3.getX();} if(c3.getY() < miny) {miny = c3.getY();} if(c3.getZ() < minz) {minz = c3.getZ();}


				if(c1.getX() > maxx) {maxx = c1.getX();} if(c1.getY() > maxy) {maxy = c1.getY();} if(c1.getZ() > maxz) {maxz = c1.getZ();}
				if(c2.getX() > maxx) {maxx = c2.getX();} if(c2.getY() > maxy) {maxy = c2.getY();} if(c2.getZ() > maxz) {maxz = c2.getZ();}
				if(c3.getX() > maxx) {maxx = c3.getX();} if(c3.getY() > maxy) {maxy = c3.getY();} if(c3.getZ() > maxz) {maxz = c3.getZ();}
				}
			this -> box = bbox(vect(minx,miny,minz,1), vect(maxx,maxy,maxz,1));
			}

		bbox getBbox()
			{
			return this -> box;
			}
	};

#endif