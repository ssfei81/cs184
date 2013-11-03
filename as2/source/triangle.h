#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <iostream>
#include "ray.h"
#include <vector>
using namespace std;


//auto converts to unit sphere
class triangle
	{
	private:
		float v1, v2, v3;
		vector<vect*> *vertices;
		vect normal;
	public:	
		triangle() //default constructor
			{

			}
		triangle(vector<vect*> *vertices,float v1, float v2, float v3) //constructor
			{
			this -> v1 = v1;
			this -> v2 = v2;
			this -> v3 = v3;
			this -> vertices = vertices;

			vector<vect*> &v = *vertices;

			this -> normal = (*v[v2] - *v[v1]) * (*v[v3] - * v[v2]);
			}
		//set / get methods

		vect getNormal() 
			{
			return this->normal;
			}
		vect getV1(){return *(this->vertices->at(this->v1));}
		vect getV2(){return *(this->vertices->at(this->v2));}
		vect getV3(){return *(this->vertices->at(this->v3));}

        bool getIntersection(ray &r,float *thit, vect *intersect)
			{		
			vector<vect*> &v = *vertices;
			vect ar = *v[v1],br = *v[v2],cr = *v[v3], pr = r.getPoint(), dr = r.getDirection();
			float xa = ar.getX(), xb = br.getX(), xc = cr.getX(), xd = dr.getX(), xe = pr.getX();
			float ya = ar.getY(), yb = br.getY(), yc = cr.getY(), yd = dr.getY(), ye = pr.getY();
			float za = ar.getZ(), zb = br.getZ(), zc = cr.getZ(), zd = dr.getZ(), ze = pr.getZ();

			float a = xa-xb, b = ya-yb, c = za-zb;
			float d = xa-xc, e = ya-yc, f = za-zc;
			float g = xd, h = yd, i = zd;
			float j = xa-xe, k = ya-ye, l = za-ze;

			float eihf = e*i - h*f, gfdi = g*f - d*i, dheg = d*h - e*g;
			float akjb = a*k - j*b, jcal = j*c - a*l, blkc = b*l - k*c;
			float M = a*eihf+b*gfdi+c*dheg;

			float beta = (j * eihf + k * gfdi + l * dheg) / M;
			float gamma =(i * akjb + h * jcal + g * blkc) / M;
			float t = -1 * (f * akjb + e*jcal + d*blkc) / M;

			if (gamma < 0 || gamma > 1) return false;
			if (beta < 0 || beta > 1 - gamma) return false;

            //cout<< t<<endl;
			if (t<r.getTmin()) return false;
			if (t>r.getTmax()) return false;
			*thit = t;
			*intersect = r.getPoint() + r.getDirection() * t;
			return true;
			}
        bool intersectP(ray &r)
        {
			vector<vect*> &v = *vertices;
			vect ar = *v[v1],br = *v[v2],cr = *v[v3], pr = r.getPoint(), dr = r.getDirection();
			float xa = ar.getX(), xb = br.getX(), xc = cr.getX(), xd = dr.getX(), xe = pr.getX();
			float ya = ar.getY(), yb = br.getY(), yc = cr.getY(), yd = dr.getY(), ye = pr.getY();
			float za = ar.getZ(), zb = br.getZ(), zc = cr.getZ(), zd = dr.getZ(), ze = pr.getZ();
            
			float a = xa-xb, b = ya-yb, c = za-zb;
			float d = xa-xc, e = ya-yc, f = za-zc;
			float g = xd, h = yd, i = zd;
			float j = xa-xe, k = ya-ye, l = za-ze;
            
			float eihf = e*i - h*f, gfdi = g*f - d*i, dheg = d*h - e*g;
			float akjb = a*k - j*b, jcal = j*c - a*l, blkc = b*l - k*c;
			float M = a*eihf+b*gfdi+c*dheg;

			float beta = (j * eihf + k * gfdi + l * dheg) / M;
			float gamma =(i * akjb + h * jcal + g * blkc) / M;
			float t = -1*(f * akjb + e*jcal + d*blkc) / M;

			if (t<r.getTmin()) return false;
			if (t>r.getTmax()) return false;
			if (gamma < 0 || gamma > 1) return false;
			if (beta < 0 || beta > 1 - gamma) return false;
            
            //cout<<t<<endl;
			return true;
        }
	};
#endif