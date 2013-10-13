#include <iostream>
using namespace std;

//auto converts to unit vector
class vect
	{
	private:
		float x,y,z;
	
	public:	
		vect(); //default constructor
		vect(float, float, float); //constructor
		
		//set / get methods
		float getX() const;
		float getY() const;
		float getZ() const;
		vect getUnitVector();

		vect operator+(const vect&);
		vect operator-(const vect&);
		vect operator*(const vect&);
		float operator^(const vect&);
		vect operator*(const float);

		friend ostream& operator<< (ostream &out, vect &vect);
		friend vect operator*(float, vect&);
	};