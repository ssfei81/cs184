#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"

//auto converts to unit camera
class camera
	{
	private:
		vect location, view, up, v, w, u;
	
	public:	
		camera() //default constructor
			{
			this -> location = vect(0,0,0,1);
			this -> view = vect(0,0,-1,0);
			this -> up = vect(0,1,0,0);
			this -> u = vect(1,0,0,0);
			this -> v = vect(0,1,0,0);
			this -> w = vect(0,0,1,0);
			}
		camera(vect location, vect view, vect up) //constructor
			{
			this -> location = location.getUnitVector();
			this -> view = view.getUnitVector();
			this -> up = up.getUnitVector();
                this -> w = view.getUnitVector();
                this -> u = (this->w * this->up).getUnitVector();
                this -> v = (this->u * this->w).getUnitVector();
			}

		//set / get methods
		vect getLocation() {return this -> location;}
		vect getView() {return this -> view;}
		vect getUp() {return this -> up;}
		vect getV() {return this -> v;}
		vect getW() {return this -> w;}
		vect getU() {return this -> u;}
	};

#endif