#include "film.h"

film::film()
	{
	this -> width = 640;
	this -> height = 480;
	}

film::film(float width, float height, float distance, float fov, vect origin)
	{
	this -> width = width;
	this -> height = height;
	this -> distance = distance;
	this -> fov = fov;
	this -> origin = origin;

	float yUnit = 2 * tan(fov/2) * distance;
	this -> ppuy = height/yUnit;

	float xUnit = yUnit * width / height;
	this -> ppux = width / xUnit;

	this->fovh = 2 * atan((xUnit/2)/distance);
	}

float film::getHeight()
	{
	return this -> height;
	}

float film::getWidth()
	{
	return this -> width;
	}

float film::getDistance()
	{
	return this -> distance;
	}

float film::getFov()
	{
	return this -> fov;
	}

vect film::getPoint(float x, float y)
	{
	float centerX = this -> width /2;
	float centerY = this -> height/2;

	float i = (float)(x + 0.5-centerX);
	float j = (float)-1*(y + 0.5-centerY);

	return vect(i/ppux,j/ppuy,origin.getZ(),1);
	}
