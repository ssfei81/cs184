#include "vect.h"

vect::vect()
	{
	//initialize vector to (0,0,0,0)
	this -> x = 0;
	this -> y = 0;
	this -> z = 0;
	this -> h = 0;
	}

vect::vect(float x, float y, float z, float h)
	{
	
	this -> x = x;
	this -> y = y;
	this -> z = z;
	this -> h = h;
	}

//////////
// GET //
////////

float vect::getX() const
	{
	return this->x;
	}
float vect::getY() const
	{
	return this->y;
	}
float vect::getZ() const
	{
	return this->z;
	}
float vect::getH() const
	{
	return this->h;
	}

//////////
// SET //
////////

void vect::setX(float x)
	{
	this -> x = x;
	}
void vect::setY(float y)
	{
	this -> y = y;
	}
void vect::setZ(float z)
	{
	this -> z = z;
	}
void vect::setH(float h)
	{
	this -> h = h;
	}

vect vect::getUnitVector()
	{
	float x = this -> x;
	float y = this -> y;
	float z = this -> z;
	float h = this -> h;
	float dist = sqrt(x*x + y*y + z*z);
	if(dist == 0) dist = 1;

	return vect (x/dist, y/dist, z/dist, h);
	}

float vect::getAngle(vect vect2)
	{
		float dotproduct = vect(this->x, this->y, this->z,this->h).getUnitVector()^vect2.getUnitVector();
		return acos(dotproduct);
	}

float vect::getMagnitude()
	{
	return sqrt(pow(this->x, 2) + pow(this->y,2) + pow(this->z, 2));
	}


vect vect::operator+(const vect& vect2)
	{
	float newX = this->getX() + vect2.getX();
	float newY = this->getY() + vect2.getY();
	float newZ = this->getZ() + vect2.getZ();
	float newH = 0;
	return vect(newX, newY, newZ, newH);
	}
vect vect::operator*(const float multiplier)
	{
	float newX = this->getX() * multiplier;
	float newY = this->getY() * multiplier;
	float newZ = this->getZ() * multiplier;
	float newH = 0;
	return vect(newX, newY, newZ, newH);
	}

vect vect::operator-(const vect& vect2)
	{
	float newX = this->getX() - vect2.getX();
	float newY = this->getY() - vect2.getY();
	float newZ = this->getZ() - vect2.getZ();
	float newH = 0;
	return vect(newX, newY, newZ, newH);
	}

vect vect::operator*(const vect& vect2) // cross product
	{
	float a1 = this->getX();
	float a2 = this->getY();
	float a3 = this->getZ();
	float b1 = vect2.getX();
	float b2 = vect2.getY();
	float b3 = vect2.getZ();

	float newX = a2*b3 - a3*b2;
	float newY = a3*b1 - a1*b3;
	float newZ = a1*b2 - a2*b1;
	float newH = 0;

	return vect(newX, newY, newZ, newH);
	}

float vect::operator^(const vect& vect2) // dot product
	{
	float a1 = this->getX();
	float a2 = this->getY();
	float a3 = this->getZ();
	float b1 = vect2.getX();
	float b2 = vect2.getY();
	float b3 = vect2.getZ();

	return a1*b1 + a2*b2 + a3*b3;
	}

ostream& operator<< (ostream &out, vect& vect)
	{
	out << "(" <<vect.getX()<<","<<vect.getY()<<","<<vect.getZ()<<")";
	return out;
	}

vect operator*(float multiplier, vect &v) // multiply by a constant
	{
	float newX = v.getX() * multiplier;
	float newY = v.getY() * multiplier;
	float newZ = v.getZ() * multiplier;
	float newH = 0;
	return vect(newX, newY, newZ, newH);
	}