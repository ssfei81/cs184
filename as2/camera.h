#include "ray.h"

//auto converts to unit camera
class camera
	{
	private:
		vect location, view, up, v, w, u;
	
	public:	
		camera(); //default constructor
		camera(vect, vect, vect); //constructor
		
		//set / get methods
		vect getLocation();
		vect getView();
        vect getUp();
        vect getV();
        vect getW();
		vect getU();
	};