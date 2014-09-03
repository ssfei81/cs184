#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "vect.h"
#include<iostream>
using namespace std;

//auto converts to unit vector
class triangle
	{
	private:
		Vector p1, p2, p3;
        Vector n1, n2, n3;
		Vector t1, t2, t3;
		string mtl;
	public:
		triangle() {} //default constructor

        triangle(Vector v1, Vector v2, Vector v3) //constructor
			{
            this->p1 = v1; this->p2 = v2; this->p3 = v3;
			}

        triangle(Vector v1, Vector v2, Vector v3, Vector n1, Vector n2, Vector n3, int t) //constructor
			{
			if(t == 0)
				{
				this->p1 = v1; this->p2 = v2; this->p3 = v3;
				this->n1 = n1; this->n2 = n2; this->n3 = n3;
				}
			else
				{
				this->p1 = v1; this->p2 = v2; this->p3 = v3;
				this->t1 = n1; this->t2 = n2; this->t3 = n3;
				}
			}

        triangle(Vector v1, Vector v2, Vector v3, Vector n1, Vector n2, Vector n3, Vector t1, Vector t2, Vector t3) //constructor
			{
            this->p1 = v1; this->p2 = v2; this->p3 = v3;
			this->n1 = n1; this->n2 = n2; this->n3 = n3;
			this->t1 = t1; this->t2 = t2; this->t3 = t3;
			}

		Vector getP1() {return this->p1;}
		Vector getP2() {return this->p2;}
		Vector getP3() {return this->p3;}
		Vector getN1() {return this->n1;}
		Vector getN2() {return this->n2;}
		Vector getN3() {return this->n3;}
		Vector getT1() {return this->t1;}
		Vector getT2() {return this->t2;}
		Vector getT3() {return this->t3;}
		string getMtl() {return this->mtl;}
		void setMtl(string mtl){this->mtl = mtl;}
	};
#endif