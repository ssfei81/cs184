#include "color.h"

#ifndef _BRDF_H_
#define _BRDF_H_

class brdf
	{
	private:
		color kd, ks, ka,kr, em;
		float sp, rindex;
	
	public:	
		brdf() //default constructor
			{
			this->kd = color(0,0,0);
			this->ks = color(0,0,0);
			this->ka = color(0,0,0);
			this->kr = color(0,0,0);
			this->em = color(0,0,0);
			this->sp = 0;
			this->rindex = 0;
			}

		brdf(color ka, color kd, color ks,  color kr, color em, float sp, float rindex)
			{
			this->kd = kd;
			this->ks = ks;
			this->ka = ka;
			this->kr = kr;
			this->em = em;
			this->sp = sp;
			this->rindex = rindex;
			}
		
		//set / get methods
		color getkd() {return this->kd;}
		color getks() {return this->ks;}
		color getka() {return this->ka;}
		color getkr() {return this->kr;}
		color getem() {return this->em;}
		float getsp() {return this->sp;}
		float getrindex() {return this->rindex;}
	};
#endif