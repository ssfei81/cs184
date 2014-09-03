#ifndef _COLOR_H_
#define _COLOR_H_

//auto converts to unit color
class color
	{
	private:
		float red, green, blue;
	
	public:	
		color() //default constructor
			{
			this -> red = 0;
			this -> green = 0;
			this -> blue = 0;
			}
		color(float r, float g, float b) //constructor
			{
			this -> red = r;
			this -> green = g;
			this -> blue = b;
			}
		//set / get methods
		float getR() {return this->red;}
		float getG() {return this->green;}
		float getB() {return this->blue;}

		color operator+(color c2)
			{
			float newR = this->getR() + c2.getR();
			float newG = this->getG() + c2.getG();
			float newB = this->getB() + c2.getB();
			return color(newR, newG, newB);
			}        
		color operator*(color c2)
			{
			float newR = this->getR() * c2.getR();
			float newG = this->getG() * c2.getG();
			float newB = this->getB() * c2.getB();
			return color(newR, newG, newB);
			}

        color operator*(float f)
			{
			float newR = this->getR() * f;
			float newG = this->getG() * f;
			float newB = this->getB() * f;
			return color(newR, newG, newB);
			}
	};

#endif