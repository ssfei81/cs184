
//auto converts to unit color
class color
	{
	private:
		float red, green, blue;
	
	public:	
		color(); //default constructor
		color(float, float, float); //constructor
		
		//set / get methods
		float getR();
		float getG();
        float getB();
	};