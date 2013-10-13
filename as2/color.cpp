#include "color.h"

color::color()
	{
	//initialize color to (0,0,0)
	this -> red = 0;
	this -> green = 0;
    this -> blue = 0;
	}

color::color(float r, float g, float b)
    {
	this -> red = r;
	this -> green = g;
    this -> blue = b;
	}

float color::getR()
	{
	return this->red;
	}

float color::getG()
	{
	return this->green;
	}

float color::getB()
    {
	return this->blue;
    }

