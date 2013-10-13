using namespace std;
#include "vect.h"


class matrix
	{
	private:
        float **m;
        
	public:	
		matrix(); //default constructor
		matrix(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
        float getVal(int, int);
	
		matrix operator+(const matrix&);
		matrix operator-(const matrix&);
		matrix operator*(const float);
        matrix operator*(const matrix&);
        vect operator*(const vect&);
        
        matrix translate(float, float, float);
        matrix scale(float, float, float);
        matrix shear(float, float, float, float, float, float);
        matrix rotateX(float);
        matrix rotateY(float);
        matrix rotateZ(float);
        matrix invert();

	};