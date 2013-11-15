#ifndef _BEZIER_H_
#define _BEZIER_H_

#include <iostream>
#include <cmath>
#include "vect.h"
using namespace std;

struct bezPt
	{
	vect pt;
	vect gradient;
	};


//auto converts to unit vector
class bezCurve
	{
	private:
		//control points
        vect cp[4];
	public:	
		bezCurve() {}
        
        bezCurve(vect v[4])
			{
            for (int x = 0; x < 4;x++) this->cp[x] = v[x];
			}

		vect getPt(int i) 
			{
			return this->cp[i];
			}

        //return a struct {pt, gradient}
        bezPt bezcurveinterp(bezCurve curve, float u)
			{
			vect A = curve.getPt(0) * (1-u) + curve.getPt(1) * (u);
            vect B = curve.getPt(1) * (1-u) + curve.getPt(2) * (u);
            vect C = curve.getPt(2) * (1-u) + curve.getPt(3) * (u);
            
            vect D = A * (1-u) + B * (u);
            vect E = B * (1-u) + C * (u);
            
			//final point
            vect p = D * (1-u) + E * (u);
			
			//derivative
			vect dPdu = 3 * (E-D);

			bezPt pt = {p,dPdu}; 
			return pt;
			}
	};

class bezPatch:bezCurve
	{
	private:
		vect cp[4][4];
		vect original_cp[4][4];

	public:
		bezPatch(){}
		bezPatch(vect v[4][4])
			{
			for (int x = 0; x < 4;x++)
				{
				for (int y = 0; y < 4; y++) {this->cp[x][y] = v[x][y]; this->original_cp[x][y] = v[x][y];}
				}
			}

		//get control points
		vect* getCPu(int u) 
			{
			return this->cp[u];
			}

		vect* getCPv(int v, vect* t)
			{
			for(int x = 0; x < 4; x++) t[x] = this->cp[x][v];
			return t;
			}

		void resize(float scale)
			{
			for (int x = 0; x < 4;x++)
				{
				for (int y = 0; y < 4; y++) 
					{
					this->cp[x][y].setX(this->original_cp[x][y].getX() * scale);
					this->cp[x][y].setY(this->original_cp[x][y].getY() * scale);
					this->cp[x][y].setZ(this->original_cp[x][y].getZ() * scale);
					}
				}
			}
		//returns a struct {pt, normal vector}
		bezPt bezpatchinterp(bezPatch patch, float u, float v)
			{
			int x;
			//build controlPoints in V
			vect vPt[4];
			
			for(x = 0; x < 4;x++) vPt[x] = this->bezcurveinterp(bezCurve(patch.getCPu(x)),u).pt;
			bezCurve vCurve = bezCurve(vPt);

			vect tmp[4], uPt[4];
			for(x = 0; x < 4;x++) uPt[x] = this->bezcurveinterp(bezCurve(patch.getCPv(x, tmp)),v).pt;
			bezCurve uCurve = bezCurve(uPt);

			bezPt Ptv = this->bezcurveinterp(vCurve, v);
			bezPt Ptu = this->bezcurveinterp(uCurve, u);

			vect n; 		

			n = (Ptu.gradient * Ptv.gradient);			
			n = n.getUnitVector();
	
			bezPt pt = {Ptv.pt, n};
			return pt;
			}
	};
#endif