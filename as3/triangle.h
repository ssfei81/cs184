#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include<iostream>
#include<cmath>
#include "vect.h"
using namespace std;

//auto converts to unit vector
class triangle
	{
	public:
        vect p1, p2, p3;
        float p1u, p1v, p2u, p2v, p3u, p3v;

		triangle() {} //default constructor
		
        triangle(vect v1, vect v2, vect v3) //constructor
        {
            this->p1 = v1;
            this->p2 = v2;
            this->p3 = v3;
        }
        
        vect mid12() {
            return (p1 + p2) * 0.5;
        }
        
        vect mid13() {
            return (p1 + p3) * 0.5;
        }
        
        vect mid23() {
            return (p2 + p3) * 0.5;
        }
        
        void setUV(float p1u, float p1v, float p2u, float p2v, float p3u, float p3v) {
            this->p1u = p1u;
            this->p1v = p1v;
            this->p2u = p2u;
            this->p2v = p2v;
            this->p3u = p3u;
            this->p3v = p3v;
        }
        
	};
#endif