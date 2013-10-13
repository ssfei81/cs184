#include "vect.h"
#include "math.h"

vect::vect()
	{
	//initialize vector to (0,0,0)
	this -> x = 0;
	this -> y = 0;
	this -> z = 0;
	}

vect::vect(float x, float y, float z)
	{
	
	this -> x = x;
	this -> y = y;
	this -> z = z;
	}

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

vect vect::getUnitVector()
	{
	float x = this -> x;
	float y = this -> y;
	float z = this -> z;
	float dist = sqrt(x*x + y*y + z*z);

	return vect(this -> x / dist, this -> y / dist, this -> z / dist);
	}

vect vect::operator+(const vect& vect2)
	{
	float newX = this->getX() + vect2.getX();
	float newY = this->getY() + vect2.getY();
	float newZ = this->getZ() + vect2.getZ();
	return vect(newX, newY, newZ);
	}
vect vect::operator*(const float multiplier)
	{
	float newX = this->getX() * multiplier;
	float newY = this->getY() * multiplier;
	float newZ = this->getZ() * multiplier;
	return vect(newX, newY, newZ);

	}

vect vect::operator-(const vect& vect2)
	{
	float newX = this->getX() - vect2.getX();
	float newY = this->getY() - vect2.getY();
	float newZ = this->getZ() - vect2.getZ();
	return vect(newX, newY, newZ);
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
	return vect(newX, newY, newZ);
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

vect operator*(float multiplier, vect &v)
	{
	float newX = v.getX() * multiplier;
	float newY = v.getY() * multiplier;
	float newZ = v.getZ() * multiplier;
	return vect(newX, newY, newZ);
	}