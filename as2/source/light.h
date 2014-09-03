#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "ray.h"
#include "color.h"
#include "vect.h"
#include <cfloat>

//auto converts to unit ray
class light
	{
	protected:
        vect pos, att;
        color c;
        double type; //0 for direction, 1 for position
	public:
        light() 
			{ //default direction light from origin
            this -> pos = vect(0,0,0,1);
            this -> c = color();
            this -> att = vect(0,0,0,0);
            this -> type = 0;
			}
        light(float x, float y, float z, float r, float g, float b, vect attCoefs, double lightType)
			{
            this -> pos = vect(x, y, z, 1);
            this -> c = color(r, g, b);
            this -> att = attCoefs;
            this -> type = lightType;
			}

        ray generateLightRay(vect intersection, vect lightPos)
			{
            if (this->type == 0) 
				{ //direction
				vect dir = (this->pos).getUnitVector();
				return ray(intersection, dir,(float)0.005, FLT_MAX);
				} 
			else	
				{ //position
				vect dir = (this->pos-intersection).getUnitVector();
				float tmax = (lightPos - intersection).getX()/dir.getX();
				return ray(intersection, dir, (float)0.005, tmax);
				}
			}

        float getAttenuation(vect point) 
			{
            float x = point.getX() - this->pos.getX();
            float y = point.getY() - this->pos.getY();
            float z = point.getZ() - this->pos.getZ();
            float d = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); //find distance
            float denominator = (this->att.getX() + this->att.getY() * d + this->att.getZ() * d * d); //denominator of equation
            if (denominator == 0) 
				{ // attenuation not defined yet
                return 1.0f;
				}
			else 
				{
                return 1 / denominator;
				}
			}
        double lightType() 
			{
            return this->type;
			}
        vect getPos() 
			{
            return this->pos;
			}
        color getColor() 
			{
            return this->c;
			}
	};

#endif