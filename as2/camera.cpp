#include "camera.h"

camera::camera()
	{
	this -> location = vect(0,0,0,1);
	this -> view = vect(0,0,-1,0);
    this -> up = vect(0,1,0,0);
	this -> u = vect(1,0,0,0);
	this -> v = vect(0,1,0,0);
	this -> w = vect(0,0,1,0);
	}

camera::camera(vect location, vect view, vect up)
	{
	this -> location = location.getUnitVector();
	this -> view = view.getUnitVector();
    this -> up = up.getUnitVector();
    this -> u = this -> view * this -> up;
	this -> v = up.getUnitVector();
	this -> w = vect(view.getX(), view.getY(), -1 * view.getZ(), view.getH());
	}

vect camera::getLocation()
	{
	return this -> location;
	}

vect camera::getView()
	{
	return this -> view;
	}

vect camera::getUp()
    {
	return this -> up;
    }

vect camera::getV()
    {
	return this -> v;
    }

vect camera::getU()
    {
	return this -> u;
    }

vect camera::getW()
    {
	return this -> w;
    }
