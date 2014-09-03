#ifndef _VECT_H_
#define _VECT_H_

#include<iostream>
#include<cmath>
using namespace std;

//auto converts to unit vector
class vect
	{
	private:
		float x,y,z,h;
	
	public:	
		vect(); //default constructor
		vect(float, float, float, float); //constructor
		
		//set / get methods
		float getX() const;
		float getY() const;
		float getZ() const;
		float getH() const;
		void setX(float);
		void setY(float);
		void setZ(float);
		void setH(float);
		vect getUnitVector();
		float getAngle(vect vect2);


		vect operator+(const vect&);
		vect operator-(const vect&);
		vect operator*(const vect&);
		float operator^(const vect&); //dot product
		vect operator*(const float);

		friend ostream& operator<< (ostream &out, vect &vect);
		friend vect operator*(float, vect&); //cross product
	};
#endif